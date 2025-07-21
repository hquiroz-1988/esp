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
#include "telemetry.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class PowerMonitor : public Task
{
public:
    PowerMonitor(Telemetry & telemetry);
    virtual ~PowerMonitor();

private:
    /** @brief  Bus voltage object
     *  This object is used to interact with the bus voltage module.
     */
    BusVoltage busVoltage;
    /** @brief  Bus current object
     *  This object is used to interact with the bus current module.
     */
    BusCurrent busCurrent;

    /** @brief  Telemetry object
     *  This object is used to send telemetry data.
     */
    Telemetry & telemetry;

    /** @brief  Runs the power monitor task
     *  This function is called to start the power monitor task.
     */
    virtual void taskRun();
    // Add private members and methods as needed
};


/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // POWER_MONITOR_HPP