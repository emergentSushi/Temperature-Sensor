# Temperature-Sensor
ESP8266 (NodeMCU 1.0 ESP-12e) firmware that pulls temperature and humidty data from SI7021 then pushes the data via MQTT to an adafruit IOT endpoint.

Expects "Private.h" to define the following macros:

~~~~
#define AIO_USERNAME    //username for https://io.adafruit.com
#define AIO_KEY         //Adafruit MQTT API key
#define WIFI_SSID       //Wifi SSID
#define WIFI_PW         //Wifi Password
~~~~

Tested on NodeMCU 1.0, probably works on other ESP8266 boards.
