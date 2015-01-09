#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include "esp8266_ntp.h"

#define WIFI_SSID "myssid"
#define WIFI_PASS "mypass"

SoftwareSerial debugSerial(10,11);

ESP8266_NTP esp8266_ntp;

int offset = 0;

void setup() {
  debugSerial.begin(38400);
  if(esp8266_ntp.initialize(WIFI_SSID, WIFI_PASS) == 0) {
    debugSerial.println("Initialized!");
    esp8266_ntp.setGMTOffset(1); //GMT+1
  }
  //set the sync provider
  setSyncProvider(getNtpTime);
  setSyncInterval(10);
}

//sync provider
time_t getNtpTime() {
  //switch offset every sync, so that we can se that a sync happened
  if(offset == 1) {
    offset = 2;
  } else {
    offset = 1;
  }
  esp8266_ntp.setGMTOffset(offset);
  return esp8266_ntp.getTime();
}

void loop() {
  //just print the time, not nice but works ;-)
  String time;
  time.concat(hour()); time.concat(":"); time.concat(minute()); time.concat(":"); time.concat(second()); time.concat(" ");
  time.concat(day()); time.concat("."); time.concat(month()); time.concat("."); time.concat(year());
  debugSerial.println(time);
  delay(1000);
}
