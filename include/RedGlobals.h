
#ifndef RED_GLOBALS_H
#define RED_GLOBALS_H
#include <dConsole.h>
#include <PubSubClient.h>
#include <Ticker.h>

#ifndef _PINS_H
#include <pins.h>
#endif

#include <dConsole.h>

/*
 * ********************************************************************************
 *            START CONFIGURATION SECTION
 * ********************************************************************************
*/

//#define TEMP_SENSOR_PRESENT                 // indicates a temperature sensor is present

#ifdef TEMP_SENSOR_PRESENT
#define _TEMP_SENSOR_PERIOD 10000         // in ms the frequency of temperature sensor reading
#define _SEND_ROOM_TEMP_INTERVAL_MS 60000 // in ms how often the temperature is sent back to the server
#define TEMPERATURE_PRECISION 9           // Lower resolution
#endif

#define VERSION "V1"             // N.B: document changes in README.md
#define MQTT_TOPIC_PREFIX "irrigation" // prefix for all MQTT topics

// winterization variables: times in seconds zone is on & off
#define WINTERIZE_ON   30
#define WINTERIZE_OFF  (5*60 - WINTERIZE_ON)

// in WiFiConfig
extern char myHostName[];
extern char deviceLocation[];
extern char mqttServer[];
extern char mqttPort[];
extern char mqttUser[];
extern char mqttPwd[];
void configureESP();       // load configuration from FLASH & configure WIFI
void checkConnection(); // check WIFI connection
void writeConfigToDisk();
void configureOTA(char *hostName);

// in MQTTConfig
extern bool debugMode;
void configureMQTT();
bool checkMQTTConnection();
void mqttDisconnect();
void mqttCallback(char *topic, byte *payload, unsigned int length);
void reportZoneStatus(int zone, char *command);

// in console.ino
extern dConsole console;
void setupConsole();
void handleConsole();

#ifdef TEMP_SENSOR_PRESENT
// in Sensors.ino
void configSensors(long interval, void (*sensorCallback)(float insideTemp, float outsideTemp));
void serviceSensors();
#endif

// in irrigation
void initializeIrrigation();
void processIrrigationCommand(int zone, char *command);
void irrigationTimer();
void winterizeON(int zone);
void winterizeModeOFF();
void winterizeZoneON();
void winterizeZoneOFF();
bool zoneON(int zone);
bool zoneOFF(int zone);
bool isZoneOn(int zone);

// in RedIrrigation.cpp
void ledON();
void ledOFF();
void tick();

#ifdef TEMP_SENSOR_PRESENT
    void updateTemperature(float temp, float outdoorTemp);
#endif

#endif
