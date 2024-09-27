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
#include "esp_log.h"
#include "i2c_task.h"
#include "bus_voltage.h"

static const char *TAG = "solar panel";



void app_main()
{

    /* initialize task */
    float voltageValue;

    init_i2cHandler();
    init_BusVoltage();
    


    while (1) 
    {
        /* get filtered voltage and current */
        voltageValue = get_filtered_voltage();

        /* print voltage and current */
        ESP_LOGI(TAG, "Voltage: %f, Current: %f\r\n", voltageValue, currentValue);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
