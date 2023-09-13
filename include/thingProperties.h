#ifndef THINGPROPERTIES_H 
#define THINGPROPERTIES_H 

#include <arduino_secrets.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = DEVICE_LOGIN;
const char SSID[]               = SECRET_SSID;            // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;   // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]         = SECRET_DEVICE_KEY;      // Secret device password

//definire functii utilizate de cloud
void onRelayChange();
void onWaterPlantChange();
void onSprayPlantChange();
void onLampaChange();

//definire variable utilizate de cloud
bool water_plant;
bool spray_plant;
bool lampa;

int temperature;
int air_humidity;
int soil_humidity;
int water_level;
int air_quality;
int light_lux;

//atasam variabiile si functiile la cloud
void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(water_plant, READWRITE, ON_CHANGE, onWaterPlantChange);
  ArduinoCloud.addProperty(spray_plant, READWRITE, ON_CHANGE, onSprayPlantChange);
  ArduinoCloud.addProperty(lampa, READWRITE, ON_CHANGE, onLampaChange);
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(air_humidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(soil_humidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(water_level, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(air_quality, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(light_lux, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

#endif