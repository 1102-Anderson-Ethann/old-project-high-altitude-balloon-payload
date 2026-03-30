
#include <Wire.h>         //data logger setup
#include "SparkFun_Qwiic_OpenLog_Arduino_Library.h"
#include <SparkFun_Qwiic_Button.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

#define AL_ADDR 0x48
OpenLog myLog; //Create instance
QwiicButton button;
SparkFun_Ambient_Light light(AL_ADDR);

int ledPin = LED_BUILTIN; //Status LED connected to digital pin 13
// Possible values: .125, .25, 1, 2
// Both .125 and .25 should be used in most cases except darker rooms.
// A gain of 2 should only be used if the sensor will be covered by a dark
// glass.
float gain = .25;

// Possible integration times in milliseconds: 800, 400, 200, 100, 50, 25
// Higher times give higher resolutions and should be used in darker light. 
int time = 100;
long luxVal = 0; 

uint8_t brightness = 100;   //The brightness to set the LED to when the button is pushed
                            //Can be any value between 0 (off) and 255 (max)

void setup() {
  //open log
  Serial.begin(115200);
  Serial.println("I'm starting");
  
  Wire.begin(); //Join I2C bus
  myLog.begin(); //Open connection to OpenLog (no pun intended)
  
  
  Serial.println("Data storage test");

  myLog.println("This goes to the log file");
  
  Serial.println("I'm working");
  float batteryVoltage = 3.4;
  myLog.println("Batt voltage: " + String(batteryVoltage));
  

//check if button will acknowledge over I2C
  if (button.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  Serial.println("Button acknowledged.");
//Setup for light sensor
  if(light.begin())
    Serial.println("Light sensor acknowledged."); 
  else
    Serial.println("Could not communicate with the sensor!");

  light.setGain(gain);
  light.setIntegTime(time);

  Serial.println("Reading settings..."); 
  Serial.print("Gain: ");
  float gainVal = light.readGain();
  Serial.print(gainVal, 3); 
  Serial.print(" Integration Time: ");
  int timeVal = light.readIntegTime();
  Serial.println(timeVal);




  if (button.isPressed() == false)
    Serial.println("Press button to begin");
    while (button.isPressed() == false)
      delay(10);  //wait for user to press button
}

void loop() {
  // read lux
 luxVal = light.readLight();
  
  //print to serial monitor
  Serial.print("Ambient Light Reading: ");
  Serial.print(luxVal);
  Serial.println(" Lux");  


  //print to log
  myLog.print("Ambient Light Reading: ");
  myLog.print(luxVal);
  myLog.println(" Lux");
  myLog.syncFile();
   
  button.LEDon(brightness); //flash light
  delay(10);
  button.LEDoff();

  
  delay(5000); //Wait
}
