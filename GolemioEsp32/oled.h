#ifdef USE_OLED

#include <U8g2lib.h>
#include "conversions.h"

#ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
#endif

#ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
#endif

#ifdef MEGAOLED
  U8G2_SSD1363_256X128_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);
#else
  U8G2_SSD1309_128X64_NONAME0_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE ,SCL,SDA);
#endif

#ifdef MEGAOLED
  #include "ZIS_12_normal.h"
  #include "ZIS_12_bold.h"
  //#include "ZIS_17_normal.h" unused
  #include "ZIS_17_bold.h"
  
  //circle bitmap
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



#ifdef MEGAOLED
  const int sloupecCile = 25;
  //const int sloupecNastupiste = 235;
  const int sloupecLinky = 23;
  const int vyskaRadku = 14;
  const int sloupecCasu = 254;
  const int pravyOkrajCile = 228;
  const int offsetRadku=37;
  const int sloupecVozicku = 26;
  const int sloupecVlocky = 39;
  const int sloupecNastupiste = 231;
  const int sloupecSmeru=220;
  const int maxSirkaTextu = sloupecCile - pravyOkrajCile;    
  const int oledMaxPocetOdjezdu = 6;
  const int maxDelkaRadku=256;
#else
  const int sloupecCile = 25;
  const int sloupecLinky = 18;
  const int vyskaRadku = 10;
  const int sloupecCasu = 128;
  const int pravyOkrajCile = 100;
  const int maxSirkaTextu = sloupecCile - pravyOkrajCile;
  const int offsetRadku=10;
  const int oledMaxPocetOdjezdu = 5;
  const int maxDelkaRadku=128;
#endif


const int infoStartPosition=98;



void oledDrawCentreString(String &buf, int x, int y) 
{
  int16_t x1, y1;
  uint16_t w, h;
 // oled.getTextBounds(buf, x, y, &x1, &y1, &w, &h);  //calc width of new string
  w=oled.getStrWidth(buf.c_str());
  oled.setCursor(x - w / 2, y);
  oled.print(buf);
}

/*
void oledDrawStringFromRight(int x, int y, const String& buf, bool fill)
{
  const char* s = buf.c_str();

  // Measure UTF-8 string with the same decoder used by drawUTF8
  uint16_t w = oled.getUTF8Width(s);

  // Optional background fill – use u8g2 drawBox (if oled is U8g2)
  if (fill) {
    oled.setDrawColor(0);
    // Example: clear a 12 px tall area behind the text
    // (replace 12 with your font's ascent+descent if needed)
    oled.drawBox(x - w, y - 12, w, 12);
    oled.setDrawColor(1);
    
  }
  oled.drawUTF8(x - w, y, s);
}
*/

void oledDrawStringFromRight( int x, int y, const String &buf, bool fill, uint8_t pad = 0)
{
  const char* s = buf.c_str();

  // Measure width using UTF-8 decoder (must match drawUTF8)
  uint16_t w = oled.getUTF8Width(s);

  // Baseline metrics
  int ascent  = oled.getAscent();   // positive
  int descent = oled.getDescent();  // usually negative in u8g2

  // Compute box covering the text line
  int box_x = x - w - pad;                  // left edge of the text minus padding
  int box_y = y - ascent - pad;             // top: baseline - ascent - padding
  int box_w = w + 2 * pad;                  // text width + padding on both sides
  int box_h = (ascent - descent) + 2 * pad; // ascent - (negative descent) + padding

  if (fill) 
  {
    // Clear background behind text (draw color 0 = background)
    oled.setDrawColor(0);
    oled.drawBox(box_x, box_y, box_w, box_h);
    // Restore draw color to foreground for text
    oled.setDrawColor(1);
  }

  // Finally draw the text right-aligned at (x - w, baseline y)
  oled.drawUTF8(x - w, y, s);
}


void oledDrawStringFromLeft(int sloupec, int radek, String obsah) 
{
  oled.drawUTF8(sloupec,radek, obsah.c_str());
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

#ifdef MEGAOLED
  void oledVykresliRadekOdjezduMega(String linka, String cil, String cas, int radek, bool accessible, bool airConditioned, String platform="", String direction="") 
  {
    const int sloupecCile = 53;
    oled.setFont(ZIS_12_bold);
    oledDrawStringFromRight(sloupecLinky, radek * vyskaRadku+offsetRadku, linka,false);

    if(accessible)
    {
      oledDrawStringFromLeft(sloupecVozicku, radek * vyskaRadku+offsetRadku, "♿");
    }

    if(airConditioned)
    {
      oledDrawStringFromLeft(sloupecVlocky, radek * vyskaRadku+offsetRadku, "❄");
    }

    if(platform!="")
    {
      oledDrawStringFromRight(sloupecNastupiste, radek * vyskaRadku+offsetRadku, platform, true);
    }
    
    if(!direction.isEmpty())
    {
      oledDrawStringFromLeft(sloupecSmeru,radek * vyskaRadku+offsetRadku,dirToArrow(direction));
    }
    
    oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku+offsetRadku, cil.substring(0, 23));
    oledDrawStringFromRight(sloupecCasu, radek * vyskaRadku+offsetRadku, cas, true);  
  }

#else
  void oledVykresliRadekOdjezdu(String linka, String cil, String cas, int radek) 
  {
    oled.setFont(czfont9);
    oledDrawStringFromRight(sloupecLinky, radek * vyskaRadku+offsetRadku, linka,true);
    oledDrawStringFromLeft(sloupecCile, radek * vyskaRadku+offsetRadku, cil.substring(0, 30));
    oledDrawStringFromRight(sloupecCasu, radek * vyskaRadku+offsetRadku, cas, true);  
  }
#endif

void oledVykresliSpodniRadekDatum(String &cas, String den, int radek, bool vykresliDvojtecku=true) 
{

  #ifdef MEGAOLED
  const int vyskaRadku = 10;
  const int sloupecCas = 255;
  const int datumOffset=126;
  oled.setFont(ZIS_12_normal);
  oled.drawLine(0, datumOffset-12, 255, datumOffset-12);
  oled.setDrawColor(0);
  oled.drawBox(0, datumOffset-11, 256, 15);
  oled.setDrawColor(1);
  oledDrawStringFromLeft(0, datumOffset, den);
  oledDrawStringFromRight(sloupecCas, datumOffset, cas, false);   
  if(vykresliDvojtecku)
  {
    String colon=":";
    oledDrawStringFromRight(sloupecCas-13, datumOffset,colon, false);   
  }
  #else
  const int vyskaRadku = 10;
  const int sloupecCas = 128;
  const int datumOffset=63;
  oled.setDrawColor(0);
  oled.drawBox(0, datumOffset-11, 128, 12); //FIX!!!
  oled.setDrawColor(1);
  oled.drawLine(0, 52, 127, 52);

/*
  for(int i=0;i<128;i++)
      {
        if((i%2)==0)
        {
          oled.drawPixel(i,52);
        }
   
      }
*/

  oledDrawStringFromLeft(0, datumOffset, den);
  oledDrawStringFromRight(sloupecCas, datumOffset, cas, true);   
  if(vykresliDvojtecku)
  {
    String colon=":";
    oledDrawStringFromRight(sloupecCas-13, datumOffset,colon, true);     
  }
  #endif
}

void oledVykresliSpodniRadekInfotext(String &cas, String infotext,int &infotextOffset, int radek, bool vykresliDvojtecku=true) 
{
  #ifdef MEGAOLED
  const int sloupecCas = 255;
  const int datumOffset=126;
  oled.setFont(ZIS_12_normal);
  oled.drawLine(0, datumOffset-12, 255, datumOffset-12);
  oled.setDrawColor(0);
  oled.drawBox(0, datumOffset-11, 256, 15);
  oled.setDrawColor(1);
  oled.setClipWindow(0, datumOffset-12, sloupecCas-30, 127); 
  oledDrawStringFromLeft(0-infotextOffset, datumOffset, infotext);
  oled.setMaxClipWindow();
  oledDrawStringFromRight(sloupecCas, datumOffset, cas, false);   

  if(vykresliDvojtecku)
  {
    String colon=":";
    oledDrawStringFromRight(sloupecCas-14, datumOffset,colon, false);   
  }
  #else
  const int vyskaRadku = 10;
  const int sloupecCas = 128;
  const int datumOffset=63;
  
  oled.setDrawColor(0);
  oled.drawBox(0, 50, 128, 15);
  oled.setDrawColor(1);    
  oled.setClipWindow(0, 52, sloupecCas-30, 64); 
  oledDrawStringFromLeft(0-infotextOffset, datumOffset, infotext);
  oled.setMaxClipWindow();
  oled.drawLine(0, 52, 127, 52);    
  oledDrawStringFromRight(sloupecCas, datumOffset, cas, true);   
  if(vykresliDvojtecku)
  {
    String colon=":";
    oledDrawStringFromRight(sloupecCas-14,datumOffset,colon, true);   
  }
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

  oledDrawStringFromLeft(0, radek * vyskaRadku + posun, String(aktStranka) + "/" + String(pocetStranek));
  oledDrawStringFromRight(sloupecCas, radek * vyskaRadku + posun, cas, false);
  oled.drawLine(0, 50, 127, 50);
}




void oledSetTextPage(String line1, String line2="", String line3="", String line4="",String line5="",String line6="")
{
  #ifdef MEGAOLED 
    oled.setFont(ZIS_12_normal);
  #else
    oled.setFont(czfont9);
  #endif;
  oled.clearBuffer();
  oledDrawStringFromLeft(0, 0*vyskaRadku+offsetRadku, line1);
  oledDrawStringFromLeft(0, 1*vyskaRadku+offsetRadku, line2);
  oledDrawStringFromLeft(0, 2*vyskaRadku+offsetRadku, line3);
  oledDrawStringFromLeft(0, 3*vyskaRadku+offsetRadku, line4);
  oledDrawStringFromLeft(0, 4*vyskaRadku+offsetRadku, line5);
  #ifdef MEGAOLED
    oledDrawStringFromLeft(0, 5*vyskaRadku+offsetRadku, line6);
  #endif

  oled.sendBuffer();
}
void oledSetTextPageRaw(String line1, String line2="", String line3="", String line4="",String line5="",String line6="")
{
  #ifdef MEGAOLED 
    oled.setFont(ZIS_12_normal);
  #else
    oled.setFont(czfont9);
  #endif;
  oledDrawStringFromLeft(0, 0*vyskaRadku+offsetRadku, line1);
  oledDrawStringFromLeft(0, 1*vyskaRadku+offsetRadku, line2);
  oledDrawStringFromLeft(0, 2*vyskaRadku+offsetRadku, line3);
  oledDrawStringFromLeft(0, 3*vyskaRadku+offsetRadku, line4);
  oledDrawStringFromLeft(0, 4*vyskaRadku+offsetRadku, line5);
  #ifdef MEGAOLED
    oledDrawStringFromLeft(0, 5*vyskaRadku+offsetRadku, line6);
  #endif
}
/*
void oledSetGlobalInfotext(String input)
{
  int delkaTextu=35;
  oledSetTextPage(input.substring(0*delkaTextu,delkaTextu),input.substring(1*delkaTextu,2*delkaTextu),input.substring(2*delkaTextu,3*delkaTextu),input.substring(3*delkaTextu,4*delkaTextu));
}
*/

void oledPeriodicDisplayUpdate()
{
  String casPrikaz = "0:00";
  String den = "";

  // time
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  char buffer[80];
  char bufferDatum[80];
  char bufferCas[80];
  char bufferMinuty[80];
  char bufferHodiny[80];

  //strftime(buffer, 80, "%Y%m%d",timeinfo);
  strftime(bufferDatum, 80, "%d.%m.%Y", timeinfo);
  strftime(bufferHodiny, 80, "%H", timeinfo);
  strftime(bufferMinuty, 80, "%M", timeinfo);

  bool displayColon=false;


  if(timeinfo->tm_sec%2==0)
  {
    displayColon=true;  
  }
  else
  {
    displayColon=false;
  }

  #ifdef MEGAOLED
    casPrikaz=String(bufferHodiny)+"   "+String(bufferMinuty); 
  #else
    casPrikaz=String(bufferHodiny)+"  "+String(bufferMinuty); 
  #endif 

  int minutOdRana = 0;
  strftime(buffer, 80, "%u", timeinfo);
  den = buffer;  
  int cisloRadkuInfo = 5;

//xxxx


//infotextGlobalVariable="Příliš žluťoučký kůň úpěl ďábelské ódy.";

  if(infotextGlobalVariable=="")
  {
    oledVykresliSpodniRadekDatum(casPrikaz, cisloDoDne(den.toInt())+String(" ")+bufferDatum, cisloRadkuInfo,displayColon);
  }
  else
  {
    oledVykresliSpodniRadekInfotext(casPrikaz,infotextGlobalVariable,infotextOffset,cisloRadkuInfo, displayColon);
    infotextOffset++;
    int textWidth=oled.getStrWidth(infotextGlobalVariable.c_str());

    if(infotextOffset>textWidth)
    {
      infotextOffset=-infoStartPosition;
    }
  }
  oled.sendBuffer();
}



void oledSetGlobalInfotext(String infotextFullscreen)
{
  #ifdef MEGAOLED 
    oled.setFont(ZIS_12_normal);
  #else
    oled.setFont(czfont9);
  #endif;
  
  int start = 0;
  String output="";
  const int n = infotextFullscreen.length();

  String stringArray[6];
  int stringIndex=0;

  String currentString="";

  // Skip leading spaces (if any)
  while (start < n && infotextFullscreen[start] == ' ') 
  {
    start++;
  }


  while (start < n) 
  {
    int end = infotextFullscreen.indexOf(' ', start);
    bool finished=false;
    if (end == -1)
    {
       end = n;  // last token to end
      finished=true;
    }

    // Extract token
    String token = infotextFullscreen.substring(start, end);

    if (token.length() > 0) 
    {
      Serial.println(token);
      output+="*"+token;
      
      String testString=currentString;
      
      if(currentString!="")
      {
        testString+=" ";
      }
      testString+=token;


      uint16_t w = oled.getUTF8Width(testString.c_str());
      if(w<maxDelkaRadku)
      {
        if(currentString!="")
        {
          currentString+=" ";
        }
        
        currentString+=token;

        if(finished)
        {
          stringArray[stringIndex]=currentString;
          stringIndex++; 
        }
      }
      else
      {
        if(stringIndex<6)
        {       
          stringArray[stringIndex]=currentString;
          stringIndex++; 
          currentString=token;
        }
      }
    }

    // Move past spaces to next token
    start = end;
    while (start < n && infotextFullscreen[start] == ' ')
    {
      start++;
    } 
  }
  
  oledSetTextPageRaw(stringArray[0],stringArray[1],stringArray[2],stringArray[3],stringArray[4],stringArray[5]);
  //return output;

}



#endif //end of OLED





