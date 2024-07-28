
#ifdef USE_OLED

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//uprava knihovny: https://forum.hwkitchen.cz/viewtopic.php?t=2503
//Adafruit GFX:


#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int vyska32 = 0;



void oledVykresliRadekOdjezdu(String &linka, String &cil, String &cas, int radek) {
  int sloupecCile = 20;
  int vyskaRadku = 10;
  int sloupecCasu = 128;
  int pravyOkrajCile = 100;
  int maxSirkaTextu = sloupecCile - pravyOkrajCile;

  oledDrawStringFromLeft(0, radek * vyskaRadku, linka);
  oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku, nahradISO8859(cil).substring(0, 17));
  oledDrawStringFromRight(sloupecCasu, radek * vyskaRadku, cas, true);
}


void oledDrawCentreString(String &buf, int x, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  oled.setCursor(x - w / 2, y);
  oled.print(buf);
}

void oledDrawStringFromRight(int x, int y, String &buf, bool fill) {
  int16_t x1, y1;
  uint16_t w, h;
  if (fill) {
    buf = " " + buf;
    oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
    oled.fillRect(x - w, y, w, h, SSD1306_BLACK);
  } else {
    oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  }

  oled.setCursor(x - w, y);
  oled.print(buf);
}

void oledVykresliSpodniRadekDatum(String &cas, String den, int radek) {

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

  oled.drawLine(0, y0, sloupecCas, y0, SSD1306_WHITE);

  //oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oledDrawStringFromLeft(0, radek * vyskaRadku + posun, den);



  //oled.setTextAlignment(TEXT_ALIGN_RIGHT);
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);
}

void oledDrawStringFromLeft(int sloupec, int radek, String obsah) {
  oled.setCursor(sloupec, radek);
  oled.println(obsah);
}


void oledVykresliSpodniRadek(String &cas, int aktStranka, int pocetStranek, int radek) {

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

  oled.drawLine(0, y0, sloupecCas, y0, SSD1306_WHITE);

  // oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oledDrawStringFromLeft(0, radek * vyskaRadku + posun, String(aktStranka) + "/" + String(pocetStranek));



  // oled.setTextAlignment(TEXT_ALIGN_RIGHT);
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);
}

#endif