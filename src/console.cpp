#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <RedGlobals.h>

dConsole console;
/*
 * ********************************************************************************

 * ********************************************************************************
*/

void setupConsole()
{
  console.enableSerial(&Serial, true);
  console.enableTelnet(23);
}


void handleConsole()
{
  
  // console
  if (console.check())
  {
    if (strcmp(console.commandString, "?") == 0)
    {
      console.printf("\n\n\n[RED]Irrigation %s ",VERSION);
      console.println(WiFi.localIP().toString());
      console.println("Available commands are:  1, mqtt server port, status, reset (Factory), reboot");
    }
    if (strcmp(console.commandString, "reset") == 0)
    {
      console.print("\nReseting configuration...");
      //reset settings - for testing
      WiFiManager wifiManager;
      wifiManager.resetSettings();
      // console.println(" Done.");
    }
    if (strcmp(console.commandString, "reboot") == 0)
    {
      console.print("\nRebooting...");
      delay(200);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    if (strcmp(console.commandString, "status") == 0)
    {
      console.println();
      console.println(myHostName);
      console.printf("MQTT Server %s, port: %s\r\n", mqttServer, mqttPort);
    }

    if (strcmp(console.commandString, "mqtt") == 0)
    {
      strcpy(mqttServer, console.parameterString);
      writeConfigToDisk();
      mqtt_client.disconnect();
      console.print("\nMQTT server changed to ");
      console.println(mqttServer);
    }

#ifdef TempSensor
    if (strcmp(console.commandString, "1") == 0)
    {
      console.println("Scanning for 1Wire");
      configSensors(_TEMP_SENSOR_PERIOD, &updateTemperature);
    }
#endif
    console.print("[RED]> ");
  }
}
