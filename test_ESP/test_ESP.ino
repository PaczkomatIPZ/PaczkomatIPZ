#include <ESP32Servo.h>

Servo myservo;

int i = 0;
int j = 0;
int pos = 90;
int currentState;
int currentState2;        
int pushCounter = 0; 
int pushCounter2 = 0;     
int lastState = HIGH;
int lastState2 = HIGH; 

const int buttonPin = 13;
const int buttonPin2 = 12;

const int relayBigPin_1 = 14;
const int relayBigPin_2 = 27;

void setup() {
  Serial.begin(9600);
  
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  
  pinMode(relayBigPin_1, OUTPUT);
  pinMode(relayBigPin_2, OUTPUT);
  
  myservo.attach(32);
}

void loop() {
  currentState = digitalRead(buttonPin);
  if (currentState != lastState) {
    if (currentState == LOW) {
      pushCounter++;
    }
  }
  lastState = currentState;
  
  currentState2 = digitalRead(buttonPin2);
  if (currentState2 != lastState2) {
    if (currentState2 == LOW) {
      pushCounter2++;
    }
  }
  lastState2 = currentState2;
  
  if (pushCounter == 1 && pos == 90) {
    for (pos = 90; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(1); 
    }                      
  }
  
  if (pushCounter == 2 && pos == -1) {
    for (pos = 0; pos <= 90; pos += 1) { 
    myservo.write(pos);              
    delay(1);                       
    }
    pos -=1;
  }

  if(pushCounter2 == 0){
     digitalWrite(relayBigPin_1, HIGH);
     digitalWrite(relayBigPin_2, HIGH);
  }

  if(pushCounter2 == 1 && i == 0){
     digitalWrite(relayBigPin_1, LOW);
     digitalWrite(relayBigPin_2, LOW);
     i++;
  }

   if(pushCounter2 == 2 && j == 0){
     digitalWrite(relayBigPin_1, HIGH);
     digitalWrite(relayBigPin_2, HIGH);
     j++;
  }
  
  if (pushCounter >= 2) {
    pushCounter = 0;
  }

   if (pushCounter2 >= 2) {
    i = 0;
    j = 0;
    pushCounter2 = 0;
  }
  
  delay(1```````00);
}
