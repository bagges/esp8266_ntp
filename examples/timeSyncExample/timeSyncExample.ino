#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Time.h>

#define DEBUG_SERIAL_BAUD 38400
#define DEVICE_SERIAL_BAUD 115200

#define WIFI_SSID "myssid"
#define WIFI_PASS "mypass"

SoftwareSerial debugSerial(10,11);
String command = "";
String result = "";
char character;

void setup() {
  debugSerial.begin(DEBUG_SERIAL_BAUD);
  debugSerial.println("Debug serial started...");
  
  Serial.begin(DEVICE_SERIAL_BAUD);
  Serial.setTimeout(5000);
  Serial.println("AT");
  if(Serial.find("OK")) {
    debugSerial.println("Device Ready");
  } else {
    debugSerial.println("Could not open device");
    return;
  }
  
  debugSerial.println("Switching to Station...");
  Serial.println("AT+CWMODE=1");
  if(Serial.find("OK")) {
    debugSerial.println("OK");
  }
  
  debugSerial.println("Connecting to wifi...");
  command = "AT+CWJAP=";
  command.concat(WIFI_SSID);
  command.concat(",");
  command.concat(WIFI_PASS);
  Serial.println(command);
  delay(5000);
  Serial.println("AT+CWJAP?");
  if(Serial.find(WIFI_SSID)) {
    debugSerial.println("OK");
  }
  
  debugSerial.println("Sending NTP Request...");
  Serial.println("AT+CIPNTP=1");
  delay(5000);
}

void loop() {
  Serial.println("AT+CIPNTP?");
  result = "";
  if(Serial.findUntil("Time: ", " ")) {
    result = Serial.readStringUntil('\n');
    if(result.length() == 26) {
      debugSerial.println("Lenght OK, converting...");
      char timeArray[26];
      result.toCharArray(timeArray, 26);
      int _hour = ((timeArray[0] - '0') * 10) + ((timeArray[1] - '0') << 0);
      int _minute = ((timeArray[3] - '0') *10) + ((timeArray[4] - '0') << 0);
      int _second = ((timeArray[6] - '0') *10) + ((timeArray[7] - '0') << 0);
      int _month = ((timeArray[9] - '0') *10) + ((timeArray[10] - '0') << 0);
      int _day = ((timeArray[12] - '0') *10) + ((timeArray[13] - '0') << 0);
      int _year = ((timeArray[15] - '0') *1000) + ((timeArray[16] - '0') *100) + ((timeArray[17] - '0') *10) + ((timeArray[18] - '0') << 0);
      setTime(_hour, _minute, _second, _month, _day, _year);
    }
    String time;
    time.concat(hour()); time.concat(":"); time.concat(minute()); time.concat(":"); time.concat(second()); time.concat(" ");
    time.concat(day()); time.concat("."); time.concat(month()); time.concat("."); time.concat(year());
    debugSerial.println(result);
    debugSerial.println(time);
  }
  Serial.readString();
  delay(1000);
}
