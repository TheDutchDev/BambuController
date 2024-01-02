#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <ArduinoJson.h>

enum class EState
{
    Setup,
    Active
};

enum class EMode
{
    Normal,
    Diagnose
};

class SystemConfig
{
public:
    EState state;
    EMode mode;

    void toJson(DynamicJsonDocument &json)
    {
        json["system"]["config"]["state"] = (int)state;
        json["system"]["config"]["mode"] = (int)mode;
    }

    void fromJson(DynamicJsonDocument json)
    {
        if (!json.containsKey("system") || !json["system"].containsKey("config"))
            return;

        if (json["system"]["config"].containsKey("state"))
            state = static_cast<EState>((int)json["system"]["config"]["state"]);

        if (json["system"]["config"].containsKey("mode"))
            mode = static_cast<EMode>((int)json["system"]["config"]["mode"]);
    }

    void resetToDefaults()
    {
        state = EState::Setup;
        mode = EMode::Normal;
    }
};

#endif