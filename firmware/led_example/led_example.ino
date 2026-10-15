#include "Blink.h"

Blink blink(D4);

void setup() {
  Serial.begin(115200);
  Serial.println("\nBlinkVersion 1.0 Junyeob_Kim");
}

void loop() {
  blink.on(500);
  blink.off(300);
}
