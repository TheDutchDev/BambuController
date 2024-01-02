#include <Arduino.h>
#include <init.h>
#include <globals.h>
#include <functional>

using std::placeholders::_1;

void Core0(void *parameter)
{
    for (;;)
    {
        server->loop();
        delay(50);
        // vTaskDelay(10);
    }
}
void Core1(void *parameter)
{
    for (;;)
    {
        delay(50);
        // Serial.println( "core1" );
    }
}

void setup()
{
    Serial.begin(115200);

    if (!fileSystem->mount())
    {
        Serial.println("[FATAL] Failed to mount filesystem!");
        ESP.restart();
        return;
    }

    // fileSystem->erase();

    if (!fileSystem->load())
    {
        network->setupAccessPoint("Bambu Controller", "bambu123");
        factoryDefaults->reload();
    }
    else
    {
        systemConfig->state = EState::Active;
        network->connect(networkConfig->ssid, networkConfig->password, networkConfig->hostName);

        ceilingLed = new Led(CEILING_LED_PIN);
        ceilingLed->trigger(true);

        bambu->setup();
        bambu->on("data", std::bind(&EspWebServer::onBambuPrinterData, server, _1));

        fileSystem->save();
    }

    server->on("update", std::bind(&FileSystem::update, fileSystem, _1));

    server->begin();
    Init(Core0, Core1);
}

void runSetupLoop()
{
    Serial.print("state: ");
    Serial.print(systemConfig->state == EState::Setup ? "Setup" : "Active");

    Serial.print(", IP Address: ");
    Serial.print(WiFi.softAPIP());
    Serial.print(", hostname: ");
    Serial.println(WiFi.softAPgetHostname());

    network->loop();
}

void runActiveLoop()
{
    if (!bambu->connected())
    {
        Serial.println("reconnecting bambu");
        bambu->reconnect();
    }

    // delay(1000);
    bambu->loop();
}

void loop()
{
    switch (systemConfig->state)
    {
    case EState::Setup:
        runSetupLoop();
        break;

    case EState::Active:
        runActiveLoop();
        break;
    }
}