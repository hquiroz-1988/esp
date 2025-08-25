/*
 * Thread.cpp
 *
 *  Created on: Mar 12, 2021
 *      Author: cornelius
 */

#include "task.hpp"

extern "C" 
{
    #include "esp_log.h"
}

const char *TAG = "Task";


Task::Task(const char *name, uint32_t _stackSize, UBaseType_t prio) :
    taskName(name), stackSize(_stackSize) 
{
    priority = prio;
    taskHandle = nullptr;
    suspended = false;
}

Task::~Task() 
{
    if (xPortInIsrContext()) 
    {
        ESP_LOGE(TAG, "Error: %i", STATUS_ISR_ERROR);
    }
    else if (CHECK_POINTER_VALID(taskHandle) == false)
    {
        /* task is nullptr */
        ESP_LOGE(TAG, "Error: %i", STATUS_NULL_POINTER);
    }
    else 
    {
        eTaskState tstate = eTaskGetState (taskHandle);

        if (tstate != eDeleted) 
        {
            vTaskDelete (taskHandle);
        }
        else 
        {
            ESP_LOGE(TAG, "Error: %i", STATUS_OS_ERROR);
        }
    }
}

Status_t Task::initTask(void)
{
    Status_t ret = STATUS_OKAY;

    TaskFunction_t taskFunction = (TaskFunction_t)Task::startTask;

    if (CHECK_POINTER_VALID(taskHandle)) 
    {
        /* if task handle is not null pointer, dont init again   */
        ret = STATUS_REINIT_ERROR;
    }

    if( (ret == STATUS_OKAY) 
        && (stackSize < configMINIMAL_STACK_SIZE) )
    {
        ret = STATUS_STACK_SIZE_TOO_SMALL;
    }

    if( (ret == STATUS_OKAY) 
        && (priority > configMAX_PRIORITIES))
    {
        ret = STATUS_OUT_OF_BOUNDS;
    }

    if( (ret == STATUS_OKAY) )
    {
        if(!xPortInIsrContext())
        {
            if (xTaskCreate ((TaskFunction_t)taskFunction, taskName, (uint16_t)stackSize, (void *)this, priority, &taskHandle) != pdPASS) 
            {
                taskHandle = nullptr;
                ret = STATUS_OS_ERROR;
            }
        }
        else
        {
            ret = STATUS_ISR_ERROR;
        }
        
    }

    return ret;
}

void Task::runInCurrent() { taskRun(); }

void Task::startTask(void *argument) { ((Task *)argument)->taskRun(); }

Status_t Task::suspend(void)
{
    Status_t ret = STATUS_OKAY;

    if(CHECK_POINTER_VALID(taskHandle) == false)
    {
        ret = STATUS_NULL_POINTER;
    }

    if( (ret == STATUS_OKAY) 
        && (!suspended) )
    {
        if (xPortInIsrContext()) 
        {
            ret = STATUS_ISR_ERROR;
        }
        else 
        {
            vTaskSuspend (taskHandle);
            suspended = true;
        }
    }

    return ret;
}

Status_t Task::resume() 
{
    Status_t ret = STATUS_OKAY;

    if(CHECK_POINTER_VALID(taskHandle) == false)
    {
        ret = STATUS_NULL_POINTER;
    }

    if( (ret == STATUS_OKAY) 
        && suspended )
    {
        if (xPortInIsrContext()) 
        {
            ret = STATUS_ISR_ERROR;
        }
        else 
        {
            vTaskResume (taskHandle);
            suspended = false;
        }
    }

    return ret;
}
