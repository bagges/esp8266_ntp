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
  }
}

void loop() {
  if(offset == 0) {
    offset=1;
  } else {
    offset = 0;
  }
  esp8266_ntp.setGMTOffset(offset);
  time_t ntptime = esp8266_ntp.getTime();
  setTime(ntptime);
  String time;
  time.concat(hour()); time.concat(":"); time.concat(minute()); time.concat(":"); time.concat(second()); time.concat(" ");
  time.concat(day()); time.concat("."); time.concat(month()); time.concat("."); time.concat(year());
  debugSerial.println(time);
  delay(1000);
}
