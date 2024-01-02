#include <factorydefaults.h>

FactoryDefaults::FactoryDefaults(PrinterStatus *pStatus, NetworkConfig *netConf, PrinterConfig *pConf, SystemConfig *sysConf)
    : printerStatus(pStatus),
      networkConfig(netConf),
      printerConfig(pConf),
      systemConfig(sysConf)
{
}

void FactoryDefaults::reload()
{
    Serial.println("Resetting to factory defaults...");

    systemConfig->resetToDefaults();
    printerConfig->resetToDefaults();
    printerStatus->resetToDefaults();
    networkConfig->resetToDefaults();

    Serial.println("Factory defaults applied!");
}