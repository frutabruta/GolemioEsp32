// tested with MH-ET LIVE ESP32 MiniKIT
//
/*
VCC - VCC
GND - GND
SDA - IO16
SCL - IO17
*/
/*
based on ESP_WiFiManager_Lite example
  ESP_WiFiManager_Lite (https://github.com/khoih-prog/ESP_WiFiManager_Lite) is a library 

*/


#include <Wire.h>


/////////////////////////////////////////  definice WifiMAnager
#include "defines.h"
#include "Credentials.h"
#include "dynamicParams.h"

ESP_WiFiManager_Lite *ESP_WiFiManager;

String poleDnu[] = { "", "Pondělí", "Úterý", "Středa", "Čtvrtek", "Pátek", "Sobota", "Neděle" };


////////////////////////////////////// definice Golemio
#ifdef ESP32
#include <SPIFFS.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

//MH-ET LIVE ESP32 MiniKIT
/*
#define D4 17 
#define D3 16 
*/

//DOIT ESP32

#define D4 21
#define D3 22

#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif



#include <ArduinoJson.h>




//#define USE_OLED 1
#define USE_LCD 1





#ifdef USE_LCD

#include "lcd.h"

#endif
int pocitacVterin = 30;


#ifdef USE_OLED

#include "oled.h"
//uprava knihovny: https://forum.hwkitchen.cz/viewtopic.php?t=2503
//Adafruit GFX:

#endif


char klic[350] = "XXX";


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

String idZastavky = "58791";  //58762 balabenka
String parametry = "";
char parametryC[200] = "?cisIds=58791&minutesBefore=1&minutesAfter=60&limit=30&mode=departures&includeMetroTrains=true&order=real";

String retezec = "";
bool filtrNeaktivni = true;


DynamicJsonDocument root(9000);

StaticJsonDocument<200> filter;



HTTPClient http;  //Declare an object of class HTTPClient
String celaAdresa = "https://api.golemio.cz/v2/pid/departureboards/";
WiFiClientSecure client;
////////////////////////////////////// funkce WifiManager

void setupFilter()
{
filter["departures"][0]["departure_timestamp"]["minutes"] = true;
filter["departures"][0]["trip"]["headsign"] = true;
filter["departures"][0]["route"]["short_name"] = true;
Serial.print("Velikost filtru:");
Serial.println(String(filter.size()));
}

void heartBeatPrint() {
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");  // H means connected to WiFi
  else {
    if (ESP_WiFiManager->isConfigMode())
      Serial.print("C");  // C means in Config Mode
    else
      Serial.print("F");  // F means not connected to WiFi
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
#define HEARTBEAT_INTERVAL 20000L
  // Print hearbeat every HEARTBEAT_INTERVAL (20) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0)) {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

#if USING_CUSTOMS_STYLE
const char NewCustomsStyle[] PROGMEM = "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}"
                                       "button{background-color:blue;color:white;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";
#endif


void setupManager() {
  Serial.println("void setupManager()");

  delay(200);

  Serial.print(F("\nStarting ESP_WiFi using "));
  Serial.print(FS_Name);
  Serial.print(F(" on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP_WIFI_MANAGER_LITE_VERSION);

#if USING_MRD
  Serial.println(ESP_MULTI_RESET_DETECTOR_VERSION);
#else
  Serial.println(ESP_DOUBLE_RESET_DETECTOR_VERSION);
#endif

  ESP_WiFiManager = new ESP_WiFiManager_Lite();

  String AP_SSID = "GolemioDisplay";
  String AP_PWD = "password";

  // Set customized AP SSID and PWD
  ESP_WiFiManager->setConfigPortal(AP_SSID, AP_PWD);

  // Optional to change default AP IP(192.168.4.1) and channel(10)
  //ESP_WiFiManager->setConfigPortalIP(IPAddress(192, 168, 120, 1));
  ESP_WiFiManager->setConfigPortalChannel(0);

#if USING_CUSTOMS_STYLE
  ESP_WiFiManager->setCustomsStyle(NewCustomsStyle);
#endif

#if USING_CUSTOMS_HEAD_ELEMENT
  ESP_WiFiManager->setCustomsHeadElement(PSTR("<style>html{filter: invert(10%);}</style>"));
#endif

#if USING_CORS_FEATURE
  ESP_WiFiManager->setCORSHeader(PSTR("Your Access-Control-Allow-Origin"));
#endif

  // Set customized DHCP HostName
  ESP_WiFiManager->begin(HOST_NAME);
  //Or use default Hostname "ESP32-WIFI-XXXXXX"
  //ESP_WiFiManager->begin();
}


////////////////////////////////////////////////////////// funkce golemio




void setupGolemio() {
  Serial.println("void setupGolemio()");

  Wire.begin(SDA, SCL);

#ifdef USE_OLED
  if (!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, true, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    /*  for (;;)
      ;  // Don't proceed, loop forever */
  }
  oled.clearDisplay();
  oled.setTextSize(1);               // Normal 1:1 pixel scale
  oled.setTextColor(SSD1306_WHITE);  // Draw white text
  oled.cp437(true);
  oled.clearDisplay();
#endif


#ifdef USE_LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
#endif




  Serial.println("DP3");
  Serial.println("displej bezi na SCL:" + String(SCL) + " SDA:" + String(SDA));
  Serial.println("displej bezi na SCL:" + String(D4) + " SDA:" + String(D3));
  Serial.println("DP4");


  Serial.println("connected...yeey :)");


  Serial.println("");
#ifdef USE_OLED Serial.println("WiFi connected");

  oled.clearDisplay();

  oledDrawStringFromLeft(0, 0, "IP address: ");
  oledDrawStringFromLeft(0, 10, WiFi.localIP().toString());
  oledDrawStringFromLeft(0, 20, " klic: " + String(klic));


  oled.display();
#endif

  delay(2000);

  Serial.println("pred padem");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("po padu");
  // Lets deal with the user config values

  // Copy the string value
  /*
 parametryC=myMenuItems[0].pdata;
  klic=myMenuItems[1].pdata;
*/




  strcpy(parametryC, myMenuItems[0].pdata);
  strcpy(klic, myMenuItems[1].pdata);
  strcat(klic, myMenuItems[2].pdata);
  /*
strncpy(klic, myMenuItems[0].pdata, sizeof(klic));
  strncpy(parametryC, myMenuItems[1].pdata, sizeof(parametryC));
 */
  /*
  strncpy(klic, custom_text_box.getValue(), sizeof(klic));
  strncpy(parametryC, custom_text_box2.getValue(), sizeof(parametryC));
*/
  Serial.println(parametryC);
  Serial.println(klic);

#ifdef USE_OLED
  oled.clearDisplay();
  oledDrawStringFromLeft(0, 0, "parametry: ");
  oledDrawStringFromLeft(0, 10, parametryC);
  oled.display();
#endif

  configTime(1 * 3600, 1 * 3600, "pool.ntp.org", "time.nist.gov");


  client.setInsecure();
  //celaAdresa += "?cisIds=" + idZastavky;
  celaAdresa += parametryC;
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
  oled.clearDisplay();
#endif


#ifdef USE_LCD
  lcd.clear();
#endif
}

void stahni() {

  if (WiFi.status() == WL_CONNECTED) {  //Check WiFi connection status



    // WiFiClientSecure *client = new WiFiClientSecure;
    //WiFiClientSecure *client = &client2;
    http.begin(client, celaAdresa);
    http.addHeader("X-Access-Token", klic);

    Serial.println(celaAdresa);


    int httpCode = http.GET();  //Send the request

    if (httpCode > 0) {  //Check the returning code

      Serial.println("kod: " + String(httpCode));
      //char[] payload = http.getStream() ;  //Get the request response payload
      // Serial.println(payload);            //Print the response payload

      const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
      Serial.println("capacity:" + String(capacity));
      //DynamicJsonDocument root(9000);



      
      String httpResult=http.getString();
      Serial.println(httpResult);


      //DeserializationError error = deserializeJson(root, http.getStream(), DeserializationOption::Filter(filter));
      DeserializationError error = deserializeJson(root, httpResult, DeserializationOption::Filter(filter));

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else
      {
        Serial.print("json capacity:");
        Serial.println(root.capacity());
      }

    

      clearDisplays();

      // oled.setTextAlignment(TEXT_ALIGN_LEFT);


#ifdef USE_LCD
      int lcdMaxPocetOdjezdu = 7;
#endif

#ifdef USE_OLED
      int oledMaxPocetOdjezdu = 5;

      int cisloRadkuInfo = 5;
      if (vyska32 == 1) {
        oledMaxPocetOdjezdu = 2;
        cisloRadkuInfo = 2;
      }
#endif




      int arraySize = root["departures"].size();
      Serial.println("pocet odjezdu:" + String(arraySize));

      int counter = 0;

      for (int i = 0; (i < arraySize); i++) {
        // Serial.println("pruchod "+String(i));

        String cas = root["departures"][i]["departure_timestamp"]["minutes"].as<const char *>();
        String linka = root["departures"][i]["route"]["short_name"].as<const char *>();
        String cil = root["departures"][i]["trip"]["headsign"].as<const char *>();
        Serial.println(linka + " " + cil + " " + cas);
        //  if ((linka == "133") || (linka == "908") || (linka == "909") || filtrNeaktivni)
        if ((linka == "133") || filtrNeaktivni)  //vyresit lepe!
          //if ((linka == "133") || filtrNeaktivni)  //vyresit lepe!
          //
          if (true) {
#ifdef USE_OLED
            if (counter < oledMaxPocetOdjezdu) {
              oledVykresliRadekOdjezdu(linka, cil, cas, counter);
            }
#endif

#ifdef USE_LCD
            if (counter < lcdMaxPocetOdjezdu) {
              lcdVykresliRadekOdjezdu(linka, cil, cas, counter);
            }
#endif

            counter++;
          }
      }




      String casPrikaz = "0:48";
      String den = "";

      //////// cas

      time_t rawtime;
      struct tm *timeinfo;
      time(&rawtime);



      timeinfo = localtime(&rawtime);

      filtrNeaktivni = !jeSpicka(timeinfo);



      //timeinfo.hour;
      // int minutOdRana=timeinfo.hour*60+timeinfo.minute;

      char buffer[80];

      //strftime(buffer, 80, "%Y%m%d",timeinfo);
      strftime(buffer, 80, "%d.%m.%g %R", timeinfo);

      int minutOdRana = 0;
      casPrikaz = buffer;
      strftime(buffer, 80, "%u", timeinfo);
      den = buffer;



#ifdef USE_LCD
      lcdVykresliCas();
#endif

      ////////konec casu

#ifdef ULE_OLED
      oledVykresliSpodniRadekDatum(casPrikaz, nahradISO8859(cisloDoDne(den.toInt())), cisloRadkuInfo);

      //oled.startscrollleft(6,7);
      oled.display();
#endif
    }

    http.end();  //Close connection
  }

  //delay(30000);  //Send a request every 30 seconds
}






String cisloDoDne(int vstup) {
  String vystup = "";


  if ((vstup < 0) || (vstup > 7)) {
    return "";
  }

  vystup = poleDnu[vstup];

  return vystup;
}




String nahradISO8859(String vstup) {
  vstup.replace("á", "\xE1");  //c hacek
  vstup.replace("Á", "\xC1");  //C hacek
  vstup.replace("č", "\xE8");  //c hacek
  vstup.replace("Č", "\xC8");  //C hacek
  vstup.replace("ď", "\xEF");  //d hacek
  vstup.replace("Ď", "\xCF");  //D hacek

  vstup.replace("é", "\xE9");  //

  vstup.replace("É", "\xC9");
  vstup.replace("ě", "\xEC");  //e hacek
  vstup.replace("Ě", "\xCC");
  vstup.replace("í", "\xED");  //dlouhe i
  vstup.replace("Í", "\xCD");  //dlouhe I
  vstup.replace("ň", "\xF2");
  vstup.replace("Ň", "\xD2");
  vstup.replace("ó", "\xF3");
  vstup.replace("Ó", "\xD3");
  vstup.replace("ř", "\xF8");
  vstup.replace("Ř", "\xD8");
  vstup.replace("š", "\xB9");
  vstup.replace("Š", "\xA9");
  vstup.replace("ť", "\xBB");
  vstup.replace("Ť", "\xAB");
  vstup.replace("ú", "\xFA");
  vstup.replace("Ú", "\xDA");
  vstup.replace("ů", "\xF9");
  vstup.replace("Ů", "\xD9");
  vstup.replace("ý", "\xFD");
  vstup.replace("Ý", "\xDD");
  vstup.replace("ž", "\xBE");
  vstup.replace("Ž", "\xAE");

  return vstup;
}














///////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("void setup()");
  setupManager();
  // Debug console


  ////////////////



  setupFilter();

  setupGolemio();
}

#if USE_DYNAMIC_PARAMETERS
void displayCredentials() {
  Serial.println(F("\nYour stored Credentials :"));

  for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++) {
    Serial.print(myMenuItems[i].displayName);
    Serial.print(F(" = "));
    Serial.println(myMenuItems[i].pdata);
  }
}

void displayCredentialsInLoop() {
  static bool displayedCredentials = false;

  if (!displayedCredentials) {
    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
      if (!strlen(myMenuItems[i].pdata)) {
        break;
      }

      if (i == (NUM_MENU_ITEMS - 1)) {
        displayedCredentials = true;
        displayCredentials();
      }
    }
  }
}

#endif

void loop() {
  ESP_WiFiManager->run();

  check_status();
  /*
#if USE_DYNAMIC_PARAMETERS
  displayCredentialsInLoop();
#endif*/

#ifdef USE_LCD
  lcdVykresliCas();
#endif

  if (pocitacVterin > 10) {
    stahni();
    pocitacVterin = 0;
  }
  pocitacVterin++;
  Serial.println("free heap:");
  Serial.println(ESP.getFreeHeap());
  Serial.print("pocet vterin: ");
  Serial.println(String(pocitacVterin));
  delay(1000);
}
