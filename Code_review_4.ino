//libraries
#include <Wire.h>   //connections
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"   //light sensor
#include <SparkFun_Qwiic_Button.h>    //button

//definitions

//button
QwiicButton button;
uint8_t brightness = 10;   //LED brightness


//light sensor
#define AL_ADDR 0x48
SparkFun_Ambient_Light light(AL_ADDR);
float gain = .125;
int time = 100;
long luxVal = 0; 

int i=0;
long sum=0;
long ave=0;
int n=0;


void setup(){

  Wire.begin();
  Serial.begin(115200);

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





}
void loop(){
  while (button.isPressed()==false){
    delay(10);
  }
  
  for(int i=1; i<=3; i++){
    luxVal = light.readLight();
    Serial.print(luxVal);
    Serial.print(",");
    sum = sum + luxVal;
    n=i;
    delay(1000);
 }
  ave = sum / n;
  Serial.println();
  Serial.print("Ave:");
  Serial.println(ave);
  sum=0;
}
