#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <printerstatus.h>

typedef struct
{
    int red;
    int green;
    int blue;
    int white;
    int yellow;
} LedStripColors;

class LedStrip
{
public:
    LedStrip(PrinterStatus *printerStatus, int redPin, int greenPin, int bluePin, int whitePin, int yellowPin);
    void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t yellow, float brightness);
    void transitionToColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t yellow, float brightness, int durationMs);

    void forceToRed(uint8_t value, float brightness);
    void forceToGreen(uint8_t value, float brightness);
    void forceToBlue(uint8_t value, float brightness);
    void forceToWhite(uint8_t value, float brightness);
    void forceToYellow(uint8_t value, float brightness);
    void blink(int delayMs);

    void onBambuPrinterData(JsonDocument jsonDoc);

private:
    int toBrightness(int color, float brightness);
    void updateLedStrip();
    int getStep(int current, int target);

    int _redPin;
    int _greenPin;
    int _bluePin;
    int _whitePin;
    int _yellowPin;

    uint _transitionMillis;
    uint _blinkMillis;

    LedStripColors _colors;
    PrinterStatus *_printerStatus;
};

#endif