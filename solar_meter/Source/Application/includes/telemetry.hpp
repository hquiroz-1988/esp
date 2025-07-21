/**
 *******************************************************************************
 * @file    telemetry.hpp
 * @author  HQ
 * @date    2025-07-20 10:22:46
 * @brief   
 *******************************************************************************
 */

#ifndef TELEMETRY_HPP
#define TELEMETRY_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "common.h"

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
typedef struct
{
    /**
     * @brief Telemetry packet structure
     * This structure defines the format of a telemetry packet.
     */
    uint8_t header[2]; // Packet header
    const char * packet_name; // Name of the packet
    uint8_t type;      // Packet type
    uint8_t length;    // Length of the payload
    uint8_t payload[]; // Variable length payload
} telemetry_packet_t;

class Telemetry
{
public:
    Telemetry() = default;
    ~Telemetry() = default;
    void init(void);
    retVal_t sendPacket(telemetry_packet_t * packet);
private:
    // Add any private members or methods if necessary
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // TELEMETRY_HPP