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
Status_t PowerMonitor::queueBusVoltageMessage()
{
    Status_t status = busVoltage.getFilteredVoltage(&latestBusVoltage);
    if (status == STATUS_OKAY)
    {
        busVoltageMessage.name = "BusVoltage";
        busVoltageMessage.timestamp = xTaskGetTickCount();
        busVoltageMessage.size = sizeof(float);
        busVoltageMessage.dataPtr = &latestBusVoltage;
        status = networkingModule.queueNetworkingMessage(&busVoltageMessage);
    }

    return status;
}

Status_t PowerMonitor::queueBusCurrentMessage()
{
    Status_t status = busCurrent.getFilteredCurrent(&latestBusCurrent);
    if (status == STATUS_OKAY)
    {
        busCurrentMessage.name = "BusCurrent";
        busCurrentMessage.timestamp = xTaskGetTickCount();
        busCurrentMessage.size = sizeof(float);
        busCurrentMessage.dataPtr = &latestBusCurrent;
        status = networkingModule.queueNetworkingMessage(&busCurrentMessage);
    }

    return status;
}

Status_t PowerMonitor::queuePowerMessage()
{
    latestPower = latestBusVoltage * latestBusCurrent;

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
    // Constructor implementation
}


PowerMonitor::~PowerMonitor()
{
    // Destructor implementation
}


void PowerMonitor::taskRun()
{
    /*  initialize power monitor variables     */

    while(FOREVER())
    {
        // Main loop for power monitoring
        // Implement the logic to monitor power here

        // Example: Read power sensor data, process it, and log or send it
        // This is a placeholder for actual implementation

        float voltage = 0.0f;
        Status_t ret = busVoltage.getFilteredVoltage(&voltage);
        if (ret == STATUS_OKAY)
        {
            // Process the voltage value as needed
            // For example, log the voltage or take action based on its value
        }

        /* collect current data */
        float current = 0.0f;
        ret = busCurrent.getFilteredCurrent(&current);
        if (ret == STATUS_OKAY)
        {
            // Process the current value as needed
            // For example, log the current or take action based on its value
        }

        /* once we have current and voltage calculate power data */
        float power = voltage * current; // Simple power calculation
        // Process the power value as needed
        // For example, log the power or take action based on its value

        /*   send the voltage, current, and power packets to telemetry module */
        if (status != STATUS_OKAY)
        {
            ESP_LOGE(TAG, "Error: %i", status);
        }

        // Add a delay or yield to avoid busy-waiting
        //!TODO: Uncomment the following before merging into main branch
        // vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100 milliseconds
    }
}