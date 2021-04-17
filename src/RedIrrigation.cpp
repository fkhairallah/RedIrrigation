/************************************************************************************

   Irrigation ESP

   Configuration parameters are found in .h file

  This code implements an irrigation control system tied into OpenHab home automation.
  It is designed to sit in front of a dumb irrigation controller and override its
  functionality. As such it has no direct UI.

  This code provides WIFI & MQTT configuration through the use of WIFI manager. Initial setup allow
  configuration of WIFI network and MQTT server

  It provides OTA functionality.

 *************************************************************************************/
#include <Arduino.h>
#include <RedGlobals.h>
#include <Ticker.h>

#ifdef TempSensor
float tempAccumulator;   // keeps sum of  temperature as reported by sensor
int tempNumberOfReading;  // keeps # of readings
unsigned long lastTempSend;
float averageTemp;        // Average temp for the last interval -- what is displayed
#endif

Ticker ticker;
/*
 * ********************************************************************************

 a few routines to drive the onboard blueLED

 * ********************************************************************************
*/
void ledON()
{
  digitalWrite(blueLED, false);
}
void ledOFF()
{
  digitalWrite(blueLED, true);
}

void tick()
{
  //toggle state
  int state = digitalRead(blueLED); // get the current state of GPIO1 pin
  digitalWrite(blueLED, !state);    // set pin to the opposite state
}

/*
 * ********************************************************************************

   Perform the initial hardware setup and then sequence the starting of the
   various modules

 * ********************************************************************************
*/

void setup() {

  // das blinken light!
  pinMode(blueLED, OUTPUT);

  // configure zone relay pins as output
  initializeIrrigation();


  // start ticker with 0.5 because we start in AP mode and try to connect

  setupConsole();

  console.enableSerial(&Serial, true);
  console.print("[RED]Irrigation ");
  console.println(VERSION);


  // Configure WIFI
  configureESP(); // load configuration from FLASH & configure WIFI

  console.print("Connected! IP address: ");
  console.println(WiFi.localIP().toString());

  // configure MQTT server
  configureMQTT();
  

#ifdef TempSensor
  //  // configure thermostat sensor
  tempAccumulator = 0;
  tempNumberOfReading = 0;
  averageTemp = -9999;
  lastTempSend = millis();
  configSensors(_TEMP_SENSOR_PERIOD, &updateTemperature);
#endif

  ledOFF();
}


/*
 * ********************************************************************************

   main loop services all modules: Wifi, MQTT, HP, console and webserver

 * ********************************************************************************
*/
void loop() {

  checkConnection();  // check WIFI connection

#ifdef TempSensor
  // service temperature and other sensos
  serviceSensors();
#endif

  checkMQTTConnection();  // check MQTT


  // handle any commands from console
  handleConsole();

}

#ifdef TempSensor
/*
 * ********************************************************************************

   This is callback routine that is called when the temperature sensor receives
   a reading. It collects the data into a long term average then,
   every _TEMP_SENSOR_PERIOD report the value back to the MQTT server

 * ********************************************************************************
*/
void updateTemperature(float temp)
{
  char str[128];
  console.println("Reporting temp reading of " + String(temp));
  tempAccumulator += temp;
  tempNumberOfReading++;

  if ((unsigned long)(millis() - lastTempSend) >= _SEND_ROOM_TEMP_INTERVAL_MS)
  {

    averageTemp = tempAccumulator / tempNumberOfReading;
    tempAccumulator = 0;
    tempNumberOfReading = 0;
    sprintf(str, "%.1f", averageTemp);
    mqtt_client.publish(mqtt_temperature_topic, str);


    tick();
  }

}
#endif
