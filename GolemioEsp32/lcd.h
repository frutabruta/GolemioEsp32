
#ifdef USE_LCD
//LiquidCrystal_I2C lcd(0x27, 20, 4);  //modry 0x27
//LiquidCrystal_I2C lcd(0x3F, 20, 4); //zeleny 0x3F
#include <LCDI2C_Multilingual.h> //LiquicCrystal I2C MultiLingual by Loc P.LE 2.0.2
LCDI2C_Generic lcd(0x27, 20, 4);
const int lcdMaxPocetOdjezdu = 7;

void lcdVykresliCas() 
{
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);

  timeinfo = localtime(&rawtime);
  char bufferCas[20];
  strftime(bufferCas, 20, "%T", timeinfo);

  lcd.setCursor(12, 3);
  String jenCas = bufferCas;
  lcd.print(jenCas);
}

String doplnNaTriCislice(String vstup) 
{
  vstup.trim();
  while (vstup.length() < 3) 
  {
    vstup = " " + vstup;
  }
  return vstup;
}

void lcdVykresliRadekOdjezdu(String &linka, String &cil, String &cas, int radek) 
{
  //oled.setTextAlignment(TEXT_ALIGN_LEFT);
  if (radek < 7) {
    if (radek < 4) {
      lcd.setCursor(0, radek);

    } else {
      lcd.setCursor(11, radek - 4);
    }
    lcd.print(doplnNaTriCislice(linka) + "  " + doplnNaTriCislice(cas) + "m");
  }
}

void lcdVymazRadekOdjezdu(int radek) 
{
  //oled.setTextAlignment(TEXT_ALIGN_LEFT);
  if (radek < 7) 
  {
    if (radek < 4) 
    {
      lcd.setCursor(0, radek);

    } 
    else 
    {
      lcd.setCursor(11, radek - 4);
    }
    lcd.print("         ");
  }
}

void lcdSetTextPage(String line1, String line2, String line3, String line4)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  lcd.setCursor(0, 2);
  lcd.print(line3);
  lcd.setCursor(0, 3);
  lcd.print(line4);
}

void lcdPeriodicDisplayUpdate()
{
  
}


#endif
