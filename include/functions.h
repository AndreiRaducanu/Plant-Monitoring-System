#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include <Arduino.h>

int getHumidityPercentage(int humiditySensorPin);
float getWaterLevel(int trigger_pin, int echo_pin);
void waterThePlant(int VALVE1_PIN, int VALVE2_PIN, int PUMP_PIN);
void sprayThePlant(int VALVE1_PIN, int VALVE2_PIN, int PUMP_PIN);

#endif