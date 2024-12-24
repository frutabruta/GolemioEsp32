# GolemioEsp32 #

Project to show Prague Integrated System (http://www.pid.cz) departures from GolemioAPI using ESP32 and 20x4 LCD display. This version requires an API key.
Tested board is DO-IT ESP32 DEVKIT V1.
ESP8266 is untested now ( probably doesn't work with more than two departures (problem with gzip encoding in golemio API).

[<img src="images/lcd20x4.jpg?raw=true" width="300px"/>](images/lcd20x4.jpg?raw=true "20x4 LCD screen")
[<img src="images/oled128x64.jpg" width="300px"/>](images/oled128x64.jpg?raw=true "128x64 OLED screen")
## Required libraries ##

WiFiManager by tzapu
https://github.com/tzapu/WiFiManager/tree/master

Arduino-LiquidCrystal-I2C-library 1.1.2
https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
LiquidCrystal_I2C.h



LiquidCrystal I2C MultiLingual by Loc P.LE 2.0.2
https://github.com/locple/LCDI2C_Multilingual


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Modification of adafruit gfx to support czech characters on 128x64 OLED screen:
https://forum.hwkitchen.cz/viewtopic.php?t=2503




## Setup ##
- NEEDS TO BE MODIFIED
- modify the code
    - to use 20x4 LCD uncomment ```#define USE_LCD 1```
    - to use 128x64 OLED uncomment ```#define USE_OLED 1```

- upload code

- connect to WI-FI hotspot created by ESP32 
    - password and SSID should be shown on the display
    - GolemioDisplay, password
- open the Configuration page
    - enter your WI-FI credentials
    - modify Golemio request parameters according to https://api.golemio.cz/pid/docs/openapi/
    - add Golemio API key
    - click on Save
- if you want to change parameters in the future, press the FLASH/BOOT button on the ESP32 or external button to create Wi-Fi hotspot again

- Wireless update using tzapu webmanager works
    - export binary using Arduino IDE - Sketch -> Export compiled binary
        - it is the sketch_folder/build/board_name/GolemioEsp32.ino.bin
    - start webportal on ESP by pressing a button
    - Android
        - transport compliled binary to the phone/tablet
        - connect to GolemioSetup Wi-Fi
        - on the captive portal select three dots - use network as is
        - in web browser navigate to 192.168.4.1
        - select updatee in menu
        - wait for update finished message



## Changelog ##
- 20241224_1935
    - migrated OLED library to u8g2
    - now supports czech language on SSD1309 2.42inch OLED display from LaskaKit 
    - added custom font for display
- 20241224_1532
    - added LaskaKit ESP32-C3-LPKit support
    - readme.md manual modification
    - wifiName changed to GolemioSetup
    - LPkit enable uSup power after boot

- 20241215_1911
    - migrated to tzapu wifi manager, now stable
    - added golemio key back, because even publci API now requires API key
- 20241111_2150
    - migrated all files back to ESP_WiFiManager_Lite
- 20240803_1310
    - wifi not connected on display
    - overwriting unused rows when numbr of departures is smaller than number of available rows
- 20240728_2250
    - can be compiled to ESP8266, works only with 2 departures
- 20240728_2135
    - added images to description
- 20240728_2059
    - fix OLED screen clear code
- 20240728_2001
    - code cleanup
- 20240714_1205 
    - first running example of key-less public API
- 20240714_1025
    - added constant to disable debugging output
    - LCD
        - removed display blinking on refresh
- 20240713_2302
    - basic version working with DO-IT ESP32 without crashing
    - swapped LCD library
    - OLED and LCD parts are separated by '#ifdef'
- 20231114_1802
    - modification of documentation
  

