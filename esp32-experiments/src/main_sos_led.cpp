#include <Arduino.h>

#define ONBOARD_LED 2

void setup3() {
  pinMode(ONBOARD_LED, OUTPUT);
}

void loop3() {
  delay(3000);

  digitalWrite(ONBOARD_LED, HIGH);
  delay(400);
  digitalWrite(ONBOARD_LED, LOW);
  delay(400);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(400);
  digitalWrite(ONBOARD_LED, LOW);
  delay(400);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(400);
  digitalWrite(ONBOARD_LED, LOW);

  delay(1200);

  digitalWrite(ONBOARD_LED, HIGH);
  delay(1200);
  digitalWrite(ONBOARD_LED, LOW);
  delay(400);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(1200);
  digitalWrite(ONBOARD_LED, LOW);
  delay(400);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(1200);
  digitalWrite(ONBOARD_LED, LOW);
  
  delay(1200);

  digitalWrite(ONBOARD_LED, HIGH);
  delay(400);
  digitalWrite(ONBOARD_LED, LOW);
  delay(400);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(400);
  digitalWrite(ONBOARD_LED, LOW);
  delay(400);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(400);
  digitalWrite(ONBOARD_LED, LOW);
}