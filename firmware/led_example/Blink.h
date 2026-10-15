#ifndef Blink_h
#define Blink_h
#include "Arduino.h"

class Blink {
  public:
    Blink(unsigned char pin);
    void on(int msec);
    void off(int msec);

  private:
    unsigned char _pin;
};

#endif
