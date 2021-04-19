# RedIrrigation

[RED]Irrigation is an ESP8266-based IOT that drives RyeManor's Irrigation system

The hardware is designed to drive 6 zones even though only 5 are installed in Rye Manor. The implementation in OpenHab also only supports 5 zones, though it's easy to add a 6th.

## MQTT communication
Each zone has a status topic and a command topics. It can accept the following commands:
    - ON -- turn zone on for 60 minutes
    - OFF -- turn zone off
    - a number -- turn zone on for that many minutes
    - winterize -- turn zone on for 15 seconds, then off for 5 minutes to allow compressor to recharge
        this mode is on until it is turned off

The state topic is updated after the command is executed and continues to to see an updated countdown of remaining time on.

## Zone Layout
![](./media/lawn.jpg)




# Version History

## 0.2 Beta
    Migration to Platform.IO including JSON6

## 0.9 RC
    Release candidate with full implementation of functionality including winterization
    to be installed in box under porch 4/19

# Hardware Notes

 Zone#, Pin, & Color:
-  ZONE1 5  // RED
-  ZONE2 4  // GREEN
-  ZONE3 14 // BLUE
-  ZONE4 12 // ORANGE
-  ZONE5 13 //YELLOW
-  ZONE6 15 // BROWN


// ***BEWARE*** Some ESP8266 pins (15) are set to LOW during boot or flash which activates the relay
