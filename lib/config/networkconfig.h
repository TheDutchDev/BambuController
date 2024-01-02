#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

class NetworkConfig
{
public:
    char ssid[32];
    char password[32];
    char hostName[32];

    void toJson(DynamicJsonDocument &json)
    {
        json["network"]["config"]["ssid"] = ssid;
        json["network"]["config"]["password"] = password;
        json["network"]["config"]["hostName"] = hostName;
    }

    void fromJson(DynamicJsonDocument json)
    {
        if (!json.containsKey("network") || !json["network"].containsKey("config"))
            return;

        writeJsonToDest(json, "ssid", ssid);
        writeJsonToDest(json, "password", password);
        writeJsonToDest(json, "hostName", hostName);
    }

    void resetToDefaults()
    {
        strcpy(ssid, "");
        strcpy(password, "");
        strcpy(hostName, "bambu-controller");
    }

private:
    void writeJsonToDest(DynamicJsonDocument &json, const char *key, char *destination)
    {
        if (!json["network"]["config"].containsKey(key))
            return;

        strcpy(destination, json["network"]["config"][key]);
    }
};

#endif