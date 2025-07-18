/*
 * Thread.cpp
 *
 *  Created on: Mar 12, 2021
 *      Author: cornelius
 */

#include "Task.hpp"

extern "C" 
{

}


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
        // stat = osErrorISR;
    }
    else if (taskHandle == NULL) 
    {
        // stat = osErrorParameter;
    }
    else 
    {
        eTaskState tstate = eTaskGetState (taskHandle);

        if (tstate != eDeleted) 
        {
            // stat = osOK;
            vTaskDelete (taskHandle);
        }
        else 
        {
            // stat = osErrorResource;
        }
    }
}

retVal_t Task::initTask(void)
{
    retVal_t ret = ERR_NONE;

    TaskFunction_t taskFunction = (TaskFunction_t)Task::startTask;

    if (taskHandle != nullptr) 
    {
        ret = ERR_FAIL;
    }

    if( (ret == ERR_NONE) 
        && (stackSize < configMINIMAL_STACK_SIZE) )
    {
        ret = ERR_STACK_SIZE_TOO_SMALL;
    }

    if( (ret == ERR_NONE) 
        && (priority > configMAX_PRIORITIES))
    {
        ret = ERR_OUT_OF_BOUNDS;
    }

    if( (ret == ERR_NONE) )
    {
        if(!xPortInIsrContext())
        {
            if (xTaskCreate ((TaskFunction_t)taskFunction, taskName, (uint16_t)stackSize, (void *)this, priority, &taskHandle) != pdPASS) 
            {
                taskHandle = nullptr;
                ret = ERR_OS_FAIL;
            }
        }
        else
        {
            ret = ERR_IN_ISR;
        }
        
    }

    return ret;
}

void Task::runInCurrent() { taskRun(); }

void Task::startTask(void *argument) { ((Task *)argument)->taskRun(); }

void Task::suspend() {

    if (xPortInIsrContext()) 
    {
        // stat = osErrorISR;
    }
    else if (taskHandle == NULL) 
    {
        // stat = osErrorParameter;
    }
    else 
    {
        // stat = osOK;
        vTaskSuspend (taskHandle);
        suspended = true;
    }
}

void Task::resume() {
    if (taskHandle && suspended) {
        if (xPortInIsrContext()) 
        {
            // stat = osErrorISR;
        }
        else if (taskHandle == NULL) 
        {
            // stat = osErrorParameter;
        }
        else 
        {
            // stat = osOK;
            vTaskResume (taskHandle);
        }
        suspended = false;
    }
}
