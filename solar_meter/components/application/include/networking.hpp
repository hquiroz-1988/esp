/**
 *******************************************************************************
 * @file    networkingmodule.hpp
 * @author  HQ
 * @date    2025-07-20 10:22:46
 * @brief   
 *******************************************************************************
 */

#ifndef NETWORKINGMODULE_HPP
#define NETWORKINGMODULE_HPP

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "helper.h"
#include "typedefs.h"
#include <string>

/*******************************************************************************
 * MACROS AND DEFINES
*******************************************************************************/

/*******************************************************************************
 * TYPEDEFS
*******************************************************************************/
typedef struct
{
    /**
     * @brief NetworkingModule packet structure
     * This structure defines the format of a networkingmodule packet.
     */
    std::string name; // Name of the networkingmodule packet
    uint32_t timestamp; // Timestamp of the networkingmodule packet
    uint16_t size; // Size of the networkingmodule packet in bytes
    void * dataPtr; // Data payload of the networkingmodule packet, size can be adjusted as needed
} NetworkingMessage_t;

class NetworkingModule
{
public:
    NetworkingModule() = default;
    ~NetworkingModule() = default;
    void init(void);
    Status_t queueNetworkingMessage(NetworkingMessage_t * message);
private:
    // Add any private members or methods if necessary
};

/*******************************************************************************
 * EXPORTED VARIABLES
*******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
*******************************************************************************/


#endif // NETWORKINGMODULE_HPP