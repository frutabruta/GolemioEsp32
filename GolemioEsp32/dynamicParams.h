/****************************************************************************************************************************
  dynamicParams.h
  For ESP8266 / ESP32 boards

  ESP_WiFiManager_Lite (https://github.com/khoih-prog/ESP_WiFiManager_Lite) is a library 
  for the ESP32/ESP8266 boards to enable store Credentials in EEPROM/SPIFFS/LittleFS for easy 
  configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services without Hardcoding.

  Built by Khoi Hoang https://github.com/khoih-prog/ESP_WiFiManager_Lite
  Licensed under MIT license
 *****************************************************************************************************************************/

#ifndef dynamicParams_h
#define dynamicParams_h

#include "defines.h"

// USE_DYNAMIC_PARAMETERS defined in defined.h

/////////////// Start dynamic Credentials ///////////////

//Defined in <ESP_WiFiManager_Lite.h>
/**************************************
  #define MAX_ID_LEN                5
  #define MAX_DISPLAY_NAME_LEN      16

  typedef struct
  {
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
  } MenuItem;
**************************************/

#if USE_DYNAMIC_PARAMETERS



#define MAX_GOLEMIO_TOKEN_LEN 160
#define MAX_GOLEMIO_PARAMETRY_LEN 200



char golemioParametry [MAX_GOLEMIO_PARAMETRY_LEN+1]= "?stopIds[]={\"0\":[\"U511Z1P\"]}&limit=2";

/*
char golemioToken1  [MAX_GOLEMIO_TOKEN_LEN+1]= "?minutesBefore=1&minutesAfter=360&limit=1&mode=departures&filter=none&order=real&skip=canceled&aswIds=548_2";
char golemioToken2  [MAX_GOLEMIO_TOKEN_LEN+1]= "?minutesBefore=1&minutesAfter=360&limit=1&mode=departures&filter=none&order=real&skip=canceled&aswIds=548_2";
*/

MenuItem myMenuItems [] =
{
    { "gop", "Parametry",   golemioParametry,    MAX_GOLEMIO_PARAMETRY_LEN}
    /*,
  { "go1", "Token",   golemioToken1,    MAX_GOLEMIO_TOKEN_LEN },
  { "go2", "Token",   golemioToken2,    MAX_GOLEMIO_TOKEN_LEN },
  */
};

uint16_t NUM_MENU_ITEMS = sizeof(myMenuItems) / sizeof(MenuItem);  //MenuItemSize;

#else

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;

#endif    //USE_DYNAMIC_PARAMETERS


#endif      //dynamicParams_h
