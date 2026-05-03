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
#include "task.hpp"
#include "bus_voltage.hpp"
#include "bus_current.hpp"
#include "networking.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/
enum class NotifyBits : uint32_t
{
    GET_POWER = 0x01,
    GET_VOLTAGE = 0x02,
    GET_CURRENT = 0x04,
    CLEAR_ALL_BITS = 0xFFFFFFFF
};

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class BusVoltage;
class BusCurrent;

class PowerMonitor : public Task
{
public:
    PowerMonitor(NetworkingModule & networkingModule, 
                 BusVoltage & busVoltage, 
                 BusCurrent & busCurrent);
    ~PowerMonitor();

    //! TODO: remove this, only for testing
    TaskHandle_t getTaskHandle() const { return Task::getTaskHandle(); }
    
    /**
     * @brief Notifies the power monitor task from an ISR context.
     */
    static void staticWrapper(void* context, uint32_t arg);
    void notifyFromISR(uint32_t arg);

    /** @brief  Runs the power monitor task
     *  This function is called to start the power monitor task.
     */
    virtual void taskRun();

private:
    /*
        Battery Bus Voltage and Current
    */
    float latestBatteryBusVoltage;

    /*
        Solar Panel Bus Voltage and Current
    */
    float latestPanelBusVoltage;
    float latestPanelBusCurrent;

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
    uint32_t notificationValue = 0;
    /**
     * @brief Queues the bus voltage message for transmission.
     */
    Status_t queueBusVoltageMessage(void);

    Status_t startAndWaitForVoltage(float & value);

    /**
     * @brief Queues the bus current message for transmission.
     */
    Status_t queueBusCurrentMessage(void);

    Status_t startAndWaitForCurrent(float & value);
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