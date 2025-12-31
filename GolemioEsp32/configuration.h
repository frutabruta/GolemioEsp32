
//SELECT BOARD



//LaskaKit ESP32-C3-LPkit - uSup connector doesn't provide enough power 
//#define LPKITC3 1

//LaskaKit ESP LAN - solder the I2C jumper
//#define ESPLAN 1 

//LaskaKit Meteo Mini
//#define METEOMINIC3 1

//LaskaKit ESPWLED (C3)
#define ESPWLED 1 

//MH-ET LIVE ESP32 MiniKIT 
//#define MHET 1

//#define CUSTOM 1 //Do-IT ESP32 with LCD display
//uncomment to enable debug messages
//#define DEBUGGING 1


//DISPLAY SETUP SECTION

//uncomment to use 128x64 I2C OLED on address 0x3C with SSD1306 driver
#define USE_OLED 1

// not used, will configure oled driver type
#define BIGOLED 1  

//also uncomment so use 256x128 with SSD1363 driver
//#define MEGAOLED 1

//uncomment to use 20x4 I2C LCD on address 0x3F
//#define USE_LCD 1

//future LCD adress selection
//LiquidCrystal_I2C lcd(0x27, 20, 4);  //modry 0x27
//LiquidCrystal_I2C lcd(0x3F, 20, 4); //zeleny 0x3F
