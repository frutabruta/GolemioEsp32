void handleResponse(HTTPClient &http) 
{
  #ifdef DEBUGGING
    Serial.println("handleResponse");
  #endif
  
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
  JsonDocument filter;

  filter["stops"][0]["stop_name"] = true;
  filter["stops"][0]["platform_code"] = true;
  filter["departures"][0]["departure_timestamp"]["minutes"] = true;
  filter["departures"][0]["trip"]["headsign"] = true;
  filter["departures"][0]["route"]["short_name"] = true;
  filter["infotexts"][0]["text"] = true;
  filter["infotexts"][0]["display_type"] = true;

  #ifdef MEGAOLED
    filter["departures"][0]["trip"]["is_wheelchair_accessible"] = true;
    filter["departures"][0]["trip"]["is_air_conditioned"] = true; 
    filter["departures"][0]["trip"]["direction"] = true; 
    filter["departures"][0]["stop"]["platform_code"] = true;
  #endif

  #ifdef DEBUGGING
    Serial.print("Velikost filtru:");
    Serial.println(String(filter.size()));
  #endif

  DeserializationError error = deserializeJson(root, httpResult, DeserializationOption::Filter(filter));

  // Test if parsing succeeds.
  if (error) 
  {
    #ifdef DEBUGGING
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    #endif DEBUGGING
    return;
  }
  else
  {
    Serial.print("json capacity:");
  }


  #ifdef USE_OLED
    oled.clearBuffer();
  #endif

  String infotextsRunning="";
  String infotextFullscreen="";
 
  bool multipleStops=false;
  
  #ifdef MEGAOLED
    int stopCount= root["stops"].size();

    if(stopCount>0)
    {
      String stopName=root["stops"][0]["stop_name"].as<const char *>();
      String platform=root["stops"][0]["platform_code"].as<const char *>();
      if(stopCount>1)
      {
        multipleStops=true;
        oledVykresliHlavicku("",stopName);
      }
      else
      {
        oledVykresliHlavicku(platform,stopName);
      }
    }
  #endif



  int arraySizeInfotexts = root["infotexts"].size();
  Serial.println("pocet infotextu:" + String(arraySizeInfotexts));
 
  for (int i = 0; (i < arraySizeInfotexts); i++) 
  {
    String infotext= root["infotexts"][i]["text"].as<const char *>();
    String infotextType= root["infotexts"][i]["display_type"].as<const char *>();
  
    if(infotextType=="inline")
    {
      infotextsRunning+=infotext;
    }
    else if (infotextType=="general")
    {
      infotextFullscreen=infotext;
    }
    Serial.println(infotext);
  }

  int arraySize = root["departures"].size();
  Serial.println("pocet odjezdu:" + String(arraySize));

  int counter = 0;

  for (int i = 0; (i < arraySize); i++) 
  {
    Serial.println("pruchod " + String(i));
    Serial.println("pocetElementu " + String(root[0][i].size()));
    String cas = root["departures"][i]["departure_timestamp"]["minutes"].as<const char *>();
    String linka = root["departures"][i]["route"]["short_name"].as<const char *>();
    String cil = root["departures"][i]["trip"]["headsign"].as<const char *>();
    String platformCode = root["departures"][i]["stop"]["platform_code"].as<const char *>();
    String direction = root["departures"][i]["trip"]["direction"].as<const char *>();
    bool isAccessible = root["departures"][i]["trip"]["is_wheelchair_accessible"];
    bool isAirConditioned = root["departures"][i]["trip"]["is_air_conditioned"];

    if(!multipleStops)
    {
      platformCode="";
    }
    else
    {
      direction="";
    }
   
    Serial.println("spoj: " + linka + " " + cil + " "+platformCode+" " + cas);

    #ifdef USE_OLED
      if (counter < oledMaxPocetOdjezdu) 
      {
        #ifdef MEGAOLED
          oledVykresliRadekOdjezdu(linka, cil, cas, counter,isAccessible,isAirConditioned,platformCode,  direction);
        #else 
          oledVykresliRadekOdjezdu(linka, cil, cas, counter);
        #endif
      }
    #endif

    #ifdef USE_LCD
      lcdVymazRadekOdjezdu(counter);
      if (counter < lcdMaxPocetOdjezdu) 
      {
        lcdVykresliRadekOdjezdu(linka, cil, cas, counter);
      }
      else 
      {
      
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
    if(  infotextGlobalVariable!=infotextsRunning)
    {
      infotextOffset=0;
    }
    else
    {
      infotextOffset++;
    }
    infotextGlobalVariable=infotextsRunning;
    oledPeriodicDisplayUpdate();
    oled.sendBuffer();
  #endif
}
