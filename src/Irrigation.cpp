#include <RedGlobals.h>

// Zone variables
int zonePins[NUMBER_OF_ZONES] = {ZONE1, ZONE2, ZONE3, ZONE4, ZONE5};
int zoneTime[NUMBER_OF_ZONES];
Ticker zoneTimeTicker;


// winterization variables
#define WINTERIZE_ON 15
#define WINTERIZE_OFF 5*60
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

void processIrrigationCommand(int zone, char* command)
{

  char str[128];
  sprintf(str, "Zone %i received command %s", zone, command);
  console.println(str);

  // check for winterize command
  if (strcmp(command, "winterize") == 0) {
    winterizeON(zone);
  }
  else
  {
    //if we have received a command while in winterize mode ==> turn it off
    winterizeModeOFF();

    // NB: strtol return 0 if not numeric. which shuts off the zone
    long duration = strtol(command, NULL, 0);


    if (duration <= 0) zoneOFF(zone);
    else // turn it on for selected number of minutes
    {
      zoneTime[zone] = duration;
      zoneON(zone);
    }
  }

  // report status back
  reportZoneStatus(zone, command);
}

// this routine is invoked every minutes. It looks to see if any
// zone is on. If it is, it is decremented.
// once the count reaches zero the zone is turned off
void irrigationTimer()
{
  // do nothing if we are winterizing
  if (winterizeMode)
    return;

  // normal mode
  console.println("zone timer...");
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
    //reportZoneStatus(i);
    char stt[10];
    // update the status topic
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
  char str[128];
  sprintf(str, "Winterizing Zone %i", zone);
  console.println(str);
  winterizeTimer.detach();

  // First: turn off all zone
  for (int i = 0; i < NUMBER_OF_ZONES; i++)
  {
    zoneTime[i] = 0;
    zoneOFF(i);
    //reportZoneStatus(i);
  }

  winterizeMode = true;
  winterizeZone = zone;
  winterizeTimer.attach(5, winterizeZoneON);

}

// turn off winterize mode
void winterizeModeOFF()
{
  winterizeMode = false;
  winterizeZone = -1;
  winterizeTimer.detach();

}

// this routine is called by a winterizing timer
// it turns the zone ON then fires a timer to call turn if off in WINTERIZE_ON seconds
void winterizeZoneON()
{
  winterizeTimer.detach();  // first detach the timer
  zoneON(winterizeZone);
  ledON();
  winterizeTimer.attach(WINTERIZE_ON, winterizeZoneOFF);

}
// this routine is called by a winterizing timer
// it turns the zone ON then fires a timer to call turn if off in WINTERIZE_ON seconds
void winterizeZoneOFF()
{
  winterizeTimer.detach();  // first detach the timer
  zoneOFF(winterizeZone);
  ledOFF();
  winterizeTimer.attach(WINTERIZE_OFF, winterizeZoneON);
}






// turn a specific zone ON
bool zoneON(int zone)
{
  if ( (zone >= 0) && (zone < NUMBER_OF_ZONES) )
  {
    digitalWrite(zonePins[zone], LOW);
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
    reportZoneStatus(zone,(char*) "OFF");
    return true;
  }
  else
  {
    return false;
  }
}

