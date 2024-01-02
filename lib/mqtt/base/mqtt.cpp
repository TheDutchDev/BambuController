#include <base/mqtt.h>

Mqtt::Mqtt() : client(wifiSecureClient)
{
    wifiSecureClient.setInsecure();
}

void Mqtt::init(const char *id, const char *user, const char *password)
{
    credentials.id = id;
    credentials.user = user;
    credentials.password = password;
}

bool Mqtt::connected()
{
    return client.connected();
}

void Mqtt::loop()
{
    client.loop();
}

void Mqtt::reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection... ");
        // Attempt to connect
        if (client.connect(credentials.id, credentials.user, credentials.password))
        {
            Serial.println("connected!");
            onConnected();
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void Mqtt::onConnected() {}

void Mqtt::subscribe(const char *topic)
{
    client.subscribe(topic);
}

void Mqtt::publish(const char *topic, const char *payload)
{
    client.publish(topic, payload);
}

void Mqtt::setServer(const char *domain, uint16_t port)
{
    client.setServer(domain, port);
}

void Mqtt::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    client.setCallback(callback);
}

void Mqtt::setBufferSize(uint16_t size)
{
    client.setBufferSize(size);
}