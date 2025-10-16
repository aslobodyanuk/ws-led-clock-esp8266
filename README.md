# 7 Segment Clock using ESP8266 and WS2812B leds

## There should be settings.h file and it should look something like this

```c++
#include "TZinfo.h"

#define WIFI_HOSTNAME "digi-clock"

// Setup
String WIFI_SSID = "name of your wifi access point";
String WIFI_PASS = "wifi password";

// pick one from TZinfo.h
String TIMEZONE = getTzInfo("Europe/Zagreb");

// change for different NTP (time servers)
#define NTP_SERVERS "pool.ntp.org"
// #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"

// August 1st, 2018
#define NTP_MIN_VALID_EPOCH 1533081600

```