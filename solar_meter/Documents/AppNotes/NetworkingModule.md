# Application Note: Networking Module

## Table of Contents
- [Overview](#overview)
- [Sequence Diagram](#sequence-diagram)
- [State Machine Diagram](#state-machine-diagram)
- [Class Diagram](#class-diagram)
- [Conclusion](#conclusion)

### Overview

The Networking Module is responsible for collecting, packaging, and transmitting system data to a central Broker. It ensures reliable communication between distributed modules, such as the Power Monitor and Current Monitor, and external systems.


The Networking Module interfaces with other system modules along with the MQTT instance to receivea and transmit data.

### Sequence Diagram

```mermaid
sequenceDiagram
    participant PowerMonitor
    participant NetworkingModule
    participant MQTTClient
    participant MQTTBroker

    PowerMonitor->>NetworkingModule: Send data
    NetworkingModule->>NetworkingModule: Package data (JSON)
    NetworkingModule->>MQTTClient: Forward data
    MQTTClient->>MQTTBroker: Publish data
    MQTTBroker->>MQTTClient: Request/Subscribe data
    MQTTClient->>NetworkingModule: Forward Data
    NetworkingModule->>NetworkingModule: Unpack data (JSON)
    NetworkingModule->>PowerMonitor: Send data
```

The Networking Module follows the following state machine.

### State Machine Diagram

```mermaid
stateDiagram-v2
    [*] --> InitNetwork
    InitNetwork --> ConfigureMQTT
    ConfigureMQTT --> AcquireData
    AcquireData --> PackageData
    PackageData --> PublishData
    PublishData --> AcquireData : next interval
    PublishData --> ErrorHandling : on failure
    ErrorHandling --> AcquireData : recover
```

The Networking Module uses the following interface

### Class Diagram

```mermaid
classDiagram

    class NetworkingMessage_t{
        +string msgName;
        +uint16_t size;
        +void * dataPtr;
    }
    
    class NetworkingModule~Task~{
        -queue~NetworkingMessage_t *~ msgQueue
        +void init(void)
        +Status_t queueNetworkingMsg(NetworkingMessage_t *)
    }
    class Task{

    }

    NetworkingModule --> NetworkingMessage_t : uses
    NetworkingModule --> Task : inherits from
```

### Conclusion

The Networking Module provides a robust mechanism for real-time data exchange, enabling monitoring and control of the Solar Meter system from remote locations.
