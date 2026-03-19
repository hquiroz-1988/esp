/**
 ********************************************************************************
 * @file    bus_voltage.hpp
 * @author  Hugo Quiroz
 * @date    2024-09-27 10:24:18
 * @brief   description
 ********************************************************************************
 */

#ifndef BUS_VOLTAGE_H
#define BUS_VOLTAGE_H


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
    BusVoltage(ADS1115 & _ads1115, ADS1115Mux_t _channel);
    virtual ~BusVoltage();

    /** @brief  Initializes Bus Voltage which is an ADS1115 channel type.
     *
     *  @param void 
     *  @return void
     */
    virtual Status_t initialize(void);

    protected:

    private:
};

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#endif //BUS_VOLTAGE_H