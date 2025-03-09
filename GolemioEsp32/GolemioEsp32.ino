//tzapu WifiManagerVerion
//ssd1309
//u8g2

String version="20250309_1601";

// tested with MH-ET LIVE ESP32 MiniKIT
//
/*
VCC - VCC
GND - GND
SDA - IO16
SCL - IO17


DOIT ESP32 DEV KIT
VCC - VIN
GND - GND
SDA - D21
SCL - D22


NodeMCU ESP8266 - now unsupported, works only for 2 departures (cannot decode gzip) 
VCC - 3V3
GND - GND
SDA - D2
SCL - D1
*/


/*
LaskaKit ESP32-C3-LPKit
Set ESP32C3 Dev Module in Arduino IDE
Enable USB CDC on boot
use default partition scheme with spiffs

Enable Erase all flash before  upload to solve the partition issue

to flash enter bootloader - hold FLASH button, shortly press RESET, release FLASH button


replug board after upload 

to update OTA  use .ino.bin file (approx. 1,23 MByte in size)
*/


/** BASED ON
 * WiFiManager advanced demo, contains advanced configurartion options
 * Implements TRIGGEN_PIN button press, press for ondemand configportal, hold for 3 seconds for reset settings.
 */
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
// wifimanager can run in a blocking mode or a non blocking mode
// Be sure to know how to process loops with no delay() if using non blocking
bool wm_nonblocking = false;        // change to true to use non blocking
               // global wm instance
  WiFiManager wm;  



#include "configuration.h"
#include "setTime.h"



#ifdef MHET
  #define TRIGGER_PIN 27 //MH ET LIVE ESP32Minikit  
  #define SDA 16 
  #define SCL 17 

#elif  LPKITC3
  #define SDA 8
  #define SCL 10
  #define TRIGGER_PIN 9 
  #define USUP_POWER 4   

#elif  ESPWLED
  #define SDA 10
  #define SCL 8
  #define TRIGGER_PIN 9 

#elif METEOMINIC3
  #define SDA 19
  #define SCL 18
  #define TRIGGER_PIN 8 //DOIT ESP32 DEVKIT
  #define USUP_POWER 3   

#elif ESPLAN  
  #define SDA 33
  #define SCL 32
  #define TRIGGER_PIN 35

#elif CUSTOM
  #define SDA 21
  #define SCL 22
  #define TRIGGER_PIN 0

#else
  #define SDA 16 
  #define SCL 17 
  #define TRIGGER_PIN 0 //DOIT ESP32 DEVKIT
#endif





/* for future gzip usage
ESP32-targz Tobozo
1.2.3
https://github.com/tobozo/ESP32-targz
*/


/*
ArduinoJson Benoit Blanchon
7.1.0f

*/





#include <FS.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif


#include <Wire.h>
#include <ArduinoJson.h>


String poleDnu[] = { "", "pondělí", "úterý", "středa", "čtvrtek", "pátek", "sobota", "neděle" };


////////////////////////////////////// definice Golemio
#ifdef ESP32
#include <SPIFFS.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#endif



#ifdef USE_LCD
#include "lcd.h"
#endif

#ifdef USE_OLED
#include "oled.h"
#endif

#include "handleResponse.h"


//default attributes
String klic = "xxx";
String parametry = "?cisIds=58791&minutesBefore=1&minutesAfter=60&limit=5&mode=departures&includeMetroTrains=true&order=real";
String zakladAdresy = "https://api.golemio.cz/v2/pid/departureboards";  //public API
String celaAdresa = "";                                                    //public API

   WiFiManagerParameter paramApiKey("golemioApiKey", "golemioApiKeyText", klic.c_str(), 350);
  WiFiManagerParameter paramParameters("golemioParameters", "golemioParametersText",  parametry.c_str(), 200);


int sloupecLinka = 0;
int sloupecCil = 4;
int sloupecCas = 15;
int delkaCile = 30;
int delkaLinky = 3;
/*
const int spickaOd=7;
const int spickaDo=9;
*/
const int spickaOd = 7;
const int spickaDo = 9;

int pocitacVterin = 30;

String idZastavky = "58791";  //58762 balabenka

String wifiPortalName="GolemioSetup";
String wifiPortalPassword="password";

bool filtrNeaktivni = true;



//////////////// http client
//https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/



void heartBeatPrint() {
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");  // H means connected to WiFi
  else {
    if (false) {
      Serial.print("C");  // C means in Config Mode
      vypisChybuNaDispleje("config mode");
    }

    else {
      Serial.print("F");  // F means not connected to WiFi
      vypisChybuNaDispleje("wifi not connected");
      WiFi.reconnect();
    }
  }

  if (num == 80) {
    Serial.println();
    num = 1;
  } else if (num++ % 10 == 0) {
    Serial.print(F(" "));
  }
}

void check_status() {
  static unsigned long checkstatus_timeout = 0;



  //KH
#define HEARTBEAT_INTERVAL 1000L
  // Print WiFi hearbeat, Publish MQTT Topic every HEARTBEAT_INTERVAL (5) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0)) {
    if (WiFi.status() == WL_CONNECTED) {
#ifdef USE_LCD
      lcdVykresliCas();
#endif
#ifdef DEBUGGING
      Serial.println("free heap:");
      Serial.println(ESP.getFreeHeap());
      Serial.print("pocet vterin: ");
      Serial.println(String(pocitacVterin));
#endif
      if (pocitacVterin > 10) {
        stahni();
        pocitacVterin = 0;
      }
      pocitacVterin++;
    }

    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}


void checkButton() {
  // check for button press
  if (digitalRead(TRIGGER_PIN) == LOW) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if (digitalRead(TRIGGER_PIN) == LOW) {
      Serial.println("Button Pressed");
      vypisChybuNaDispleje("zmacknuto tlacitko");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000);  // reset delay hold
      if (digitalRead(TRIGGER_PIN) == LOW) {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        vypisChybuNaDispleje("mazu konfiguraci");
        wm.resetSettings();
        ESP.restart();
      }

      // start portal w delay
      Serial.println("Starting config portal");
      vypisChybuNaDispleje("startuju konfiguracni portal");
      setTextPage("starting portal","192.168.4.1",wifiPortalName,wifiPortalPassword);

      if (!wm.startConfigPortal(wifiPortalName.c_str(), wifiPortalPassword.c_str())) {
        Serial.println("failed to connect or hit timeout");
        setTextPage("wi-fi ","connection","failed","");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        //vypisChybuNaDispleje("wifi pripojena");
        setTextPage("IP address: ",WiFi.localIP().toString(),WiFi.SSID(),"");
        delay(2000);
      }
    }
  }
}



String timeToString() {
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);

  timeinfo = localtime(&rawtime);
  char bufferCas[20];
  strftime(bufferCas, 20, "%T", timeinfo);

  // Serial.println(String(timeinfo->tm_hour) + ":" + String(timeinfo->tm_min));


  String jenCas = bufferCas;
  return jenCas;
  // delete timeinfo;
}
////////////////////////////////////////////////////////// funkce golemio


void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setTextPage(String line1, String line2, String line3, String line4)
{
  #ifdef USE_OLED
   oledSetTextPage(line1,line2,line3,line4);
#endif

#ifdef USE_LCD
 lcdSetTextPage(line1,line2,line3,line4);
#endif 

}

void vypisChybuNaDispleje(String text) {
  printLocalTime();
  //Serial.println(timeToString() );

  setTextPage(text,"","","");


  Serial.println("error: " + text);
}


void setupDisplay()
{
  Serial.println("setupDisplay");


    Wire.begin(SDA, SCL);

#ifdef USE_OLED
  if (!oled.begin()) {
    Serial.println(F("SSD1306 allocation failed"));
    /*  for (;;)
      ;  // Don't proceed, loop forever */
  }
  oled.enableUTF8Print();
  
  oled.clearBuffer();
  oled.setFont(czfont9);
  oled.setFontDirection(0);
  oled.setCursor(0,9);
  oled.print("oled init");
  oled.sendBuffer();
                // Normal 1:1 pixel scale
 // oled.setTextColor(SSD1306_WHITE);  // Draw white text
//  oled.cp437(true);
 
#endif


#ifdef USE_LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
#endif  


setTextPage("golemio display","version",version,"");

}

void setupGolemio() {
  Serial.println("void setupGolemio()");

  Serial.println("SCL:" + String(SCL) + " SDA:" + String(SDA));

  


  delay(2000);

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println(parametry);

setTextPage( "parametry: ",parametry,"","");

  Serial.println(timeToString());

  //http.useHTTP10(true);
}

bool jeSpicka(tm *vstup) {
  int hodina = vstup->tm_hour;
  int den = vstup->tm_wday;

  if ((hodina >= spickaOd) && (hodina < spickaDo) && (den > 0) && (den < 6)) {
    return true;
  }
  return false;
}

void clearDisplays() {
#ifdef USE_OLED
  oled.clearBuffer();
#endif


#ifdef USE_LCD
  lcd.clear();
#endif
}



void stahni() {

  if (WiFi.status() == WL_CONNECTED) {  //Check WiFi connection status

#ifdef ESP8266
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
#endif

#ifdef ESP32
    NetworkClientSecure *client = new NetworkClientSecure;
#endif

    client->setInsecure();
    if (client) {
      HTTPClient http;
      celaAdresa = zakladAdresy+parametry;
      http.setTimeout(60000);

      http.begin(*client, celaAdresa);

      http.addHeader("X-Access-Token", klic);
      http.addHeader(F("Content-Length"), String(0));

#ifdef ESP32
      http.setAcceptEncoding("identity");
      // http.setConnectTimeout(1000);
#endif

#ifdef ESP8266
      http.addHeader("Accept-Encoding", "identity");  //doesn't work
#endif

      Serial.println(celaAdresa);


      int httpCode = http.GET();  //Send the request

      if (httpCode > 0) {
        switch (httpCode) {
          case 200:
            handleResponse(http);
            break;
          default:
            vypisChybuNaDispleje("chyba http: " + String(httpCode)+" kanal "+WiFi.channel());

            break;
        }

      } else {
        vypisChybuNaDispleje("chyba http: " + String(httpCode)+" kanal "+WiFi.channel());
      }

      http.end();  //Close connection
    }
#ifdef ESP32
    delete client;
#endif
  } else {
    vypisChybuNaDispleje("wifi nepripojeno");
  }

  //delay(30000);  //Send a request every 30 seconds
}








bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
  //ESP.restart();
}

void saveParamsCallback () {
  Serial.println("Should save params");
  shouldSaveConfig = true;
  klic = paramApiKey.getValue();
  parametry = paramParameters.getValue();
  saveSpiffs();
  //ESP.restart();
}





void setupSpiffs() {
  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin(true)) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        JsonDocument json;
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if (!deserializeError) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) 
        {
#endif
          Serial.println("\nparsed json");
          klic = String(json["golemio_api_key"]);
          parametry = String(json["golemio_parameters"]);
          

        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    } 
  } else {
    Serial.println("failed to mount FS");
  }
}

void saveSpiffs() {
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    JsonDocument json;
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif


    json["golemio_api_key"] = klic;
    json["golemio_parameters"] = parametry;    


    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    //ESP.restart();
    //end save
  }
}


void setupManager() {
 // WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP
   wm.setConfigPortalTimeout(120);
   wm.setBreakAfterConfig(true);
  Serial.println("\n Starting");

  
  //if (wm_nonblocking) wm.setConfigPortalBlocking(false); 


  //set config save notify callback
  wm.setSaveConfigCallback(saveConfigCallback); 
  wm.setSaveParamsCallback(saveParamsCallback);

  paramApiKey.setValue(klic.c_str(),350);
  paramParameters.setValue(parametry.c_str(),200);  
  wm.addParameter(&paramApiKey);
  wm.addParameter(&paramParameters);

  // set dark theme
  wm.setClass("invert");

vypisChybuNaDispleje("connecting");
 setTextPage("starting portal","192.168.4.1",wifiPortalName,wifiPortalPassword);
 if (!wm.autoConnect(wifiPortalName.c_str(), wifiPortalPassword.c_str())) 
 {
    Serial.println("failed to connect and hit timeout");
    vypisChybuNaDispleje("spojeni se nezdařilo");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  vypisChybuNaDispleje("připojeno");
  klic = paramApiKey.getValue();
  parametry = paramParameters.getValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {  

  Serial.begin(115200);
 // Serial.setDebugOutput(true);
  Serial.println("void setup()");
  
     #ifdef LPKITC3
  pinMode(USUP_POWER, OUTPUT); 
  digitalWrite(USUP_POWER, HIGH); // enable power supply for uSup
   Serial.println("usup turned on");
  #endif
  
    #ifdef METEOMINIC3
  pinMode(USUP_POWER, OUTPUT); 
  digitalWrite(USUP_POWER, HIGH); // enable power supply for uSup
   Serial.println("usup turned on");
  #endif
    
 pinMode(TRIGGER_PIN, INPUT);
  #ifdef ESPLAN
pinMode(TRIGGER_PIN, INPUT_PULLUP); 
  #endif

   #ifdef MHET
  pinMode(TRIGGER_PIN, INPUT_PULLUP);  
  #endif

  setupDisplay();
  setupSpiffs();
  setupManager();   

  Serial.println("The values in the file are: ");
  Serial.println("\tapi key : " + klic);
  Serial.println("\tparametry: " + parametry);

  saveSpiffs();

  setupGolemio();
  clearDisplays();

  //NTP setup
  initTime("CET-1CEST,M3.5.0,M10.5.0/3"); //Prague TimeZone

}


void loop() {
  checkButton();
  // put your main code here, to run repeatedly:
  check_status();
}
