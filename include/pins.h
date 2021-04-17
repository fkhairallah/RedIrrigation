/*
 * ********************************************************************************
 * 
 * This program runs undercabinet LED lights AND temperature sensor from a single
 * WEMOS D1 esp8266
 * 
 * 
 * 
 * Hardware configuration:
 * 
 *   - Blue LED connected to pin 2
 *   - 1Wire thermocouple connected to pin 3
 *   - WS2812B individually addressable LED pin 4


    Hardware Notes:

    - GPIO-0 must be tied to ground for programming
    - GPIO-0 floats to run program
    - GPIO-0 runs Red LED on Huzzah
    - GPIO-2 is tied to Blue Led (*NOT* a PWM pin)
    - GPIO-13 is RESERVED

 * ********************************************************************************
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
//#define ZONE5 10 // BROWN -- NC
#define NUMBER_OF_ZONES 5 // total number of zones

#endif