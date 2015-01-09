#ifndef ESP8266_NTP_H
#define ESP8266_NTP_H

#include <Arduino.h>
#include <Time.h>

#define DEVICE_NOT_FOUND 1
#define MODE_CHANGE_FAILED 2
#define CONNECTION_FAILED 3

#define DEVICE_SERIAL_BAUD 115200

class ESP8266_NTP
{
  public:
    ESP8266_NTP();
    int initialize(char* WIFI_SSID, char* WIFI_PASS);
    time_t getTime();
    void setGMTOffset(int offset);
  private:
    int gmtOffset;
};

#endif
