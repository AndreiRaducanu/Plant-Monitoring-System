#include "functions.h"


//functie folosita pentru citirea umiditatii solului
int getHumidityPercentage(int humiditySensorPin) {
  int sensorValue = analogRead(humiditySensorPin);
  //constante folosite pentru calibrarea senzorului
  const int dry = 3800;
  const int wet = 1700;
  int humidityPercentage = 100 * (sensorValue - dry)/(wet-dry);
  if (humidityPercentage >= 0 and humidityPercentage <= 100) {
    return humidityPercentage;
  }
  else if (humidityPercentage > 100) {
    return 100;
  }
  else {
    return 0;
  }
}

//functie folosita pentru a calcula nivelul de apa ramas in bazin
float getWaterLevel(int trigger_pin, int echo_pin) {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  float distance = duration * 0.034 / 2;
  //10.5cm....0% 2cm.....100%
  float waterLevel = map(distance, 10.5, 2, 0, 100);
  if (waterLevel >= 0 and waterLevel <= 100) {
    return waterLevel;
  } 
  else if (waterLevel > 100) {
    return 100;
  }
  else {
    return 0;
  }
  
}

void waterThePlant(int VALVE1_PIN, int VALVE2_PIN, int PUMP_PIN) {
  digitalWrite(VALVE1_PIN, LOW);
  digitalWrite(VALVE2_PIN, HIGH);
  digitalWrite(PUMP_PIN, LOW);
  delay(2000);                    
  digitalWrite(VALVE1_PIN, HIGH);
  digitalWrite(VALVE2_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);
}

void sprayThePlant(int VALVE1_PIN, int VALVE2_PIN, int PUMP_PIN) {
  digitalWrite(VALVE1_PIN, HIGH);
  digitalWrite(VALVE2_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
  delay(2000);            
  digitalWrite(VALVE1_PIN, HIGH);
  digitalWrite(VALVE2_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);
}


