# Energy Meter #
Will measure power of a device as well as control it wirelessly. Currently only supports a single device. The end goal of this project is to not only measure power of a linear (resistive) load, but also a reactive load. This is currently done by measuring the instantaneous voltage and current, and multiplying them to get the instantaneous power, which is then passed through a low pass filter to get the average power. 

## Hardware ##
* Microcontroller - ESP32
* Current Sensor - ACS712-05
* Voltage Sensor - ZMPT101B
* Relay

## To Do ##
* Phase measurement