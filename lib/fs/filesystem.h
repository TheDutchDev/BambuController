#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "FS.h"
#include <config.h>
#include <defs.h>
#include <ArduinoJson.h>

class FileSystem
{
public:
    FileSystem(NetworkConfig *netConfig, PrinterConfig *printer, SystemConfig *sysConfig);
    bool save();
    bool load();
    bool mount();
    void erase();
    void update(JsonDocument json);

    void onFactoryReset(JsonDocument json);

private:
    void readJson(JsonDocument &jsonDoc);

    JsonDocument writeJson();

    const char *filePath;
    PrinterConfig *printerConfig;
    NetworkConfig *networkConfig;
    SystemConfig *systemConfig;
};

#endif