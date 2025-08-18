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
#include "ads1115.hpp"
#include "bus_voltage.hpp"


/* static variables    */
static const char *TAG = "main";



#define TEST_I2C_TASK
#undef  TEST_I2C_TASK
#define TEST_ADS1115_TASK
// #undef TEST_ADS1115_TASK


#ifdef TEST_I2C_TASK

#define WRITE_BIT                           I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                            I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                        0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                       0x0              /*!< I2C master will not check ack from slave */
static void testI2CTask(void);

static void testI2CTask(void)
{
    // int ret;
    /* create i2c object pointer    */
    i2c_handler_t i2cObj;
    i2c_handler_t * i2cObjPtr = &i2cObj;
    i2cObj.cmd = i2c_cmd_link_create();
    i2cObj.taskHdl = xTaskGetCurrentTaskHandle();

    i2c_master_start(i2cObj.cmd);
    i2c_master_write_byte(i2cObj.cmd, 0xAA, ACK_CHECK_DIS);
    i2c_master_stop(i2cObj.cmd);
    /* send to queue*/
    if( i2cQueueHdl != NULL )
    {
        /* 
            Send an unsigned long. Wait for 10 ticks for space to become
            available if necessary.
        */
        ESP_LOGI(TAG, "i2cObjPtr = %i\r\n", (uint32_t)i2cObjPtr);
        if( xQueueSendToBack( i2cQueueHdl,
                            ( void * ) &i2cObjPtr,
                            ( TickType_t ) 10 ) != pdPASS )
        {
            /* Failed to post the message, even after 10 ticks. */
            ESP_LOGI(TAG, "failed to send command\r\n");
        }
        else
        {
            ESP_LOGI(TAG, "sent command\r\n");
            ulTaskNotifyTake(pdTRUE, ( TickType_t ) 1000);
            ESP_LOGI(TAG, "command complete\r\n");
        }
    }
    else 
    {
        ESP_LOGI(TAG, "queue hdl null\r\n");
    }
    i2c_cmd_link_delete(i2cObj.cmd);
}
#endif //TEST_I2C_TASK


#ifdef TEST_ADS1115_TASK
/* static function prototypes    */
static void testAds1115Task(void);

static void testAds1115Task(void)
{
    ads1115ConfigRegister_t configRegister;

    ADS1115 ads1115;

    if(STATUS_OKAY != ads1115.getConfiguration(&configRegister))
    {
        /* some failure occured     */
        ESP_LOGI(TAG, "Could not get configuration");
    }

    /* print out current configuration   */


    /* change configuration  */
    
    /* no conversion needed at the moment    */
    configRegister.opStatus = ADS1115_MUX_AIN0_AIN1;
    

    /*  set mux to use AIN0 and AIN1 as diferential pair        */
    configRegister.mux = ADS1115_MUX_AIN0_AIN1;


    /* read out configuration    */
    

}
#endif //TEST_ADS1115_TASK


extern "C" void app_main()
{

    /* initialize task */
    // float voltageValue;
    // Status_t errRet;

    /* initialize i2c handler task and i2c module    */
    init_i2cHandler();

    /*  initialize bus voltage module   */
    BusVoltage busVoltage;
    
    while (1) 
    {
        // /* get filtered voltage and current */
        // errRet = get_filtered_voltage(&voltageValue);

        // if(STATUS_OKAY == errRet)
        // {
        //     /* print voltage and current */
        //     ESP_LOGI(TAG, "Voltage: %f, Current: %f\r\n", voltageValue, currentValue);
        // }
        // else
        // {
        //     /* print err */
        //     ESP_LOGI(TAG, "Err Value: %i\r\n", errRet);
        // }
        

        /* add test for i2c task here */
        #ifdef TEST_I2C_TASK
        testI2CTask();
        #endif

        /* add test for i2c task here */
        #ifdef TEST_ADS1115_TASK
        testAds1115Task();
        #endif

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
