#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led
{
public:
    Led(int ledPin);
    void trigger(bool toggle);

private:
    int ledPin;
};

#endif