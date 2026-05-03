/**
 *******************************************************************************
 * @file    power_monitor.cpp
 * @author  hq
 * @date    2025-07-16 12:50:37
 * @brief   Power monitor source file
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
 *******************************************************************************/
#include "power_monitor.hpp"

extern "C"
{
    #include "esp_log.h"
}

/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS
 *******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/
static const char *TAG = "PowerMonitor";
/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTIONS
 *******************************************************************************/
Status_t PowerMonitor::startAndWaitForVoltage(float & value)
{
    Status_t status = busVoltage.startConversion();
    uint32_t notifyBit = static_cast<uint32_t>(NotifyBits::GET_VOLTAGE);

    /* after starting conversion, set the callback to call our static wrapper */
    if (status == STATUS_OKAY)
    {
        status = busVoltage.setCallback(PowerMonitor::staticWrapper, this, notifyBit);
    }

    /* start voltage conversion  */
    if (    status == STATUS_OKAY
         && xTaskNotifyWait(0, static_cast<uint32_t>(NotifyBits::CLEAR_ALL_BITS), &notificationValue, portMAX_DELAY) != pdTRUE)
    {
        status = STATUS_OS_ERROR;
    }

    /* make sure we have the correct notification value */
    if (    status == STATUS_OKAY 
            && (notificationValue & notifyBit) )
    {
        status = busVoltage.getConversion(latestBusVoltage);
        //!TODO: remove this log, only for testing
        if (status == STATUS_OKAY)
        {
            ESP_LOGI(TAG, "Voltage conversion complete: %d mV", static_cast<int>(latestBusVoltage * 1000.0f));
        }
    }

    /* whether or not we received the correct notification, clear the callback */
    busVoltage.clearCallback();

    return status;
}

Status_t PowerMonitor::startAndWaitForCurrent(float & value)
{
    Status_t status = STATUS_OKAY;

    status = busCurrent.getCurrentAndBusVoltage(latestBusCurrent, latestBusVoltage);

    if (status == STATUS_OKAY)
    {
        ESP_LOGI(TAG, "Current conversion complete: %d mA", static_cast<int>(latestBusCurrent * 1000.0f));
    }

    return status;
}




Status_t PowerMonitor::queueBusVoltageMessage()
{

    Status_t status = startAndWaitForVoltage(latestBatteryBusVoltage);

    if (status == STATUS_OKAY)
    {
        busVoltageMessage.name = "BusVoltage";
        busVoltageMessage.timestamp = xTaskGetTickCount();
        busVoltageMessage.size = sizeof(float);
        busVoltageMessage.dataPtr = &latestBatteryBusVoltage;
        status = networkingModule.queueNetworkingMessage(&busVoltageMessage);
    }

    return status;
}

Status_t PowerMonitor::queueBusCurrentMessage()
{
    Status_t status = busCurrent.getCurrentAndBusVoltage(latestPanelBusCurrent, latestPanelBusVoltage);

    if (status == STATUS_OKAY)
    {
        busCurrentMessage.name = "BusCurrent";
        busCurrentMessage.timestamp = xTaskGetTickCount();
        busCurrentMessage.size = sizeof(float);
        busCurrentMessage.dataPtr = &latestPanelBusCurrent;
        status = networkingModule.queueNetworkingMessage(&busCurrentMessage);
    }

    return status;
}

Status_t PowerMonitor::queuePowerMessage()
{
    latestPower = latestPanelBusVoltage * latestPanelBusCurrent;

    powerMessage.name = "Power";
    powerMessage.timestamp = xTaskGetTickCount();
    powerMessage.size = sizeof(float);
    powerMessage.dataPtr = &latestPower;
    
    return networkingModule.queueNetworkingMessage(&powerMessage);
}



/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/
PowerMonitor::PowerMonitor(NetworkingModule &_networkingModule,
                           BusVoltage &_busVoltage,
                           BusCurrent &_busCurrent) : Task("PowerMonitor", 256 * 4),
                                                      busVoltage(_busVoltage),
                                                      busCurrent(_busCurrent),
                                                      networkingModule(_networkingModule)
{
    /*
        initializing task here, its best to call here since this
        should be the last constructor called in the construction
        order, this or even after construction is when init task
        should be called.
    */
    initTask();
}

PowerMonitor::~PowerMonitor()
{
    // Destructor implementation
}



void PowerMonitor::taskRun()
{
    /*  initialize power monitor variables     */

    while (FOREVER())
    {
        Status_t status = STATUS_OKAY;
        

        if (xTaskNotifyWait(0, static_cast<uint32_t>(NotifyBits::CLEAR_ALL_BITS), &notificationValue, portMAX_DELAY) != pdTRUE)
        {
            status = STATUS_OS_ERROR;
        }

        /*   send the voltage, current, and power packets to telemetry module */
        if (    status == STATUS_OKAY 
                && notificationValue & static_cast<uint32_t>(NotifyBits::GET_POWER))
        {
            status = queueBusVoltageMessage();

            if(status == STATUS_OKAY)
            {
                status = queueBusCurrentMessage();
            }

            if(status == STATUS_OKAY)
            {
                status = queuePowerMessage();
            }
        }

        if (status != STATUS_OKAY)
        {
            ESP_LOGE(TAG, "Error: %i", status);
        }

        // Add a delay or yield to avoid busy-waiting
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100 milliseconds
    }
}

void PowerMonitor::staticWrapper(void* context, uint32_t arg) 
{
    PowerMonitor * instance = static_cast<PowerMonitor*>(context);
    instance->notifyFromISR(arg);
}

void PowerMonitor::notifyFromISR(uint32_t arg)
{
    uint32_t notifyBit = arg;

    /* notify the task from ISR */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(this->getTaskHandle(), notifyBit, eSetBits, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR();
}

