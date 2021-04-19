/**********************************************************************************
 * 
 *    Implements functionality for telnet and serial console
 * 
 *********************************************************************************/
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <RedGlobals.h>

dConsole console;

/*
 * ********************************************************************************

  ********************  CUSTOMIZABLE SECTION  ***************************

 * ********************************************************************************
*/
#define CUSTOM_COMMANDS "Zone Commands: on #, off #, status"

void executeCustomCommands(char* commandString,char* parameterString)
{
  console.println();

  if (strcmp(commandString, "on") == 0)
  {
    int zone = atoi(parameterString) - 1;
    processIrrigationCommand(zone,(char*)"ON");
  }

  if (strcmp(commandString, "off") == 0)
  {
    int zone = atoi(parameterString) - 1;
    processIrrigationCommand(zone, (char*)"OFF");
  }

  // show current temp
   if (strcmp(console.commandString, "status") == 0)
  {
    for (int i=0;i<NUMBER_OF_ZONES;i++)
    {
      console.printf("Zone %i is %s \r\n",i+1,isZoneOn(i)?"ON":"OFF");
    }
  }

}

/*
 * ********************************************************************************

    ********************  END OF CUSTOMIZABLE SECTION  ***************************

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
    executeCustomCommands(console.commandString, console.parameterString);

    if (strcmp(console.commandString, "?") == 0)
    {
      console.print("[RED]");
      console.print(MQTT_TOPIC_PREFIX);
      console.print(" ");
      console.println(VERSION);
      console.printf("Host: %s - %s @", myHostName, deviceLocation);
      console.println(WiFi.localIP().toString());
      console.printf("MQTT Server %s, port: %s\r\n", mqttServer, mqttPort);
      console.println("Commands: ?, debug, location room, mqtt server, reset (Factory), reboot, quit");
      console.println(CUSTOM_COMMANDS);
    }
    if (strcmp(console.commandString, "debug") == 0)
    {
      debugMode = !debugMode;
      console.print("Debug mode is now ");
      console.println(debugMode);
    }
    if (strcmp(console.commandString, "reset") == 0)
    {
      console.print("Reseting configuration...");
      //reset settings - for testing
      WiFiManager wifiManager;
      wifiManager.resetSettings();
      console.println(" Done.");
    }
    if (strcmp(console.commandString, "reboot") == 0)
    {
      console.print("Rebooting...");
      delay(200);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    if (strcmp(console.commandString, "mqtt") == 0)
    {
      strcpy(mqttServer, console.parameterString);
      writeConfigToDisk();
      console.print("MQTT server changed to ");
      console.println(mqttServer);
      mqttDisconnect();
    }
    if (strcmp(console.commandString, "location") == 0)
    {
      strcpy(deviceLocation, console.parameterString);
      writeConfigToDisk();
      console.printf("location changed to %s\r\n", deviceLocation);
      console.println("Change will take effect after next reboot");
    }
    if (strcmp(console.commandString, "quit") == 0)
    {
      console.print("quiting...");
      console.closeTelnetConnection();
      delay(500);
      console.println("");
    }

    console.print("[RED]> ");
  }
}
