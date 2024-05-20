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
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
static const int RXPin = D7, TXPin = D8;
static const uint32_t GPSBaud = 9600;


// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL33k0PeAnp"
#define BLYNK_TEMPLATE_NAME "Andela Sai Ramana"
#define BLYNK_AUTH_TOKEN "EA5kLzkOB5kHF10wieu1unQ5SFXZTFoY"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sai";
char pass[] = "12345678";
const int m1[] = {D1, D2};
const int m2[] = {D3, D4};

void setup()
{
  // Debug consolek
  Serial.begin(9600);
  ss.begin(GPSBaud);
  for (int i = 0; i < 2; i++) {
    pinMode(m1[i], OUTPUT);
    pinMode(m2[i], OUTPUT);

  }
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();  
  if (Serial.available()) {
    //Serial.write(Serial.read());
    String myString = Serial.readStringUntil('\n');
    String t = getValue(myString, ',', 0);
    String S = getValue(myString, ',', 1);
    String G = getValue(myString, ',', 2);
    String M = getValue(myString, ',', 3);
    String C = getValue(myString, ',', 4);

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, S);
    Blynk.virtualWrite(V2, G);
    Blynk.virtualWrite(V3, M);
    Blynk.virtualWrite(V4, C);
  }
}

BLYNK_WRITE(V11) {
  int val = param.asInt();
  if (val) {
    //Blynk.virtualWrite(V0, "Forward");
    digitalWrite(m1[0], HIGH);
    digitalWrite(m2[0], HIGH);
    
  }
  else {
    //Blynk.virtualWrite(V0, ".......");
    digitalWrite(m1[0], LOW);
    digitalWrite(m2[0], LOW);
   
  }
}

BLYNK_WRITE(V12) {
  int val = param.asInt();
  if (val) {
    //Blynk.virtualWrite(V0, "Backward");
    digitalWrite(m1[1], HIGH);
    digitalWrite(m2[1], HIGH);
    
  }
  else {
    //Blynk.virtualWrite(V0, ".......");
    digitalWrite(m1[1], LOW);
    digitalWrite(m2[1], LOW);
    
  }
}

//BLYNK_WRITE(V21) {
//  int val = param.asInt();
//  if (val) {
//    //Blynk.virtualWrite(V0, "Right");
//    digitalWrite(m1[1], HIGH);
//    digitalWrite(m2[0], HIGH);
//    
//    
//  }
//  else {
//    //Blynk.virtualWrite(V0, ".......");
//    digitalWrite(m1[1], LOW);
//    digitalWrite(m2[0], LOW);
//    
//  }
//}
//
//BLYNK_WRITE(V22) {
//  int val = param.asInt();
//  if (val) {
//    //Blynk.virtualWrite(V0, "Left");
//    digitalWrite(m1[0], HIGH);
//    digitalWrite(m2[1], HIGH);
//    
//  }
//  else {
//    //Blynk.virtualWrite(V0, ".......");
//    digitalWrite(m1[0], LOW);
//    digitalWrite(m2[1], LOW);
//    
//  }
//}

void displayInfo()
{
  Blynk.virtualWrite(V7,F("Location: ")); 
  if (gps.location.isValid())
  {
    //Serial.print("LATITUDE:");
    //Serial.println(gps.location.lat(), 6);
    Blynk.virtualWrite(V8,gps.location.lat(),6);
    //Serial.print(F(","));
    //Serial.print("LONGITUDE:");
    //Serial.println(gps.location.lng(), 6);
    Blynk.virtualWrite(V9,gps.location.lng(),6);

  }
  else
  {
    Blynk.virtualWrite(V7,F("INVALID"));
  }
  delay(500);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
