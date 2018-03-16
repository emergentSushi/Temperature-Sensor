#include <Wire.h>
#include <SI7021.h>
#include <ESP8266WiFi.h>
#include "Private.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define SENSOR_ID       "1"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish tempFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/t" SENSOR_ID);
Adafruit_MQTT_Publish humFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/h" SENSOR_ID);

SI7021 si7021;
ADC_MODE(ADC_VCC);

void setup()
{
  Serial.begin(115200);
  
  si7021.begin(D2, D1);
  si7021.setHeater(0);
  while(!Serial);
    
  WiFi.begin(WIFI_SSID, WIFI_PW);    //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection
    delay(500);
    Serial.println("Waiting for wifi");
  }
}

void loop()
{
  si7021_thc data;
  data = si7021.getTempAndRH();
  Serial.printf("%fC %d%%\n", data.celsiusHundredths / 100.0, data.humidityPercent);
  Serial.println();

  MQTT_connect();
  
  tempFeed.publish(data.celsiusHundredths / 100.0);
  humFeed.publish(data.humidityPercent);
  
  //need to wire RST to GPIO16 (D0) for this line to work
  //ESP.deepSleep(30e6, WAKE_RF_DEFAULT);
  
  delay(5000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
