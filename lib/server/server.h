#ifndef ESPWEBSERVER_H
#define ESPWEBSERVER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "defs.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include "AsyncJson.h"
#include <config.h>
#include <ESPAsyncWebServer.h>
#include <eventemitter.h>
#include <functional>
#include "index.html.gz.h"
#include "favicon.ico.gz.h"

class EspWebServer : public EventEmitter<JsonDocument>
{
public:
    EspWebServer(uint16_t port, SystemConfig *sysConf);
    void begin();
    void loop();
    void broadcastWs(const char *payload);
    void onBambuPrinterData(JsonDocument jsonDoc);

private:
    void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void onApiInfo(AsyncWebServerRequest *request);
    void onUpdate(AsyncWebServerRequest *request, JsonVariant &json);
    void onNotFound(AsyncWebServerRequest *request);
    void onReboot(AsyncWebServerRequest *request);
    void onMainPage(AsyncWebServerRequest *request);
    void onOverrideLedColor(AsyncWebServerRequest *request, JsonVariant &json);
    void onFactoryReset(AsyncWebServerRequest *request);

    String getAllConfigAndStatusAsString();

    AsyncWebServer server;
    AsyncWebSocket ws;

    SystemConfig *systemConfig;

};

#endif