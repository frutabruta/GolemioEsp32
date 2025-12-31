/* used for different library
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
*/
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



String dirToArrow(const String& s) {
  if      (s == "top")          return "↑"; // \xE2\x86\x91
  else if (s == "top-right")    return "↗"; // \xE2\x86\x97
  else if (s == "right")        return "→"; // \xE2\x86\x92
  else if (s == "bottom-right") return "↘"; // \xE2\x86\x98
  else if (s == "bottom")       return "↓"; // \xE2\x86\x93
  else if (s == "bottom-left")  return "↙"; // \xE2\x86\x99
  else if (s == "left")         return "←"; // \xE2\x86\x90
  else if (s == "top-left")     return "↖"; // \xE2\x86\x96
  return "?";
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


