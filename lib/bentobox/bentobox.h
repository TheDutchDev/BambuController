#ifndef BENTOBOX_H
#define BENTOBOX_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PrinterStatus.h>

enum class EBentoBoxState
{
    Off,
    On
};

class BentoBox
{
public:
    BentoBox(PrinterStatus *printerStatus, int pinNumber, uint64_t turnOffDelayMs);
    void start();
    void stop();
    EBentoBoxState state();
    void onBambuPrinterData(JsonDocument jsonDoc);
    void stopIfTimeExpired();

private:
    int _pinNumber;
    uint64_t _lastRequiredAtMs;
    uint64_t _turnOffDelayMs;
    PrinterStatus *_printerStatus;
};

#endif