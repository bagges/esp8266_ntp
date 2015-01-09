/* 

(c) 2015 - Markus Backes - https://backes-markus.de/blog/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
der GNU General Public License, wie von der Free Software Foundation,
Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
veröffentlichten Version, weiterverbreiten und/oder modifizieren.

Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
Siehe die GNU General Public License für weitere Details.

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>. 
*/

#include <Arduino.h>
#include <Time.h>
#include "esp8266_ntp.h"

#define CONST_OK "OK"
#define CONST_AT "AT"

ESP8266_NTP::ESP8266_NTP() {
}

int ESP8266_NTP::initialize(char* WIFI_SSID, char* WIFI_PASS) {
  gmtOffset = 0;
  String command;
  String result;
  Serial.begin(DEVICE_SERIAL_BAUD);
  Serial.setTimeout(5000);
  Serial.println(CONST_AT);
  if(!Serial.find(CONST_OK)) {
    //could not open device
    return DEVICE_NOT_FOUND;
  }
  
  Serial.println("AT+CWMODE=1");
  if(!Serial.find(CONST_OK)) {
    return MODE_CHANGE_FAILED;
  }
  
  command = "AT+CWJAP=";
  command.concat(WIFI_SSID);
  command.concat(",");
  command.concat(WIFI_PASS);
  Serial.println(command);
  delay(5000);
  Serial.println("AT+CWJAP?");
  if(!Serial.find(WIFI_SSID)) {
    return CONNECTION_FAILED;
  }
  return 0;
}

void ESP8266_NTP::setGMTOffset(const int offset) {
  gmtOffset = offset;
}

time_t ESP8266_NTP::getTime() {
  String command = "AT+CIPNTP=";
  command.concat(gmtOffset);
  Serial.println(command);
  delay(1000);
  while(Serial.available()) { //clear buffer
    Serial.read();
  }
  String result = "";
  Serial.println("AT+CIPNTP?");
  if(Serial.findUntil("Time: ", " ")) {
    result = Serial.readStringUntil('\n');
    if(result.length() == 26) {
      char timeArray[26];
      result.toCharArray(timeArray, 26);
      tmElements_t tm;
      tm.Hour = ((timeArray[0] - '0') * 10) + (timeArray[1] - '0');
      tm.Minute = ((timeArray[3] - '0') *10) + (timeArray[4] - '0');
      tm.Second = ((timeArray[6] - '0') *10) + (timeArray[7] - '0');
      tm.Month = ((timeArray[9] - '0') *10) + (timeArray[10] - '0');
      tm.Day = ((timeArray[12] - '0') *10) + (timeArray[13] - '0');
      tm.Year = ((timeArray[15] - '0') *1000) + ((timeArray[16] - '0') *100) + ((timeArray[17] - '0') *10) + (timeArray[18] - '0') - 1970;
      return makeTime(tm);
    }
  }
  while(Serial.available()) {
    Serial.read();
  }
}
