from flask import Flask, redirect, render_template, request, jsonify, abort, flash
from flask_sqlalchemy import SQLAlchemy

from sqlalchemy import desc ,asc
from flask_marshmallow import Marshmallow
from marshmallow import Schema, fields, pprint
from datetime import datetime, timedelta
import  os
from os.path import isfile, join
from os import listdir
import json
from io import StringIO
from werkzeug.wrappers import Response
import itertools
import random
import string


app = Flask(__name__)
app.secret_key = 'development key'

SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
    username="tarnasivo",
    password="alamakota", # database password hidden
    hostname="tarnasivo.mysql.pythonanywhere-services.com",
    databasename="tarnasivo$paczkomat1",
)
app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
app.config["SQLALCHEMY_POOL_RECYCLE"] = 299 # connection timeouts
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False # no warning disruptions

db = SQLAlchemy(app)
ma = Marshmallow(app)

class Users(db.Model):

    __tablename__ = "users"
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(4096))


    def __init__(self, name):
        self.name = name

class UsersSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('id' ,'name')

user_schema = UsersSchema()
users_schema = UsersSchema(many=True)


@app.route("/users", methods=["GET"])
def get_all_users():
    user_many = Users.query.all()
    result = users_schema.dump(user_many)
    return jsonify(result)

@app.route("/user/<id>", methods=["GET"])
def get_all_user(id):
    user = Users.query.get(id)
    result = user_schema.dump(user)
    return jsonify(result)

@app.route("/users", methods=["POST"])
def add_user():
    name= request.json["name"]
    new_user = Users(name)
    db.session.add(new_user)
    db.session.commit()  # PK increment
    user = Users.query.get(new_user.id)
    return user_schema.jsonify(user)

@app.route("/web/users", methods=["GET"])
def get_users_nasz():
    all_users= Users.query.order_by(Users.name).all()
    result = users_schema.dump(all_users)
    return render_template('uzytkownicy.html', title='PaczkomatIPZ', users=result)


class Box_lockers(db.Model):

    __tablename__ = "box_locker"
    id = db.Column(db.Integer, primary_key=True)
    adress = db.Column(db.String(4096))


    def __init__(self, adress):
        self.adress = adress

class Box_lockersSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('id' ,'adress')

box_locker_schema = Box_lockersSchema()
box_lockers_schema = Box_lockersSchema(many=True)


@app.route("/box_lockers", methods=["GET"])
def get_all_box_lockers():
    box_locker_many = Box_lockers.query.all()
    result = box_lockers_schema.dump(box_locker_many)
    return jsonify(result)

@app.route("/box_locker/<id>", methods=["GET"])
def get_all_box_locker(id):
    box_locker = Box_lockers.query.get(id)
    result = box_locker_schema.dump(box_locker)
    return jsonify(result)

@app.route("/box_lockers", methods=["POST"])
def add_box_locker():
    adress= request.json["adress"]
    new_box_locker = Box_lockers(adress)
    db.session.add(new_box_locker)
    db.session.commit()  # PK increment
    box_locker = Box_lockers.query.get(new_box_locker.id)
    return box_locker_schema.jsonify(box_locker)

@app.route("/web/box_lockers", methods=["GET"])
def get_box_lockers_nasz():
    all_box_lockers = Box_lockers.query.all()
    #all_box_lockers= Box_lockers.query.order_by(Box_lockers.id).all()
    result = box_lockers_schema.dump(all_box_lockers)
    return render_template('paczkomaty.html', title='PaczkomatIPZ', box_lockers=result)


class Boxes(db.Model):

    __tablename__ = "box"
    id = db.Column(db.Integer, primary_key=True)
    occupy = db.Column(db.Boolean)
    code = db.Column(db.String(4096))
    size = db.Column(db.String(4096))
    box_locker_id = db.Column(db.Integer)


    def __init__(self, occupy, code, size, box_locker_id):
        self.occupy = occupy
        self.code = code
        self.size = size
        self.box_locker_id = box_locker_id

class BoxesSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('id', 'occupy', 'code', 'size', 'box_locker_id')

box_schema = BoxesSchema()
boxes_schema = BoxesSchema(many=True)


@app.route("/boxes", methods=["GET"])
def get_all_boxes():
    box_many = Boxes.query.all()
    result = boxes_schema.dump(box_many)
    return jsonify(result)

@app.route("/box/<id>", methods=["GET"])
def get_all_box(id):
    box = Boxes.query.get(id)
    result = box_schema.dump(box)
    return jsonify(result)

@app.route("/boxes", methods=["POST"])
def add_box():
    occupy= request.json["occupy"]
    code= request.json["code"]
    size= request.json["size"]
    box_locker_id= request.json["box_locker_id"]
    new_box = Boxes(occupy, code, size, box_locker_id)
    db.session.add(new_box)
    db.session.commit()  # PK increment
    box = Boxes.query.get(new_box.id)
    return user_schema.jsonify(box)


class Orders(db.Model):

    __tablename__ = "orders"
    box_id = db.Column(db.Integer)
    order_name = db.Column(db.Integer, primary_key=True)
    created_at = db.Column(db.DATETIME)
    receiver_id = db.Column(db.Integer)
    sender_id = db.Column(db.Integer)


    def __init__(self, box_id, created_at, receiver_id, sender_id):
        self.box_id = box_id
        self.created_at = created_at
        self.receiver_id = receiver_id
        self.sender_id = sender_id

class OrdersSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('box_id', 'order_name', 'created_at','receiver_id', 'sender_id')


order_schema = OrdersSchema()
orders_schema = OrdersSchema(many=True)

@app.route("/orders", methods=["GET"])
def get_all_orders():
    order_many = Orders.query.all()
    result = orders_schema.dump(order_many)
    return jsonify(result)

@app.route("/order/<id>", methods=["GET"])
def get_all_order(id):
    order = Orders.query.get(id)
    result = order_schema.dump(order)
    return jsonify(result)

@app.route("/orders", methods=["POST"])
def add_order():
    sender= request.json["sender_id"]
    receiver= request.json["receiver_id"]
    box= request.json["box_id"]
    new_order = Orders(box, datetime.now(), receiver, sender)
    db.session.add(new_order)
    db.session.commit()  # PK increment
    order = Orders.query.get(new_order.order_name)
    return order_schema.jsonify(order)


@app.route("/web/orders", methods=["GET"])
def get_orders_nasz():
    #all_orders= Orders.query.all()
    all_orders= Orders.query.order_by(Orders.order_name).all()
    result = orders_schema.dump(all_orders)
    return render_template('strona.html', title='PaczkomatIPZ', orders=result)