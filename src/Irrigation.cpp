/*********************************************************************************
 * 
 * 
 * 
 ********************************************************************************/
#include <RedGlobals.h>

// Zone variables
int zonePins[NUMBER_OF_ZONES] = {ZONE1, ZONE2, ZONE3, ZONE4, ZONE5, ZONE6};
int zoneTime[NUMBER_OF_ZONES];
Ticker zoneTimeTicker;


bool winterizeMode; // winterize or normal mode
int winterizeZone;  // zone being winterized
Ticker winterizeTimer; // timer to use while winterizing


// Initialize all pins and make sure all zones are off
void initializeIrrigation()
{
  winterizeMode = false;

  // configure & turn off all zones
  for (int i = 0; i < NUMBER_OF_ZONES; i++)
  {
    pinMode(zonePins[i], OUTPUT);
    zoneTime[i] = 0;
    zoneOFF(i);
  }

  // attach a timer to monitor zones
  zoneTimeTicker.attach(60, irrigationTimer);
}


/** ********************************************************************************

   This routine handles time related irrigation commands and forwards them to
   the ZoneControl module

 * *********************************************************************************/

void processIrrigationCommand(int zone, char *command)
{

  char str[128];
  sprintf(str, "Zone %i received command %s", zone+1, command);
  console.println(str);

  // check for winterize command
  if (strcmp(command, "winterize") == 0)
  {
    winterizeON(zone);
    reportZoneStatus(zone, command);
  }
  else
  {
    //if we have received a command while in winterize mode ==> turn it off
    if (winterizeMode) winterizeModeOFF();

    if (strcmp(command, "ON") == 0)
    {
      zoneTime[zone] = 60;
      zoneON(zone);
    }
    else if (strcmp(command, "OFF") == 0)
    {
      zoneTime[zone] = 0;
      zoneOFF(zone);
    }
    else

    {

      // NB: strtol return 0 if not numeric. which shuts off the zone
      long duration = strtol(command, NULL, 0);

      if (duration <= 0)
        zoneOFF(zone);
      else // turn it on for selected number of minutes
      {
        zoneTime[zone] = duration;
        zoneON(zone);
      }
    }
    // report status back
    char stt[10];
    sprintf(stt, "%i", zoneTime[zone]);
    reportZoneStatus(zone, stt);
  }
  
}

// this routine is invoked every minutes. It looks to see if any
// zone is on. If it is, it is decremented.
// once the count reaches zero the zone is turned off
void irrigationTimer()
{
  // do nothing if we are winterizing
  if (winterizeMode) return;

  // normal mode
  console.println("Irrigation timer...");
  tick(); // toggle LED

  for (int i = 0; i < NUMBER_OF_ZONES; i++)
  {
    if (zoneTime[i] > 0) // see if any time is left on the zone
    {
      zoneTime[i]--; // 1 minute has gone by
      if (zoneTime[i] == 0)
      {
        zoneOFF(i); // turn off once it reaches zero
        console.println("Turning off Zone#");
        console.println(i);
      }
    }
    // update the status topic
    char stt[10];
    sprintf(stt, "%i", zoneTime[i]);
    reportZoneStatus(i, stt);
  }
}

/*
 * ********************************************************************************

   This routine handles winterizing a zone by:
   1. turning it on for winterizeON seconds to blow the water
   2. turning it off for wintereizeOFF seconds to recharge the compressor
   this repeats until it is turned off

 * ********************************************************************************
*/
void winterizeON(int zone)
{
  winterizeTimer.detach();

  // First: turn off all zone
  for (int i = 0; i < NUMBER_OF_ZONES; i++)
  {
    zoneTime[i] = 0;
    zoneOFF(i);
    //if (i != zone) reportZoneStatus(i,0);
    //else reportZoneStatus(zone, "w");
  }

  winterizeMode = true;
  winterizeZone = zone;
  winterizeTimer.attach(5, winterizeZoneON);

  char str[128];
  sprintf(str, "Winterizing MODE ON - Zone %i", zone + 1);
  console.println(str);
  
}

// turn off winterize mode
void winterizeModeOFF()
{
  winterizeMode = false;
  winterizeZone = -1;
  winterizeTimer.detach();
  char str[128];
  sprintf(str, "Winterizing MODE OFF");
  console.println(str);
}

// this routine is called by a winterizing timer
// it turns the zone ON then fires a timer to call turn if off in WINTERIZE_ON seconds
void winterizeZoneON()
{
  console.printf("Winterizing zone %i : ON\r\n", winterizeZone+1);
  winterizeTimer.detach();  // first detach the timer
  zoneON(winterizeZone);
  winterizeTimer.attach(WINTERIZE_ON, winterizeZoneOFF);

}
// this routine is called by a winterizing timer
// it turns the zone ON then fires a timer to call turn if off in WINTERIZE_ON seconds
void winterizeZoneOFF()
{
  console.printf("Winterizing zone %i : OFF\r\n", winterizeZone + 1);
  winterizeTimer.detach(); // first detach the timer
  zoneOFF(winterizeZone);
  winterizeTimer.attach(WINTERIZE_OFF, winterizeZoneON);
}






// turn a specific zone ON
bool zoneON(int zone)
{
  if ( (zone >= 0) && (zone < NUMBER_OF_ZONES) )
  {
    digitalWrite(zonePins[zone], LOW);
    if (!winterizeMode) 
      reportZoneStatus(zone, (char*)"ON");
    return true;
  }
  else
  {
    return false;
  }
}

// turn a specific zone OFF
bool zoneOFF(int zone)
{
  if ( (zone >= 0) && (zone < NUMBER_OF_ZONES) )
  {
    digitalWrite(zonePins[zone], HIGH);
    if (!winterizeMode) 
      reportZoneStatus(zone, (char *)"OFF");
    return true;
  }
  else
  {
    return false;
  }
}

// returns a boolean with the status of a zone
bool isZoneOn(int zone)
{
  if ((zone >= 0) && (zone < NUMBER_OF_ZONES))
  {
    return !digitalRead(zonePins[zone]);
  }
  else
  {
    return false;
  }
}

