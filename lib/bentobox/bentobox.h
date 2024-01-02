#ifndef BENTOBOX_H
#define BENTOBOX_H

#include <Arduino.h>
#include <ArduinoJson.h>

enum class EBentoBoxState
{
    Off,
    On
};

class BentoBox
{
public:
    BentoBox(int pinNumber);
    void start();
    void stop();
    EBentoBoxState state();
    void onBambuPrinterData(DynamicJsonDocument jsonDoc);

private:
    int pinNumber;
};

#endif