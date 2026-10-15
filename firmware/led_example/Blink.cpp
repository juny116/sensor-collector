#include "Blink.h"
#include "Arduino.h"

Blink::Blink(unsigned char pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Blink::on(int msec) {
  digitalWrite(_pin, HIGH);
  delay(msec);
}

void Blink::off(int msec) {
  digitalWrite(_pin, LOW);
  delay(msec);
}
