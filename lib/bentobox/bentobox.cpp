#include <bentobox.h>

BentoBox::BentoBox(int pinNumber) : pinNumber(pinNumber)
{
    pinMode(pinNumber, OUTPUT);
}

void BentoBox::start()
{
    digitalWrite(pinNumber, HIGH);
}

void BentoBox::stop()
{
    digitalWrite(pinNumber, LOW);
}

EBentoBoxState BentoBox::state()
{
    int pinState = digitalRead(pinNumber);
    return pinState == HIGH ? EBentoBoxState::On : EBentoBoxState::Off;
}

void BentoBox::onBambuPrinterData(DynamicJsonDocument jsonDoc)
{
    if (state() == EBentoBoxState::Off)
        return;
}