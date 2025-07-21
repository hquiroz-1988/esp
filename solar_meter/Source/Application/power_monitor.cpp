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

/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTIONS
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/
PowerMonitor::PowerMonitor(Telemetry & _telemetry) 
: Task("PowerMonitor", 256 * 4), telemetry(_telemetry)
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
        retVal_t ret = busVoltage.getFilteredVoltage(&voltage);
        if (ret == ERR_NONE)
        {
            // Process the voltage value as needed
            // For example, log the voltage or take action based on its value
        }

        /* collect current data */
        float current = 0.0f;
        retVal_t ret = busCurrent.getFilteredCurrent(&current);
        if (ret == ERR_NONE)
        {
            // Process the current value as needed
            // For example, log the current or take action based on its value
        }

        /* once we have current and voltage calculate power data */
        float power = voltage * current; // Simple power calculation
        // Process the power value as needed
        // For example, log the power or take action based on its value

        /*   send the voltage, current, and power packets to telemetry module */
        telemetry_packet_t packet;
        packet.header[0] = 0xAA; // Example header
        packet.type = 0x01;      // Example type
        packet.length = sizeof(packet) - sizeof(packet.header);
        packet.payload[0] = voltage;
        packet.payload[1] = current;
        packet.payload[2] = power;

        telemetry.sendPacket(&packet);


        // Add a delay or yield to avoid busy-waiting
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100 milliseconds
    }
}