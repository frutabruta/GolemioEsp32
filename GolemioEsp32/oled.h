

#ifdef USE_OLED
#include <U8g2lib.h>



  #ifdef MEGAOLED
    #include "ZIS_12_normal.h"
    #include "ZIS_12_bold.h"
 //   #include "ZIS_17_normal.h"
    #include "ZIS_17_bold.h"
    
static const unsigned char CIRCLE_XBM[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00,
  0x00, 0xF0, 0x03,
  0x00, 0xFC, 0x0F,
  0x00, 0xFE, 0x1F,
  0x00, 0xFF, 0x3F,
  0x80, 0xFF, 0x7F,
  0x80, 0xFF, 0x7F,
  0xC0, 0xFF, 0xFF,
  0xC0, 0xFF, 0xFF,
  0xC0, 0xFF, 0xFF,
  0xC0, 0xFF, 0xFF,
  0xC0, 0xFF, 0xFF,
  0xC0, 0xFF, 0xFF,
  0x80, 0xFF, 0x7F,
  0x80, 0xFF, 0x7F,
  0x00, 0xFF, 0x3F,
  0x00, 0xFE, 0x1F,
  0x00, 0xFC, 0x0F,
  0x00, 0xF0, 0x03
};


const uint8_t CIRCLE_WIDTH  = 24;
const uint8_t CIRCLE_HEIGHT = 19;


  #else
    #include "czfonts.h"
  #endif


#ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
#endif
  #ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif





//#define SCREEN_WIDTH 128     // OLED display width, in pixels
//#define SCREEN_HEIGHT 64     // OLED display height, in pixels
//#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
//#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


;


#ifdef MEGAOLED
U8G2_SSD1363_256X128_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);
#else
U8G2_SSD1309_128X64_NONAME0_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE ,SCL,SDA)
#endif
//U8G2_SSD1363_256X128_F_2ND_HW_I2C(U8G2_R0, U8X8_PIN_NONE);


#define BIGOLED 1 // fixed with u8g2 if using SSD1309 2.42" from LaskaKit to replace Czech characters


const int vyska32 = 0;

const int infoStartPosition=98;

String nahradISO8859(String vstup) 
{
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

String nahradDiakritiku(String vstup) 
{
  vstup.replace("á", "a");  //c hacek
  vstup.replace("Á", "A");  //C hacek
  vstup.replace("č", "c");  //c hacek
  vstup.replace("Č", "C");  //C hacek
  vstup.replace("ď", "d");  //d hacek
  vstup.replace("Ď", "D");  //D hacek

  vstup.replace("é", "e");  //

  vstup.replace("É", "E");
  vstup.replace("ě", "e");  //e hacek
  vstup.replace("Ě", "E");
  vstup.replace("í", "i");  //dlouhe i
  vstup.replace("Í", "I");  //dlouhe I
  vstup.replace("ň", "n");
  vstup.replace("Ň", "N");
  vstup.replace("ó", "o");
  vstup.replace("Ó", "O");
  vstup.replace("ř", "r");
  vstup.replace("Ř", "R");
  vstup.replace("š", "s");
  vstup.replace("Š", "S");
  vstup.replace("ť", "t");
  vstup.replace("Ť", "T");
  vstup.replace("ú", "u");
  vstup.replace("Ú", "U");
  vstup.replace("ů", "u");
  vstup.replace("Ů", "U");
  vstup.replace("ý", "y");
  vstup.replace("Ý", "Y");
  vstup.replace("ž", "z");
  vstup.replace("Ž", "Z");

  return vstup;
}

String  cisloDoDne(int vstup) 
{
  String vystup = "";


  if ((vstup < 0) || (vstup > 7)) {
    return "";
  }

  #ifdef BIGOLED
 // vystup= nahradDiakritiku(poleDnu[vstup]);
 vystup=poleDnu[vstup];
  #else
  vystup= nahradISO8859(poleDnu[vstup]);
  #endif


  return vystup;
}



void oledDrawCentreString(String &buf, int x, int y) 
{
  int16_t x1, y1;
  uint16_t w, h;
 // oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  w=oled.getStrWidth(buf.c_str());
  oled.setCursor(x - w / 2, y);
  oled.print(buf);
}

void oledDrawStringFromRight(int x, int y, String &buf, bool fill) 
{
  int16_t x1, y1;
  uint16_t w, h;
  w=oled.getStrWidth(buf.c_str());
  if (fill) {
    buf = " " + buf;
  //  oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
 //   oled.fillRect(x - w, y, w, h, SSD1306_BLACK);
  } else {
  //  oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  }

  oled.setCursor(x - w, y);
  oled.print(buf);
}
void oledDrawStringFromLeft(int sloupec, int radek, String obsah) 
{
  oled.setCursor(sloupec, radek);
  oled.println(obsah);
}

void oledVykresliRadekOdjezdu(String linka, String cil, String cas, int radek) 
{
  #ifdef MEGAOLED
  int sloupecCile = 25;
  int sloupecLinky = 3;
  int vyskaRadku = 14;
  int sloupecCasu = 252;
  int pravyOkrajCile = 228;
  int maxSirkaTextu = sloupecCile - pravyOkrajCile;
  int offsetRadku=37;
  oled.setFont(ZIS_12_bold);
  #else
  int sloupecCile = 25;
  int sloupecLinky = 0;
  int vyskaRadku = 10;
  int sloupecCasu = 125;
  int pravyOkrajCile = 100;
  int maxSirkaTextu = sloupecCile - pravyOkrajCile;
  int offsetRadku=10;
  #endif

  oledDrawStringFromLeft(sloupecLinky, radek * vyskaRadku+offsetRadku, linka);
  #ifdef BIGOLED
 // oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku, nahradDiakritiku(cil).substring(0, 17));
  oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku+offsetRadku, cil.substring(0, 23));
  #else  
  oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku+offsetRadku, nahradISO8859(cil).substring(0, 23));
  #endif


  if(cas.length()<2)
  {
    cas=" "+cas;
  }
  oledDrawStringFromRight(sloupecCasu, radek * vyskaRadku+offsetRadku, cas, true);
  
}

void oledVykresliRadekOdjezdu(String linka, String cil, String cas, int radek, bool accessible, bool airConditioned) 
{
  #ifdef MEGAOLED
  int sloupecCile = 53;
  int sloupecLinky = 3;
  int sloupecVozicku = 25;
  int sloupecVlocky = 39;
  int vyskaRadku = 14;
  int sloupecCasu = 252;
  int pravyOkrajCile = 228;
  int maxSirkaTextu = sloupecCile - pravyOkrajCile;
  int offsetRadku=37;
  oled.setFont(ZIS_12_bold);

if(accessible)
{
  oledDrawStringFromLeft(sloupecVozicku, radek * vyskaRadku+offsetRadku, "♿");
}

if(airConditioned)
{
  oledDrawStringFromLeft(sloupecVlocky, radek * vyskaRadku+offsetRadku, "❄");
}
  
  


  #else
  int sloupecCile = 25;
  int sloupecLinky = 0;
  int vyskaRadku = 10;
  int sloupecCasu = 125;
  int pravyOkrajCile = 100;
  int maxSirkaTextu = sloupecCile - pravyOkrajCile;
  int offsetRadku=10;
  #endif

  oledDrawStringFromLeft(sloupecLinky, radek * vyskaRadku+offsetRadku, linka);
  #ifdef BIGOLED
 // oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku, nahradDiakritiku(cil).substring(0, 17));
  oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku+offsetRadku, cil.substring(0, 23));
  #else  
  oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku+offsetRadku, nahradISO8859(cil).substring(0, 23));
  #endif


  if(cas.length()<2)
  {
    cas=" "+cas;
  }
  oledDrawStringFromRight(sloupecCasu, radek * vyskaRadku+offsetRadku, cas, true);
  
}

void oledVykresliSpodniRadekDatum(String &cas, String den, int radek) 
{

  #ifdef MEGAOLED
  int vyskaRadku = 10;
  int sloupecCas = 255;
  int posunPc = 0;
  int posunNc = 0;
  int posun = 13;
  int datumOffset=126;
  oled.setFont(ZIS_12_normal);
  oled.drawLine(0, datumOffset-12, 255, datumOffset-12);
  oledDrawStringFromLeft(0, datumOffset, den);
  oledDrawStringFromRight(sloupecCas, datumOffset, cas, false);   
  #else
  int vyskaRadku = 10;
  int sloupecCas = 128;
  int posunPc = 0;
  int posunNc = 0;
  int posun = 13;
  oled.drawLine(0, 52, 127, 52);
  oledDrawStringFromLeft(0, radek * vyskaRadku + posun, den);
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);   
  #endif
}

void oledVykresliHlavicku(String nastupiste, String nazev)
{
  #ifdef MEGAOLED
  if(nastupiste!="")
  {
  oled.setFont(ZIS_17_bold);  
  oled.drawLine(0, 20, 255, 20);
  oledDrawStringFromLeft(53, 16, nazev);

  oled.setDrawColor(1); 
  oled.drawXBM(0, 0, CIRCLE_WIDTH, CIRCLE_HEIGHT, CIRCLE_XBM);
  oled.setDrawColor(0); 
  oledDrawStringFromLeft(12, 16, nastupiste);
  oled.setDrawColor(1); 
  }
  else
  {
  oled.setFont(ZIS_17_bold);  
  oled.drawLine(0, 20, 255, 20);
  oledDrawStringFromLeft(3, 16, nazev);
  }
  #endif
}

void oledVykresliSpodniRadekInfotext(String &cas, String infotext,int &infotextOffset, int radek) 
{
  #ifdef MEGAOLED
  int sloupecCas = 255;
  int datumOffset=126;
  oled.setFont(ZIS_12_normal);
  oled.drawLine(0, datumOffset-12, 255, datumOffset-12);
  int posun = 13;
  oled.setDrawColor(0);
  oled.drawBox(0, datumOffset-11, 256, 15);
  oled.setDrawColor(1);    
  oled.setClipWindow(0, datumOffset-12, sloupecCas-30, 127); 
  oledDrawStringFromLeft(0-infotextOffset, datumOffset, infotext);
  oled.setMaxClipWindow();
  oledDrawStringFromRight(sloupecCas, datumOffset, cas, false);

  #else
  int vyskaRadku = 10;
  int sloupecCas = 128;
  oled.drawLine(0, 52, 127, 52);
  oledDrawStringFromLeft(0, radek * vyskaRadku + posun, den);
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);   
  int posun = 13;
  oled.setDrawColor(0);
  oled.drawBox(0, 50, 128, 15);
  oled.setDrawColor(1);    
  oled.setClipWindow(0, 52, sloupecCas-30, 64); 
  oledDrawStringFromLeft(0-infotextOffset, radek * vyskaRadku + posun, infotext);
  oled.setMaxClipWindow();
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);
  #endif   
}



void oledVykresliSpodniRadek(String &cas, int aktStranka, int pocetStranek, int radek) 
{

  int vyskaRadku = 10;
  int sloupecCas = 128;
  int posunPc = 0;
  int posunNc = 0;
  int posun = 3;

  int y0 = 64 - vyskaRadku - posun;
  if (vyska32 == 1) {
    posun = 1;
    y0 = 32 - vyskaRadku - posun + 1;
  }




  // oled.setTextAlignment(TEXT_ALIGN_LEFT);
 // oledDrawStringFromLeft(0, radek * vyskaRadku + posun, String(aktStranka) + "/" + String(pocetStranek));

 oledDrawStringFromLeft(0, radek * vyskaRadku + posun, String(aktStranka) + "/" + String(pocetStranek));


  // oled.setTextAlignment(TEXT_ALIGN_RIGHT);
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);
  oled.drawLine(0, 50, 127, 50);
}


void oledSetTextPage(String line1, String line2, String line3, String line4)
{
    oled.clearBuffer();
  oledDrawStringFromLeft(0, 10, line1);
  oledDrawStringFromLeft(0, 20, line2);
  oledDrawStringFromLeft(0, 30, line3);
  oledDrawStringFromLeft(0, 40, line4);
    oled.sendBuffer();
}

void oledPeriodicDisplayUpdate()
{
   String casPrikaz = "0:00";
  String den = "";

  //////// cas

  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);

  timeinfo = localtime(&rawtime);



  //timeinfo.hour;
  // int minutOdRana=timeinfo.hour*60+timeinfo.minute;

  char buffer[80];

  char bufferDatum[80];
  char bufferCas[80];

  //strftime(buffer, 80, "%Y%m%d",timeinfo);
  strftime(bufferDatum, 80, "%d.%m.%Y", timeinfo);
  strftime(bufferCas, 80, "%R", timeinfo);

  int minutOdRana = 0;
  casPrikaz = bufferCas;
  strftime(buffer, 80, "%u", timeinfo);
  den = buffer;
  
    int cisloRadkuInfo = 5;


 if(infotextGlobalVariable=="")
 {
  oledVykresliSpodniRadekDatum(casPrikaz, cisloDoDne(den.toInt())+String(" ")+bufferDatum, cisloRadkuInfo);
 }
 else
 {
  oledVykresliSpodniRadekInfotext(casPrikaz,infotextGlobalVariable,infotextOffset,cisloRadkuInfo);
  infotextOffset++;
  int textWidth=oled.getStrWidth(infotextGlobalVariable.c_str());
  
  if(infotextOffset>textWidth)
  {
    infotextOffset=-infoStartPosition;
  }
 }


  


  oled.sendBuffer();
}

#endif
