#include "ota.h"
#include <namespaces.h>

Ota::Ota()
    : _fota(HARDWARE_VERSION, FIRMWARE_VERSION, true),
      _rootCA(new CryptoMemAsset("Root CA", rootCA, strlen(rootCA) + 1)),
      _doUpdate(false),
      _lastMessageAtMs(0)
{
}

void Ota::setup()
{
    _fota.setManifestURL("https://raw.githubusercontent.com/thedutchdev/bambu-controller/master/firmware.json");
    _fota.setRootCA(_rootCA);

    // CryptoMemAsset *MyPubKey = new CryptoMemAsset("RSA Key", public_key, strlen(public_key) + 1);
    // _fota.setPubKey(MyPubKey);

    _fota.setUpdateBeginFailCb(std::bind(&Ota::updateBeginFailCallback, this, _1));
    _fota.setProgressCb(std::bind(&Ota::progressCallback, this, _1, _2));

    _fota.setUpdateEndCb(std::bind(&Ota::updateEndCallback, this, _1));
    _fota.setUpdateCheckFailCb(std::bind(&Ota::updateCheckFailCallback, this, _1, _2));

    _fota.printConfig();
}

void Ota::updateBeginFailCallback(int partition)
{
    Serial.printf("[ota] Update could not begin with %s partition\n", partition == U_SPIFFS ? "spiffs" : "firmware");
}

void Ota::progressCallback(size_t progress, size_t size)
{
    if (progress == size || progress == 0)
        Serial.println();

    Serial.print(".");

    // let the clients know every second and at the end
    if (millis() - _lastMessageAtMs > 1000 || progress == size)
    {
        float percent = (float)progress / (float)size * 100.0;
        sendProgressUpdate(percent);
        _lastMessageAtMs = millis();
    }
}

void Ota::updateEndCallback(int partition)
{
    Serial.printf("[ota] Update ended with %s partition\n", partition == U_SPIFFS ? "spiffs" : "firmware");
    sendProgressFinished();
}

void Ota::updateCheckFailCallback(int partition, int error_code)
{
    // error codes:
    //  -1 : partition not found
    //  -2 : validation (signature check) failed
    Serial.printf("[ota] Update could not validate %s partition (error %d)\n", partition == U_SPIFFS ? "spiffs" : "firmware", error_code);
}

void Ota::sendProgressUpdate(float progressPct)
{
    JsonDocument json;
    json["msg"].set("ota_progress");
    json["progress"].set(round(progressPct));

    String jsonString;
    serializeJson(json, jsonString);
    // emit ws
}

void Ota::sendProgressFinished()
{
    JsonDocument json;
    json["msg"] = "ota_finished";

    String jsonString;
    serializeJson(json, jsonString);
    // emit ws
}

void Ota::loop()
{
    if (!_doUpdate)
        return;

    _fota.handle();
    _doUpdate = false;
}