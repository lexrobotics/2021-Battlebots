/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - NodeMCU board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>


//BLYNK VALUES
char auth[] = "bwilCptQtp710DQOEdzOXD6tX4ud44DA";
char ssid[] = "";
char pass[] = "";


//PORTS
const uint8_t servoPin = D0;
const uint8_t leftMotorPin1 = D1;
const uint8_t leftMotorPin2 = D2;
const uint8_t rightMotorPin1 = D3;
const uint8_t rightMotorPin2 = D4;
const uint8_t ledPin = D5;

Servo servo;


//SERVO CONTROL
void servoContol(int pinValue) 
{
  servo.write(pinValue);
}


//MOTOR CONTROL
void leftMotorControl(int x)
{
  int dutyCycle = abs(x) * 1023 / 1000;
  if (x > 0) {
    //digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    analogWrite(leftMotorPin1, dutyCycle);
  } else if (x < 0) {
    digitalWrite(leftMotorPin1, LOW);
    //digitalWrite(leftMotorPin2, HIGH);
    analogWrite(leftMotorPin2, dutyCycle);
  }
  else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }
}

void rightMotorControl(int x)
{
  int dutyCycle = abs(x) * 1023 / 1000;
  if (x > 0) {
    //digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    analogWrite(rightMotorPin1, dutyCycle);
  } else if (x < 0) {
    digitalWrite(rightMotorPin1, LOW);
    //digitalWrite(rightMotorPin2, HIGH);
    analogWrite(rightMotorPin2, dutyCycle);
  }
  else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }
}



//BLYNK VIRTUAL PORTS
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  Serial.print("V0 Slider value is: ");
  Serial.println(pinValue);
  servoContol(pinValue);
}

BLYNK_WRITE(V1)
{
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("Left X = ");
  Serial.print(x);
  Serial.print("; Left Y = ");
  Serial.println(y);

  leftMotorControl(x);
}

BLYNK_WRITE(V2)
{
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("Right X = ");
  Serial.print(x);
  Serial.print("; Right Y = ");
  Serial.println(y);

  rightMotorControl(x);
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  servo.attach(servoPin);
  servo.write(0);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
}

void loop()
{
  Blynk.run();
}
