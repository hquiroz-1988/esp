# Sunlight Meter

## Table of Contents
- [Introduction](#introduction)
- [System Diagram](#system-diagram)
- [Circuit Diagram](#circuit-diagram)

## Introduction
This project seeks to use a small 1 watt solar panels to characterize the amount of sunlight in a given area. This data will ultimately be collected and analyzed to determine the best area to place plants with varying sunlight requirements i.e. low, mid, full.

## System Diagram
The following diagram describes the connections between sub-systems. These are the solar panel, discharge load, ESP32 (client), and RPI (server).

```mermaid
block-beta
  columns 4
  solarPanel("Solar Panel"):4
  space arrow1<["Charge"]>(down):2 space
  battery("Battery"):4
  battToMCU<["Discharge"]>(down):2 space:2
  led("LED Lighting") mcu("ESP MCU") mcuToRpi<["Network"]>(x) server("RPI Server")
  
  classDef green fill:#696,stroke:#333;
  classDef pink fill:#969,stroke:#333;

  class battery,solarPanel green
  class mcu,server,led pink
```


## Circuit Diagram
The following circuit diagram describes the interface between solar panel, battery cell/s, ADS1115 ADC, INA219 Current Sensor, and the ESP32 Module.

![alt text](/solar_meter/Documents/Images/solar_meter_circuit_diagram.png)


## ESP32 Module Overview

```mermaid
block-beta
  columns 8

  block:powerMonitor:8
    columns 2
    PowerMonitor Telemetry
  end

  %% first row
  block:busvoltage:2
    columns 1
    BusVoltage ADS1115
  end

  %% second row
  block:currentmonitor:2
    columns 1
    CurrentMonitor INA219
  end

  block:mqtt:4
    columns 1
    MQTT
  end

  %% OS Layer
  block:osLayer:8
    %% columns auto (default)
    RTOS
  end

  %% hal layer
  block:halLayer:4
    columns 1
    I2C GPIO
  end

  block:timer:4
    Timer 
  end

```

## RPI Server Module Overview

tbd



