#include <Wire.h>
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
float t;
int tempPin = A0;
const int trig_Pin = 2;
const int echo_Pin = 3;
long duration;
int d;
#define sensor_pin 4
int sensor_value;

const int Pin = 6;
//int pirPin = 6;
String str, S, M, m, G, C;
int in1 = 8;
int in2 = 9;
int in3 = 12;
int buzzer = 11;
void setup() {
  Serial.begin(9600);
  // Initialize the TCS34725 sensor
  if (!tcs.begin()) {
    //Serial.println("Color sensor not found. Please check wiring!");
    while (1);
  }
  pinMode(trig_Pin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_Pin, INPUT);
  pinMode(Pin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(sensor_pin, INPUT);
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  get_temp();
  distance();
  gas_stat();
  //metal_status();
  colour();
  str = String(t) + "," + String(S) + "," + String(G)  + "," + String(C);
  Serial.println(str);
  delay(1000);
}

void get_temp() {
  t = analogRead(tempPin);
  // read analog volt from sensor and save to variable temp
  t = t * 0.48828125;
}

void distance() {
  // Clears the trigPin
  digitalWrite(trig_Pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Pin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_Pin, HIGH);
  // Calculating the distance
  d = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  if (d < 35 ) {

    S = "STOP!ALERT!";
    digitalWrite(buzzer, HIGH);
  }
  else {

    S = "FORWARD";
    digitalWrite(buzzer, LOW);
  }

}

void gas_stat() {
  sensor_value = digitalRead(sensor_pin);

  if (sensor_value == LOW) {
    G = "GAS Detected";
  }
  else {
    G = "GAS Not Detected";
  }
}

//void metal_status() {
//  int sensorValue = digitalRead(Pin);
//  if (sensorValue == LOW) {
//    M = "METAL DETECTED!";
//
//  }
//  else {
//    M = "..........";
//
//  }
//}

void colour() {
  // Read RGB color values from the TCS34725 sensor
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  // Print RGB values to serial monitor
//  Serial.print("Red: "); Serial.print(r);
//  Serial.print(" Green: "); Serial.print(g);
//  Serial.print(" Blue: "); Serial.print(b);
//  Serial.print(" Clear: "); Serial.println(c);
  
  // Check if the sensed color meets a certain condition (example: red)
  if (r > 200 && g < 100 && b < 100) {
    // Activate relay channels to perform an action (example: turn on devices)
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    C = "Red color";
  } else if (g > 100 && r < 100 && b < 100) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    C = "Green color";
  } else if (b > 100 && r < 100 && g < 100) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    C = "Blue color";
  } else {
    // Deactivate all relay channels if no color condition is met
    C = "UNKNOWN COLOR";
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
  }

  // Add a delay before the next reading
  delay(500);
}
