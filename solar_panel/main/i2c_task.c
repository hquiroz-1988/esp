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
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c_task.h"
#include "i2c_handler.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define QUEUE_LENGTH    (10u)
#define ITEM_SIZE       (sizeof(i2c_handler_t *))
#define QUEUE_TIMEOUT   (2000)

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static const char *TAG = "solar panel";

/*
    cmddObj holds a task handle of the task that sent the command
    and the i2c command descriptor
*/
static i2c_handler_t cmdObj;

/*
    The variable used to hold the queue's data structure
*/
static StaticQueue_t i2cCmdQueue;

/*
    Queue Storage Variable
*/
static uint8_t queueStorage[QUEUE_LENGTH*ITEM_SIZE];

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/** @brief  I2C task that runs periodically
 * 
 * Will pop queue values 
 * 
 *  @param void 
 *  @return void 
 */
void i2c_Task(void);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void init_i2cHandler(void)
{
    /*  create i2c task */
    xTaskCreate(i2c_Task, "i2c_task", 1024, NULL, 5, NULL);

    /* create or initialize i2c object queue */
    
}

void i2c_Task(void)
{
    /*  create queue handle */
    QueueHandle_t queueHandle;

    /* queue to queue pointers to i2c command objects   */
    queueHandle = xQueueCreateStatic(   QUEUE_LENGTH, 
                                        ITEM_SIZE,
                                        queueStorage,
                                        &i2cCmdQueue );

    /* i2c object pointer to receive queueu value   */
    i2c_handler_t * i2cObjPtr = NULL;

    /* queue receive return value */
    BaseType_t retVal = pdFALSE;

    /* while i2c data exists send out data  */
    ESP_LOGI(TAG, "init i2c task\r\n");

    while (1) 
    {

        /* receive command from queue, this should block until receive or timeout   */
        retVal = xQueueReceive( queueHandle,
                                &i2cObjPtr,
                                QUEUE_TIMEOUT);

        /* if received data, process data*/
        if(pdPASS == retVal)
        {
            /* objptr now has pointer to i2c obj perform i2c cmd*/


            /* once queue element is addressed , notify sending task    */
        }
        else
        {
            /* throw some error  */
            ESP_LOGI(TAG, "i2c queue timeout\r\n");
        }
    }
}