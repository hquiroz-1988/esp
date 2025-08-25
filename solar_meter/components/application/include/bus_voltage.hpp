/**
 ********************************************************************************
 * @file    bus_voltage.h
 * @author  Hugo Quiroz
 * @date    2024-09-27 10:24:18
 * @brief   description
 ********************************************************************************
 */

#ifndef BUS_VOLTAGE_H
#define BUS_VOLTAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
extern "C" 
{

}

#include "typedefs.h"
#include "ads1115_channel.hpp"
#include "power_monitor.hpp"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
class PowerMonitor;

class BusVoltage : public ADS1115Channel
{
public:
    BusVoltage(ADS1115 & _ads1115);
    virtual ~BusVoltage();

    /** @brief  Initializes Bus Voltage which is an ADS1115 channel type.
     *
     *  @param void 
     *  @return void
     */
    virtual Status_t initialize(PowerMonitor * _pm);

    /**
     * @brief  Runs the alert ISR for the BusVoltage.
     * 
     * @param arg - pointer to any arguments needed for the ISR
     * @return void
     */
    virtual void runAlertISR(void * arg);

    private:
    // Add private members if needed
    PowerMonitor *pm;
};

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

#ifdef __cplusplus
}
#endif

#endif //BUS_VOLTAGE_H