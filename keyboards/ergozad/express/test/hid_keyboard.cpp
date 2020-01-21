/*
 * Arduino.h is absolutely necessary to correctly compile this object
 * In case it isn't included symbols aren't correctly named and
 * linking ends up complainig that loop ans setup aren't defined
 * anywere.
 */
#include <variant.h>
#include <Arduino.h>

#include <delay.h>
void setup();
void loop();

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
}
