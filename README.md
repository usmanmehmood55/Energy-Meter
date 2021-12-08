# Energy Meter #
Will measure power of a device as well as control it wirelessly. Currently only supports a single device. The end goal of this project is to not only measure power of a linear (resistive) load, but also a reactive load. This is currently done by measuring the instantaneous voltage and current, and multiplying them to get the instantaneous power, which is then passed through a low pass filter to get the average power. 

## Hardware ##
* Microcontroller - ESP32
* Current Sensor - ACS712-05
* Voltage Sensor - ZMPT101B
* Relay

## Folder Structure ##
```
|-- lib
|   |-- board ----------------> Hardware difinitions for the ESP32 dev board
|   |   |-- board.h
|   |
|   |-- lagFilter ------------> Filter to purposefully delay voltage and current
|   |   |-- lagFilter.h
|   |   |-- lagFilter.c
|   |
|   |-- lowpassFilter --------> Filter to smoothen out and remove AC components from power
|   |   |-- lowpassFilter.h
|   |   |-- lowpassFilter.c
|   |
|   |-- mainsFilter ----------> Filter to remove DC bias and allow only AC mains frequencies
|   |   |-- mainsFilter.h
|   |   |-- mainsFilter.c
|   |
|   |-- power ----------------> Library to do power related calculations
|   |   |-- power.hpp
|   |   |-- power.cpp
|
|-- src ----------------------> Main code file
|   |-- main.cpp
|
|-- .gitignore
|-- LICENSE
|-- platformio.ini
|-- README.md
```
## To Do ##
* Test values and their underlying types in power.cpp
* Self calibration functions of current and voltage sensors
* Addition of MQTT to send data to a server
* Phase measurement
* Investigate weird harmonics in voltage measurements as sampling rate increases