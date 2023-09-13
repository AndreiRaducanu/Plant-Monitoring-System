//Includem librariile necesare proiectului
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
#include "thingProperties.h"
#include "functions.h"
#include "RTClib.h"
#include <DHT.h>
#include <DHT_U.h>

//definim pinii utilizati de senzori
const int DHT_PIN = 23;
const int MQ_PIN = 34;
const int SOIL_PIN = 35;
//HC-SR04 PINS
const int TRIG_PIN = 16;
const int ECHO_PIN = 4;

//variabile utilizate de modulul RTC
DateTime previous;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//creem obiectele necesare modulelor
LiquidCrystal_I2C lcd(0x27, 16, 2);
BH1750 lightMeter(0x23);
RTC_DS3231 RTC;
DHT_Unified dht(DHT_PIN,DHT22);

//definim pinii utilizati de relee
const int LAMPA_PIN = 25;
const int PUMP_PIN = 26;
const int VALVE1_PIN = 32;
const int VALVE2_PIN = 33;


void setup() {
  Serial.begin(9600);

  //setam pinul senzorului de umiditate ca intrare
  pinMode(SOIL_PIN, INPUT);

  //setam pinii utilizati de modulul HC-SR04 ca intrare si iesire
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //setam pinul utilizat de modulul MQ can intrare
  pinMode(MQ_PIN, INPUT);

  //incepem comunicarea cu senzorul DHT22
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  //verificam ca modulul RTC functioneaza in parametrii normali
  #ifndef ESP32
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif
  if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  //in cazul in care modulul RTC ramane fara baterie resetam data
  if (RTC.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    RTC.adjust(DateTime(2023, 6, 25, 23, 33, 35));
  }
  
  //verificam ca senzorul BH1750 functioneaza in mod normal
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }

  //initializam LCD
  lcd.init();
  lcd.backlight();
  delay(1500); 
  //ne conectam la Arduino IoT Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  //setam pini relee
  pinMode(LAMPA_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(VALVE1_PIN, OUTPUT);
  pinMode(VALVE2_PIN, OUTPUT);

  //la pornirea programului toate releele sunt dezactivate
  digitalWrite(LAMPA_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);
  digitalWrite(VALVE1_PIN, HIGH);
  digitalWrite(VALVE2_PIN, HIGH);
}



void loop() {
//folosim aceasta expresie pentru a ne asigura ca trec 5 minute inainte de a incerca sa luam actiuni automate asupra releelor
  for(int i = 1; i <= 75; i++){     //75 de iteratii * 4 secunde delay
    //luam valori de la toti senzorii
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    temperature = event.temperature; //event.temperature *C
    dht.humidity().getEvent(&event);
    air_humidity = event.relative_humidity; //event.relative_humidity %
    soil_humidity = getHumidityPercentage(SOIL_PIN);
    water_level = getWaterLevel(TRIG_PIN, ECHO_PIN); // %
    air_quality = analogRead(MQ_PIN); //ppm
    light_lux = lightMeter.readLightLevel(); //lux
    //afisam valorile pe LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura: ");
    lcd.print(temperature);
    lcd.print(" *C");
    lcd.setCursor(0,1);
    lcd.print("Umiditate aer: ");
    lcd.print(air_humidity);
    lcd.print(" %");
    lcd.setCursor(0,2);
    lcd.print("Umiditate sol: ");
    lcd.print(soil_humidity);
    lcd.print(" %");
    //facem update la cloud
    ArduinoCloud.update();
    delay(2000);
    //afisam urmatorile valori pe LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nivel apa: ");
    lcd.print(water_level);
    lcd.print(" %");
    lcd.setCursor(0,1);
    lcd.print("Calit aer: ");
    lcd.print(air_quality);
    lcd.print(" ppm");
    lcd.setCursor(0,2);
    lcd.print("Lumina: ");
    lcd.print(light_lux);
    lcd.print(" lux");
    delay(2000);
    ArduinoCloud.update();
  }
  //daca umiditatea solului este mai mica de 30% si au trecute 5 minute de la ultima irigare, porneste pompa si valva respectiva
  if (soil_humidity < 30) {
    DateTime now = RTC.now();
    int elapsedSeconds = (now.unixtime() - previous.unixtime());
    //folosim modulul RTC DS3231 pentru a verifica ca au trecut minim 5 minute de la irigarea plantei
    if (elapsedSeconds >= 5 * 60) {
      previous = RTC.now();
      Serial.println("ACTIVATE PUMP 5MIN PASSED");
      waterThePlant(VALVE1_PIN, VALVE2_PIN, PUMP_PIN);
    }
  }
  delay(2000); //important pentru afisarea corecta a valorilor senzorilor dupa decuplarea imediata a releelor
}

//functie folosita de Cloud pentru a iriga planta
void onWaterPlantChange()  {
  if(water_plant){
    waterThePlant(VALVE1_PIN, VALVE2_PIN, PUMP_PIN);
    water_plant = false;
    delay(1000); //to avoid abnormal readings from DHT
  }
  else{
    water_plant = false;
  }
}

//functie folosita de Cloud pentru a stropii planta
void onSprayPlantChange()  {
  if(spray_plant){
    sprayThePlant(VALVE1_PIN, VALVE2_PIN, PUMP_PIN);
    spray_plant = false;
    delay(1000); //to avoid abnormal readings from DHT
  }
  else{
    spray_plant = false;
  }
}
//functie folosita de Cloud pentru a activa lampa
void onLampaChange(){
  if(lampa){
    digitalWrite(LAMPA_PIN, LOW);
  }
  else{
    digitalWrite(LAMPA_PIN, HIGH);
  }
}