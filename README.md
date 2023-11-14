# GolemioEsp32 #

Project to show Prague Integrated System (http://www.pid.cz) departures from GolemioAPI using ESP32 and 20x4 LCD display.
## Required libraries ##

ESP_WiFiManager_Lite
https://github.com/khoih-prog/ESP_WiFiManager_Lite


Arduino-LiquidCrystal-I2C-library 1.1.2
https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
LiquidCrystal_I2C.h


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Modification of adafruit gfx to support czech characters on 128x64 OLED screen:
https://forum.hwkitchen.cz/viewtopic.php?t=2503


## Setup ##

- upload code
- generate Golemio API key at  api.golemio.cz/api-keys
- double press the reset button on the ESP32
- connect to WI-FI hotspot created by ESP32 
    - password and SSID should be shown on the display
    - GolemioDisplay, password
- open the Configuration page
    - enter your WI-FI credentials
    - modify Golemio request parameters according to https://api.golemio.cz/pid/docs/openapi/
    - split the API key in two equally long parts and enter them to the configuration pagen
    - click on Save


## Changelog ##
- 20231114_1802
  - modification of documentation
  

