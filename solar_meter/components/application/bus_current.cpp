/**
 *******************************************************************************
 * @file    bus_current.cpp
 * @author  HQ
 * @date    2025-07-20
 * @brief   BusCurrent stub implementation
 *******************************************************************************
 */

#include "bus_current.hpp"

BusCurrent::BusCurrent(INA219 & _ina219)
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
