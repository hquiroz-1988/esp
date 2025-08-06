/**
 ********************************************************************************
 * @file    power_monitor.hpp
 * @author  hq
 * @date    2025-07-16 13:00:16
 * @brief   Power monitor header file
 ********************************************************************************
 */

#ifndef POWER_MONITOR_HPP
#define POWER_MONITOR_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "Task.hpp"
#include "bus_voltage.hpp"
#include "bus_current.hpp"
#include "networking.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/
#define GET_POWER_NOTIFY_BIT    (0x01)

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class PowerMonitor : public Task
{
public:
    PowerMonitor(NetworkingModule & networkingModule, 
                 BusVoltage & busVoltage, 
                 BusCurrent & busCurrent);
    ~PowerMonitor();

private:
    /**
     * @brief Stores the latest measured bus voltage value.
     */
    float latestBusVoltage;
    /**
     * @brief Stores the latest measured bus current value.
     */
    float latestBusCurrent;
    /** * @brief Stores the latest measured power value.
     */
    float latestPower;

    /** @brief  Bus voltage object
     *  This object is used to interact with the bus voltage module.
     */
    BusVoltage & busVoltage;
    /** @brief  Bus current object
     *  This object is used to interact with the bus current module.
     */
    BusCurrent & busCurrent;

    /** @brief  NetworkingModule object
     *  This object is used to send networking data.
     */
    NetworkingModule & networkingModule;

    /** @brief  Telemetry object
     *  This object is used to send telemetry data.
     */
    NetworkingMessage_t busVoltageMessage;
    NetworkingMessage_t busCurrentMessage;
    NetworkingMessage_t powerMessage;

    /** @brief  Runs the power monitor task
     *  This function is called to start the power monitor task.
     */
    virtual void taskRun();
    /**
     * @brief Queues the bus voltage message for transmission.
     */
    Status_t queueBusVoltageMessage(void);
    /**
     * @brief Queues the bus current message for transmission.
     */
    Status_t queueBusCurrentMessage(void);
    /**
     * @brief Queues the power message for transmission.
     */
    Status_t queuePowerMessage(void);
};


/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // POWER_MONITOR_HPP