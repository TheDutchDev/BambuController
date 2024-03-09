#include <bentobox.h>

BentoBox::BentoBox(PrinterStatus *printerStatus, int pinNumber, uint64_t turnOffDelayMs)
    : _printerStatus(printerStatus),
      _pinNumber(pinNumber),
      _turnOffDelayMs(turnOffDelayMs),
      _lastRequiredAtMs(0)
{
    pinMode(_pinNumber, OUTPUT);
}

void BentoBox::start()
{
    digitalWrite(_pinNumber, HIGH);
    _lastRequiredAtMs = 0;
}

void BentoBox::stop()
{
    digitalWrite(_pinNumber, LOW);
}

EBentoBoxState BentoBox::state()
{
    int pinState = digitalRead(_pinNumber);
    return pinState == HIGH ? EBentoBoxState::On : EBentoBoxState::Off;
}

void BentoBox::stopIfTimeExpired()
{
    if (_lastRequiredAtMs == 0)
        return;

    if (millis() - _lastRequiredAtMs < _turnOffDelayMs)
        return;

    _lastRequiredAtMs = 0;
    stop();
}

void BentoBox::onBambuPrinterData(JsonDocument json)
{
    // Only turn on bento box for non-PLA filament types
    if (_printerStatus->filamentStatus->type == EFilamentType::PLA)
    {
        if (state() == EBentoBoxState::Off)
            return;

        if (_lastRequiredAtMs != 0)
            return;

        _lastRequiredAtMs = millis();
    }
    else
    {
        if (state() == EBentoBoxState::On)
            return;

        _lastRequiredAtMs = 0;
        start();
    }
}