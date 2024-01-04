#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <LittleFS.h>
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
    void update(DynamicJsonDocument json);

    void onFactoryReset(DynamicJsonDocument json);

private:
    void readJson(DynamicJsonDocument &jsonDoc);

    DynamicJsonDocument writeJson();

    const char *filePath;
    PrinterConfig *printerConfig;
    NetworkConfig *networkConfig;
    SystemConfig *systemConfig;
};

#endif