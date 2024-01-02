#include <led.h>

Led::Led(int ledPin) : ledPin(ledPin)
{
    pinMode(ledPin, OUTPUT);
}

void Led::trigger(bool toggle)
{
    digitalWrite(ledPin, toggle ? HIGH : LOW);
}