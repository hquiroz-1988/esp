/**
 ********************************************************************************
 * @file    i2c_task.c
 * @author  Hugo Quiroz
 * @date    2024-09-06 15:40:43
 * @brief   This module handles the i2c management using an RTOS task.
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "i2c_task.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define QUEUE_LENGTH                (10u)
#define ITEM_SIZE                   (sizeof(i2c_handler_t *))
#define QUEUE_TIMEOUT               (2000)
#define I2C_CHANNEL_NUM              I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_SCL_IO           5                /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO           4               /*!< gpio number for I2C master data  */

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static const char *TAG = "i2c task";

/*  create queue handle */
QueueHandle_t i2cQueueHdl;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * 
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/** @brief  I2C task that runs periodically
 * 
 * Will pop queue values 
 * 
 *  @param void 
 *  @return void 
 */
static void i2c_Task(void *arg);

/************************************
 * STATIC FUNCTIONS
 ************************************/
/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_example_master_init()
{
    int i2c_master_port = I2C_CHANNEL_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = 1;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = 1;
    conf.clk_stretch_tick = 300; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
    ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
    ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
    return ESP_OK;
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void init_i2cHandler(void)
{

    i2c_example_master_init();

    /* queue to queue pointers to i2c command objects   */
    i2cQueueHdl = xQueueCreate( QUEUE_LENGTH, 
                                ITEM_SIZE );

    /*  create i2c task */
    xTaskCreate(i2c_Task, "i2c_task", 1024, NULL, 5, NULL);
}


static void i2c_Task(void *arg)
{
    
    /* i2c object pointer to receive queue value   */
    i2c_handler_t * i2cObjPtr = NULL;

    /* queue receive return value */
    BaseType_t retVal = pdFALSE;

    /* esp error type   */
    esp_err_t errRet = ESP_FAIL;

    /* while i2c data exists send out data  */
    ESP_LOGI(TAG, "init i2c task\r\n");

    while (1) 
    {
        //!TODO: check for queue null pointers  


        /* receive command from queue, this should block until receive or timeout   */
        retVal = xQueueReceive( i2cQueueHdl,
                                &i2cObjPtr,
                                QUEUE_TIMEOUT);

        ESP_LOGI(TAG, "got data from queue\r\n");
        /* if received data, process data*/
        if(pdPASS == retVal)
        {
            ESP_LOGI(TAG, "i2cObjPtr: %i\r\n", (uint32_t)i2cObjPtr);

            if( i2cObjPtr != NULL && 
                i2cObjPtr->cmd != NULL &&
                i2cObjPtr->taskHdl != NULL)
            {
                /* objptr now has pointer begin i2c command */
                errRet = i2c_master_cmd_begin(I2C_CHANNEL_NUM, i2cObjPtr->cmd, 1000 / portTICK_RATE_MS);

                if(errRet == ESP_FAIL)
                {
                    /* throw error   */
                    ESP_LOGI(TAG, "i2c command failed\r\n");
                }

                /* once queue element is addressed , notify sending task    */
                xTaskNotifyGive(i2cObjPtr->taskHdl);
            }
            else
            {
                /* throw error   */
                ESP_LOGI(TAG, "i2c object pointer invalid\r\n");
            }
        }
        else
        {
            /* throw some error  */
            ESP_LOGI(TAG, "i2c queue timeout\r\n");
        }
    }
}