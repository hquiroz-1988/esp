/**
 *******************************************************************************
 * @file    bus_current.cpp
 * @author  HQ
 * @date    2025-07-20
 * @brief   BusCurrent stub implementation
 *******************************************************************************
 */

#include "bus_current.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace
{
constexpr TickType_t BUS_CURRENT_CONVERSION_DELAY_TICKS = pdMS_TO_TICKS(1);
}

BusCurrent::BusCurrent(INA219 & _ina219)
    : ina219(_ina219)
{
    //! TODO: implement constructor
}

BusCurrent::~BusCurrent()
{
    //! TODO: implement destructor
}

void BusCurrent::init(void)
{
    //! TODO: implement init
}

Status_t BusCurrent::getFilteredCurrent(float * value)
{
    //! TODO: implement getFilteredCurrent
    return STATUS_OKAY;
}

Status_t BusCurrent::getLatestConversion(float & value)
{
    Status_t status = STATUS_OKAY;

    //!TODO: implement getLatestConversion,

    return status;
}

Status_t BusCurrent::getCurrentAndBusVoltage(float & current, float & busVoltage)
{
    Status_t status = STATUS_OKAY;

    /* start a single bus voltage and shunt voltage conversion */
    status = ina219.triggerSingleMeasurement(INA219_OperatingMode_t::ShuntAndBusSingle);

    if(status == STATUS_OKAY)
    {
        /*
            wait for conversion to complete, this is because ina219 has no interrupt
            to notify of conversion complete.
        */
        vTaskDelay(BUS_CURRENT_CONVERSION_DELAY_TICKS);

        /* read the current and bus voltage values */
        status = ina219.readCurrentAndBusVoltage(current, busVoltage);
    }

    return status;
}




Status_t BusCurrent::startConversion(void)
{
    Status_t status = STATUS_OKAY;

    /* start a single bus voltage and shunt voltage conversion */
    status = ina219.triggerSingleMeasurement(INA219_OperatingMode_t::ShuntAndBusSingle);

    return status;
}
