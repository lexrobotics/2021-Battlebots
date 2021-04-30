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
#include <Wire.h>
#include <string>
#include <BlynkSimpleEsp8266.h>


//BLYNK VALUES
char auth[] = "bwilCptQtp710DQOEdzOXD6tX4ud44DA";
char ssid[] = "";
char pass[] = "";

void wireSend(string )

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
