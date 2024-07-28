
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

String  cisloDoDne(int vstup) {
  String vystup = "";


  if ((vstup < 0) || (vstup > 7)) {
    return "";
  }

  vystup = poleDnu[vstup];

  return vystup;
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
void oledDrawStringFromLeft(int sloupec, int radek, String obsah) {
  oled.setCursor(sloupec, radek);
  oled.println(obsah);
}

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