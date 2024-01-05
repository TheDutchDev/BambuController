#ifndef GLOBALS_H
#define GLOBALS_H

///////////////
// Includes
///////////////
#include <defs.h>
#include <config.h>
#include <filesystem.h>
#include <network.h>
#include <server.h>
#include <bambu.h>
#include <led.h>
#include <ledstrip.h>
#include <bentobox.h>
#include <factorydefaults.h>

Network *network = new Network();
FileSystem *fileSystem = new FileSystem(networkConfig, printerConfig, systemConfig);
EspWebServer *server = new EspWebServer(80, systemConfig);
Bambu *bambu = new Bambu(printerConfig, printerStatus);
FactoryDefaults *factoryDefaults = new FactoryDefaults(printerStatus, networkConfig, printerConfig, systemConfig);

BentoBox *bentoBox;
LedStrip *ledStrip;

Led *ceilingLed;

#endif // GLOBALS_H
