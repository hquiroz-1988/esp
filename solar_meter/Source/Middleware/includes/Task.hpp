/*
 * Task.h
 *
 *  Created on: Mar 12, 2021
 *      Author: cornelius
 */

#ifndef SRC_EVDRIVERS_TASK_H_
#define SRC_EVDRIVERS_TASK_H_

#include "common.h"
#include "freertos/FreeRTOS.h"

extern "C" {
    #include "freertos/task.h"
    // #include "cmsis_os2.h"  // ::CMSIS:RTOS2
}


class Task {
public:
    Task(const char *name, uint32_t _stackSize,
         UBaseType_t prio = ESP_NORMAL_PRIORITY);
    virtual ~Task();

    // start internal loop (main function) in new task and return
    Status_t initTask(void);

    // start internal loop (main function) in current task
    // (should not return)
    void runInCurrent(void);

    // suspend/resume only make sense if a new thread was started with run().
    // Ignored if runInCurrent() was used
    Status_t suspend(void);
    Status_t resume(void);

private:
    TaskHandle_t taskHandle;
    static void startTask(void *argument);
    virtual void taskRun() = 0;
    const char *taskName;
    uint32_t stackSize;
    UBaseType_t priority;
    volatile bool suspended;
};


#endif // SRC_EVDRIVERS_TASK_H_
