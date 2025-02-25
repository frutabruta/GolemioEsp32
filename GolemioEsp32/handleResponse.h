void handleResponse(HTTPClient &http) {
 // const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
 // Serial.println("capacity:" + String(capacity));
  //DynamicJsonDocument root(9000);

 Serial.println("handleResponse");


  String httpResult = http.getString();
#ifdef DEBUGGING
  Serial.println(httpResult);
#endif
  /*
      to use http stream ChunkDecodingStream may help
    https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/
DeserializationError error = deserializeJson(root, http.getStream(), DeserializationOption::Filter(filter));
*/
  JsonDocument root;

  //DynamicJsonDocument root(9000);

  JsonDocument filter;
  //StaticJsonDocument<200> filter;
  /*
  filter[0][0]["departure"]["minutes"] = true;
  filter[0][0]["trip"]["headsign"] = true;
  filter[0][0]["route"]["short_name"] = true;
  */
  filter["departures"][0]["departure_timestamp"]["minutes"] = true;
  filter["departures"][0]["trip"]["headsign"] = true;
  filter["departures"][0]["route"]["short_name"] = true;
  Serial.print("Velikost filtru:");
  Serial.println(String(filter.size()));





  DeserializationError error = deserializeJson(root, httpResult, DeserializationOption::Filter(filter));

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    Serial.print("json capacity:");
// deprecated in ArduinoJson7    Serial.println(root.capacity());
  }



  //   clearDisplays();

  // oled.setTextAlignment(TEXT_ALIGN_LEFT);


#ifdef USE_LCD
  int lcdMaxPocetOdjezdu = 7;
#endif

#ifdef USE_OLED
  int oledMaxPocetOdjezdu = 5;
  oled.clearBuffer();

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
    Serial.println("pruchod " + String(i));
    Serial.println("pocetElementu " + String(root[0][i].size()));
    String cas = root["departures"][i]["departure_timestamp"]["minutes"].as<const char *>();
    String linka = root["departures"][i]["route"]["short_name"].as<const char *>();
    String cil = root["departures"][i]["trip"]["headsign"].as<const char *>();
    Serial.println("spoj: " + linka + " " + cil + " " + cas);

#ifdef USE_OLED
    if (counter < oledMaxPocetOdjezdu) {
      oledVykresliRadekOdjezdu(linka, cil, cas, counter);
    }
#endif

#ifdef USE_LCD
    lcdVymazRadekOdjezdu(counter);
    if (counter < lcdMaxPocetOdjezdu) {
      lcdVykresliRadekOdjezdu(linka, cil, cas, counter);
    } else {
     
    }


#endif

    counter++;
  }


#ifdef USE_LCD

  if(arraySize<lcdMaxPocetOdjezdu)
  {
    for(int j=arraySize;j<lcdMaxPocetOdjezdu;j++)
    {
      lcdVymazRadekOdjezdu(j);
    }
  }
#endif



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



#ifdef USE_LCD
  lcdVykresliCas();
#endif

  ////////konec casu

#ifdef USE_OLED
  oledVykresliSpodniRadekDatum(casPrikaz, cisloDoDne(den.toInt())+String(" ")+bufferDatum, cisloRadkuInfo);

  //oled.startscrollleft(6,7);
  oled.sendBuffer();
#endif
}
