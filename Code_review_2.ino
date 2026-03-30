//libraries
#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"


//definitions
#define AL_ADDR 0x48
SparkFun_Ambient_Light light(AL_ADDR);
float gain = .125; 
int time = 100;
long luxVal = 0; 




int i = 0;


void setup(){

  Wire.begin();
  Serial.begin(115200);



  
//light sensor setup
  if(light.begin())
    Serial.println("Ready to sense some light!"); 
  else
    Serial.println("Could not communicate with the sensor!");
  // Again the gain and integration times determine the resolution of the lux
  // value, and give different ranges of possible light readings. Check out
  // hoookup guide for more info. 
  light.setGain(gain);
  light.setIntegTime(time);


for(int i=1; i<=6; i++){
    luxVal = light.readLight();
    Serial.print("Reading ");
    Serial.print(i);
    Serial.print(":");
    Serial.println(luxVal);
    delay(1000);
 }

}
void loop(){
  


}
