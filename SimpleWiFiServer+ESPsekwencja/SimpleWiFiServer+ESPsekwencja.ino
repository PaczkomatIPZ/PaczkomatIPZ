/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */
#include <ESP32Servo.h>
#include <WiFi.h>

const char* ssid     = "HUAWEI";
const char* password = "12345678";

WiFiServer server(80);
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

const int buttonPin = 25;
const int buttonPin2 = 33;
const int doorClosedLed = 26;

const int relayBigPin_1 = 14;
const int relayBigPin_2 = 27;


void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    WiFi.setSleep(false);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();
    Serial.begin(9600);
  
    pinMode(buttonPin, INPUT);
    pinMode(buttonPin2, INPUT);
    
    pinMode(relayBigPin_1, OUTPUT);
    pinMode(relayBigPin_2, OUTPUT);
    pinMode(doorClosedLed, OUTPUT);
    
    myservo.attach(32);
    myservo.write(90);
    digitalWrite(doorClosedLed, LOW);
    digitalWrite(relayBigPin_1, HIGH);
    digitalWrite(relayBigPin_2, HIGH);

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("KLIKNIJ <a href=\"/H\">TU</a> BY OTWORZYC PACZKOMAT.<br>");
            //client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":   
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
        
        ///////////////////////////////////////////////
          
           if (currentLine.endsWith("GET /H"))  {
            digitalWrite(relayBigPin_1, LOW);
            digitalWrite(doorClosedLed, LOW);
            delay(1000);
            for (pos = 90; pos >= 0; pos -= 1) { 
            myservo.write(pos);              
            delay(1); 
            }
            digitalWrite(relayBigPin_1, HIGH);
            delay(1000);
            digitalWrite(relayBigPin_1, LOW);                     
          } 
        
        ///////////////////////////////////////////////
        
          if (pushCounter2 == 1 && pos == -1) {
            delay(200);
            for (pos = 0; pos <= 90; pos += 1) { 
            myservo.write(pos);              
            delay(1);                       
            }
            delay(1000);
            digitalWrite(relayBigPin_1, HIGH);
            digitalWrite(doorClosedLed, HIGH);                     
            pos -=1;
          }
        
        //////////////////////////////////////////////
         
          if (pushCounter >= 1) {
            pushCounter = 0;
          }
        
           if (pushCounter2 >= 1) {
            i = 0;
            j = 0;
            pushCounter2 = 0;
          }
          
          delay(100);
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
