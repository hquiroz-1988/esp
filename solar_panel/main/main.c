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
#include "i2c_task.h"
#include "bus_voltage.h"


#define TEST_I2C_TASK
// #undef  TEST_I2C_TASK

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

        /* add test for i2c task here */
        #ifdef TEST_I2C_TASK
        int ret;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        // i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        #endif

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
