#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <systemconfig.h>
#include <printerconfig.h>
#include <printerstatus.h>
#include <networkconfig.h>

/* create an instance of Preferences library */
extern PrinterStatus *printerStatus;
extern NetworkConfig *networkConfig;
extern PrinterConfig *printerConfig;
extern SystemConfig *systemConfig;

#endif