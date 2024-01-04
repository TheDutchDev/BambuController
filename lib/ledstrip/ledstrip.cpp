#include <ledstrip.h>

LedStrip::LedStrip(int redPin, int greenPin, int bluePin, int whitePin, int yellowPin)
    : _redPin(redPin),
      _greenPin(greenPin),
      _bluePin(bluePin),
      _whitePin(whitePin),
      _yellowPin(yellowPin)
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(whitePin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    _blinkMillis = millis();
}

void LedStrip::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t yellow, float brightness)
{
    _colors.red = toBrightness(red, brightness);
    _colors.green = toBrightness(green, brightness);
    _colors.blue = toBrightness(blue, brightness);
    _colors.white = toBrightness(white, brightness);
    _colors.yellow = toBrightness(yellow, brightness);

    updateLedStrip();
}

int LedStrip::getStep(int current, int target)
{
    if (target < current)
        return -1;

    if (target > current)
        return 1;

    return 0;
}

void LedStrip::transitionToColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t yellow, float brightness, int durationMs)
{
    red = toBrightness(red, brightness);
    green = toBrightness(green, brightness);
    blue = toBrightness(blue, brightness);
    white = toBrightness(white, brightness);
    yellow = toBrightness(yellow, brightness);

    float stepTime = (float)durationMs / 255.0f;

    int redStep = getStep(_colors.red, red);
    int greenStep = getStep(_colors.green, green);
    int blueStep = getStep(_colors.blue, blue);
    int whiteStep = getStep(_colors.white, white);
    int yellowStep = getStep(_colors.yellow, yellow);

    int maxRed = _colors.red < red ? red : _colors.red;
    int maxGreen = _colors.green < green ? green : _colors.green;
    int maxBlue = _colors.blue < blue ? blue : _colors.blue;
    int maxWhite = _colors.white < white ? white : _colors.white;
    int maxYellow = _colors.yellow < yellow ? yellow : _colors.yellow;

    _transitionMillis = millis();

    int i = 0;
    while (i < 256)
    {
        if (millis() - _transitionMillis < stepTime)
            continue;

        i++;
        _transitionMillis = millis();
        _colors.red = constrain(_colors.red + redStep, 0, maxRed);
        _colors.green = constrain(_colors.green + greenStep, 0, maxGreen);
        _colors.blue = constrain(_colors.blue + blueStep, 0, maxBlue);
        _colors.white = constrain(_colors.white + whiteStep, 0, maxWhite);
        _colors.yellow = constrain(_colors.yellow + yellowStep, 0, maxYellow);

        updateLedStrip();
    }
}

void LedStrip::forceToRed(uint8_t value, float brightness)
{
    setColor(value, 0, 0, 0, 0, brightness);
}

void LedStrip::forceToGreen(uint8_t value, float brightness)
{
    setColor(0, value, 0, 0, 0, brightness);
}

void LedStrip::forceToBlue(uint8_t value, float brightness)
{
    setColor(0, 0, value, 0, 0, brightness);
}

void LedStrip::forceToWhite(uint8_t value, float brightness)
{
    setColor(0, 0, 0, 0, value, brightness);
}

void LedStrip::forceToYellow(uint8_t value, float brightness)
{
    setColor(0, 0, 0, value, 0, brightness);
}

int LedStrip::toBrightness(int color, float brightness)
{
    return round(color * brightness);
}

void LedStrip::blink(int delayMs)
{
    if (millis() - _blinkMillis < delayMs)
        return;

    _blinkMillis = millis();
    transitionToColor(0, 0, (_colors.blue > 0 ? 0 : 255), 0, 0, .7, delayMs / 2);
}

void LedStrip::updateLedStrip()
{
    analogWrite(_redPin, _colors.red);
    analogWrite(_greenPin, _colors.green);
    analogWrite(_bluePin, _colors.blue);
    analogWrite(_whitePin, _colors.white);
    analogWrite(_yellowPin, _colors.yellow);
}

void LedStrip::onBambuPrinterData(DynamicJsonDocument jsonDoc)
{
}