#include "init.h"
#include <Arduino.h>


void Init( void (*core0)( void * parameter ), void (*core1)( void * parameter ) )
{

    TaskHandle_t Task1, Task2;
  
    xTaskCreatePinnedToCore(
        core0,
        "Core0",
        1500,
        NULL,
        1,
        &Task1,
        0);
    delay(500);  // needed to start-up task1

    xTaskCreatePinnedToCore(
        core1,
        "Core1",
        1000,
        NULL,
        1,
        &Task2,
        1);

}
