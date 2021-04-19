/*
 *********************************************************************************
                     Hardware pin definition
 *********************************************************************************
 */
#ifndef _PINS_H
#define _PINS_H

// hardware pin definitions
#define pgm_pin 0
#define blueLED 2      // blue LED light (D1 Mini D4)


// ***BEWARE*** Some ESP8266 pins (15) are set to LOW during boot or flash which activates the relay

// Zone Pin <--> GPIO#
#define ZONE1 5  // RED
#define ZONE2 4  // GREEN
#define ZONE3 14 // BLUE
#define ZONE4 12 // ORANGE
#define ZONE5 13 //YELLOW
#define ZONE6 15 // BROWN
#define NUMBER_OF_ZONES 6 // total number of zones

#endif