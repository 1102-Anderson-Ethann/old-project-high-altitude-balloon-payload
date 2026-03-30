//Libraries
#include <Wire.h>   //connections
#include "SparkFun_Qwiic_OpenLog_Arduino_Library.h"   //Data storage
#include <SparkFun_Qwiic_Button.h>    //button
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"   //light sensor
#include <SparkFun_I2C_GPS_Arduino_Library.h>   //GPS
#include <TinyGPS++.h>    //GPS
#include <SparkFun_PHT_MS8607_Arduino_Library.h>    //WX

//Definitions
//log
OpenLog myLog;

//mission timer
unsigned long mytime;

//button
QwiicButton button;
uint8_t brightness = 10;   //LED brightness

//light sensor
#define AL_ADDR 0x48
SparkFun_Ambient_Light light(AL_ADDR);
float gain = .125;
int time = 100;
long luxVal = 0; 

//GPS
I2CGPS myGPS;
TinyGPSPlus gps;

//WX sensor
MS8607 WX;

//-----------------------------------------------------------------------------------------------------------------------------------
void setup() {
//basics
  Wire.begin();
  myLog.begin();
  Serial.begin(115200);
  Serial.println("setup starting");

//Open log
  myLog.println("Best team ever data log");
  Serial.println("Log opened");
 
//Data headers
  myLog.println("Mission time,  # of Satellites,  GPS Time, Latitude, Longitude,  Altitude, HDOP, Temp (C), Pressure, Humidity, Light (lux)");

//Button setup  
  while (button.begin()==false){
    delay(10);}
  Serial.println("Button connected");
  button.LEDoff();
  
//light sensor
  while (light.begin()==false){
    delay(10);}
  Serial.println("Light sensor connected");
  light.setGain(gain);
  light.setIntegTime(time);

//GPS
  while (myGPS.begin()==false){
    delay(10);}
  Serial.println("GPS connected");

//WX
  while (WX.begin()== false){
    delay(10);}
  Serial.println("WX connected");


//push button start
  button.LEDon();
  while (button.isPressed()==false){
    delay(10);}
  button.LEDoff();

} //Close void setup
//----------------------------------------------------------------------------------------------------------------------------------
void loop() {
  
//mission time (time since turned on)
  Serial.print("Time elapsed:");
  mytime= millis();
  Serial.print(mytime);
  myLog.print(mytime);
  myLog.print(",");

//GPS  # of satellites
  while (myGPS.available()){
    gps.encode(myGPS.read());}
  if (gps.satellites.isValid()){
    int sats = (gps.satellites.value());
    Serial.print(" Sats:");
    Serial.print(sats);
    Serial.print(",");
    myLog.print(sats);
    myLog.print(",");}
  else{
    Serial.print("Sats:");
    Serial.print("unknown");
    Serial.print(",");
    myLog.print("unknown");
    myLog.print(",");}
  
//Time (GPS)
  if (gps.time.isValid()){
  //serial
    Serial.print(" GPS Time:");
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  //log 
    if (gps.time.hour() < 10) myLog.print(F("0"));
    myLog.print(gps.time.hour());
    myLog.print(F(":"));
    if (gps.time.minute() < 10) myLog.print(F("0"));
    myLog.print(gps.time.minute());
    myLog.print(F(":"));
    if (gps.time.second() < 10) myLog.print(F("0"));
    myLog.print(gps.time.second());
    myLog.print(",");}

//Location (GPS) Latitude, Longitude
  if (gps.location.isValid()){
    float latitude = (gps.location.lat());
    float longitude = (gps.location.lng());
    Serial.print(" lat:");
    Serial.print(latitude);
    Serial.print(",");
    myLog.print(latitude);
    myLog.print(",");
    Serial.print(" lng:");
    Serial.print(longitude);
    Serial.print(",");
    myLog.print(longitude);
    myLog.print(",");}
  else{
    Serial.print("lat:");
    Serial.print("unknown");
    Serial.print(",");
    myLog.print("unknown");
    myLog.print(",");
    Serial.print("lng:");
    Serial.print("unknown");
    Serial.print(",");
    myLog.print("unknown");
    myLog.print(",");}

//Altitude (GPS)
  if (gps.altitude.isValid()){
    float altitude = (gps.altitude.meters()); 
    Serial.print(" alt:");
    Serial.print(altitude);
    Serial.print(",");
    myLog.print(altitude);
    myLog.print(",");}
  else{
    Serial.print("alt");
    Serial.print("unknown");
    Serial.print(",");
    myLog.print("unknown");
    myLog.print(",");}

//HDOP (GPS)
  if (gps.hdop.isValid()){
    float HDOP =  (gps.hdop.isValid());
    Serial.print(" HDOP:");
    Serial.print(HDOP);
    Serial.print(",");
    myLog.print(HDOP);  
    myLog.print(",");}
  else {
    myLog.print("unknown");
    myLog.print(",");}

//WX  Temp, Pressure, Humidity
  float temperature = WX.getTemperature();
    Serial.print(" Temp:");
    Serial.print(temperature);
    Serial.print(",");
    myLog.print(temperature);  
    myLog.print(",");    

  float pressure = WX.getPressure();
    Serial.print(" Pres:");
    Serial.print(pressure);
    Serial.print(",");
    myLog.print(pressure);  
    myLog.print(",");   

  float humidity = WX.getHumidity();
    Serial.print(" Hum:");
    Serial.print(humidity);
    Serial.print(",");
    myLog.print(humidity);  
    myLog.print(","); 

    
//Light sensor
  luxVal = light.readLight();
  Serial.print("light:");
  Serial.print(luxVal);
  Serial.print(",");
  myLog.print(luxVal);
  myLog.print(",");

//end line and sync
  Serial.println();
  myLog.println();
  myLog.syncFile();

//blink LED and wait
  button.LEDon();
  delay(100);
  button.LEDoff();

  delay(14270);

} //Close void loop
