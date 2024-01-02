#ifndef FACTORYDEFAULTS_H
#define FACTORYDEFAULTS_H

#include <Arduino.h>
#include <config.h>

class FactoryDefaults
{
public:
    FactoryDefaults(PrinterStatus *pStatus, NetworkConfig *netConf, PrinterConfig *pConf, SystemConfig *sysConf);
    void reload();

private:
    PrinterStatus *printerStatus;
    PrinterConfig *printerConfig;
    NetworkConfig *networkConfig;
    SystemConfig *systemConfig;
};

#endif
