#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

typedef struct
{
    const char *id;
    const char *user;
    const char *password;
} MqttCredentials;

class Mqtt
{

public:
    Mqtt();
    void init(const char *id, const char *user, const char *password);
    virtual void reconnect();
    bool connected();
    void subscribe(const char *topic);
    void publish(const char *topic, const char *payload);
    void loop();

protected:
    void setServer(const char *domain, uint16_t port);
    void setBufferSize(uint16_t size);
    void setCallback(MQTT_CALLBACK_SIGNATURE);

    virtual void onConnected();

private:
    PubSubClient client;
    WiFiClientSecure wifiSecureClient;
    MqttCredentials credentials;
};

#endif