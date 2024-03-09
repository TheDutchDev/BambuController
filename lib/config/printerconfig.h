#ifndef PRINTERCONFIG_H
#define PRINTERCONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <cstring>

class PrinterConfig
{
public:
    char ipAddress[16];
    char accessCode[9];
    char serialNumber[16];

    void toJson(JsonDocument &json, bool includeSensitiveInfo = false)
    {
        json["printer"]["config"]["ipAddress"] = ipAddress;

        if (includeSensitiveInfo)
        {
            json["printer"]["config"]["accessCode"] = accessCode;
            json["printer"]["config"]["serialNumber"] = serialNumber;
        }
    }

    void fromJson(JsonDocument json)
    {
        if (!json.containsKey("printer") || !json["printer"].containsKey("config"))
            return;

        writeJsonToDest(json, "ipAddress", ipAddress);
        writeJsonToDest(json, "accessCode", accessCode);
        writeJsonToDest(json, "serialNumber", serialNumber);
    }

    void resetToDefaults()
    {
        strcpy(ipAddress, "");
        strcpy(accessCode, "");
        strcpy(serialNumber, "");
    }

    bool isConfigured()
    {
        return std::strcmp(ipAddress, "") != 0 && std::strcmp(accessCode, "") != 0 && std::strcmp(serialNumber, "") != 0;
    }

private:
    void writeJsonToDest(JsonDocument &json, const char *key, char *destination)
    {
        if (!json["printer"]["config"].containsKey(key))
            return;

        strcpy(destination, json["printer"]["config"][key]);
    }
};

#endif