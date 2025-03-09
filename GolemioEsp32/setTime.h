void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void initTime(String timezone){
  struct tm timeinfo;

  Serial.println("Setting up time");
  //configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
  configTime(0,0, "tik.cesnet.cz", "time.nist.gov");
//    configTime(1 * 3600, 1 * 3600, "tik.cesnet.cz", "time.nist.gov");
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}
