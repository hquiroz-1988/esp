/* adc example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/i2c.h"
#include "esp_log.h"


#include "gpio.hpp"
#include "i2c_bus.hpp"
#include "ads1115.hpp"
#include "bus_voltage.hpp"


/* static variables    */
// static const char *TAG = "main";


extern "C" void app_main()
{

    // NetworkingModule networkingModule;

    /*  initialize bus voltage module   */
    Gpio sdaPin(GPIO_NUM_4, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP_ENABLE);
    Gpio sclPin(GPIO_NUM_5, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP_ENABLE);
    I2CBus i2cBus(sdaPin, sclPin, I2C_NUM_0);
    Gpio ads1115AlertPin(GPIO_NUM_6, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE);
    // ADS1115 ads1115(ads1115AlertPin);
    

    // BusVoltage busVoltage(ads1115);

    // /* initialize bus current module */
    // INA219 ina219;
    // BusCurrent busCurrent(ina219);

    // PowerMonitor pm(networkingModule, busVoltage, busCurrent);

    while (1) 
    {   


        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
