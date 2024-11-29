#include <Servo.h>
#include "SR04.h"

// ultrasonic sensor variables
#define TRIG_PIN 12
#define ECHO_PIN 11
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;

// button variables
#define BUTTON_PIN 2
#define LONG_PRESS_THRESHOLD 1000  // 1 second
bool buttonPressed = false;
unsigned long pressStartTime;

// servo variables
#define MIN_SERVO 0
#define MAX_SERVO 180
Servo myservo;
long minDistance = 0;
long maxDistance = 1000;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  myservo.attach(9); // digital pin 9 on the Arduino
}

void loop() {
  distance = sr04.Distance(); // distance measurement from the ultrasonic sensor

  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    // when button pressed
    if (buttonPressed == false) {
      buttonPressed = true;
      pressStartTime = millis();
    }

  } else {
    digitalWrite(LED_BUILTIN, LOW);
    // when button released
    if (buttonPressed) {
      buttonPressed = false;
      if (millis() - pressStartTime < LONG_PRESS_THRESHOLD) {
        minDistance = distance;
        Serial.print(distance);
        Serial.println("Short press for minimum");
      } else {
        maxDistance = distance;
        Serial.print(distance);
        Serial.println("Long press for maximum");
      }
    }
  }

  Serial.print(distance);
  Serial.println("cm");
  int servoAngle = map(distance, minDistance, maxDistance, MIN_SERVO, MAX_SERVO);
  myservo.write(servoAngle);
  delay(10);
}
