/**
 *******************************************************************************
 * @file    bus_current.hpp
 * @author  HQ
 * @date    2025-07-20 09:56:53
 * @brief   
 *******************************************************************************
 */

#ifndef BUS_CURRENT_HPP
#define BUS_CURRENT_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "helper.h"
#include "power_monitor.hpp"
#include "ina219.hpp"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
class PowerMonitor;

class BusCurrent
{
    public:
    BusCurrent(INA219 & _ina219);
    virtual ~BusCurrent();
    Status_t getCurrentAndBusVoltage(float & current, float & busVoltage);

    private:
    INA219 & ina219;
    void init(void);
    Status_t getFilteredCurrent(float * value);
    Status_t getLatestConversion(float & value);
    Status_t startConversion(void);
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // BUS_CURRENT_HPP