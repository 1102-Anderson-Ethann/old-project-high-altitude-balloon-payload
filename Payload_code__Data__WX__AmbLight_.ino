/******************************************************************************
Notes log
Setup the button and LED
Integrated light sensor loop
Added LED flash at the start and end of the loop
WX sensor setup
Data log setup
******************************************************************************/
//Libraries
#include <Wire.h>   //Allows the processor to communicate with connected devices
#include <SparkFun_Qwiic_Button.h>    //Button Library
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"   //Light sensor library
#include <SparkFun_PHT_MS8607_Arduino_Library.h>    //WX Sensor library
#include "SparkFun_Qwiic_OpenLog_Arduino_Library.h"   //Datat storage library


//Definitions
OpenLog myLog;    //Create instance
#define AL_ADDR 0x48 //Not sure, needed for light sensor
QwiicButton button;   //Define the button as 'button'
SparkFun_Ambient_Light light(AL_ADDR);    //Define the light sensor as 'light'
MS8607 barometricSensor;    //Define WX sensor as 'barometricSensor'


//settings
uint8_t brightness = 100;   //LED brightness (min 0, max 255)
float gain = .125;    //Possible values (.125, .25, 1, 2) Use higher gain for low light
int time = 100;   //Possible values (milliseconds) (800, 400, 200, 100, 50, 25) Use larger value for low light
long luxVal = 0;    // Not sure, needed for light sensor
const byte OpenLogAddress = 42; //Default Qwiic OpenLog I2C address



//Settings established, starting setup
void setup() {    //All these things are only done once
//basics 
  Wire.begin(); //tell the processor to look for connections
  Serial.begin(115200);   //data rate
  Serial.println("Tyrus's testing program");    //Starting message

  myLog.begin();    //Start log
  myLog.append("TestLog.txt");    //Create named log
  myLog.println("Title of log");    //Indicates the start of a dataset on the log. Useful if you press the reset button while testing

  
/*Disabled due to lack of cables while testing
//button setup
  if (button.begin() == false) {    //checks if button is connected
    Serial.println("Button not found");   //Report is button is not working and wait
    while (1);}
  Serial.println("Button acknowledged");   //Report that button is connected
  button.LEDoff(); //Start with LED off
*/

//light sensor setup
  if (light.begin()==false){
    Serial.println("Light sensor not found");}
  while (light.begin()==false){
    delay(10);}
  Serial.println("Light sensor acknowledged");


//WX Sensor setup
  if (barometricSensor.begin() == false){
    Serial.println("WX sensor not found");}
  while (barometricSensor.begin() == false){
    delay(10);}
  Serial.println("WX sensor acknowledged");


//humidity aspect of WX setup
 int err = barometricSensor.set_humidity_resolution(MS8607_humidity_resolution_12b); // 12 bits
  if (err != MS8607_status_ok){
    Serial.print("Problem setting the MS8607 sensor humidity resolution. Error code = ");
    Serial.println(err);
    Serial.println("Freezing.");
    while (1);
  }
  err = barometricSensor.disable_heater();    // Turn the humidity sensor heater OFF
  if (err != MS8607_status_ok){   // The TE examples say that get_compensated_humidity and get_dew_point will only work if the heater is OFF
    Serial.print("Problem disabling the MS8607 humidity sensor heater. Error code = ");
    Serial.println(err);
    Serial.println("Freezing.");
    while (1);
  }

  myLog.println("Temperature (C), Pressure (mbar), Humidity (%RH), Light (lux),"); //headers for columns in the log



/* Disabled due to lack of cables while testing
//waiting for button press
  Serial.println("Set-up complete");
  Serial.println("Press button to start loop");
  while (button.isPressed()== false){
    delay(10);}
  Serial.println("Starting loop");
*/


}   //Bracket to close void setup
//setup complete, starting loop
void loop() {   //All these things are done in a loop

/* Disabled LED due to lack of cables while testing
//flash LED
  button.LEDon(brightness);
  delay(100);
  button.LEDoff();
*/


//take and report temperature reading
  float temperature = barometricSensor.getTemperature();
    Serial.print("Temperature= ");
    Serial.print(temperature);
    Serial.print(" C  ");


//take and report pressure reading  
  float pressure = barometricSensor.getPressure();
    Serial.print(" Pressure= ");
    Serial.print(pressure);
    Serial.print(" mbar ");

//take and report humidity reading
  float humidity = barometricSensor.getHumidity();
    Serial.print("Humidity= ");
    Serial.print(humidity);
    Serial.print("(%RH) ");


//take and report light reading
  luxVal = light.readLight();   //read value from sensor and save it as luxVal
    Serial.print("Ambient Light Reading: ");    //print to serial monitor
    Serial.print(luxVal);   //Print the value that was just saved as luxVal
    Serial.println(" Lux");   // Added to show units in monitor


//Record data to log
  myLog.print(temperature);
  myLog.print(",  ");
  myLog.print(pressure);
  myLog.print(",  ");
  myLog.print(humidity);
  myLog.print(",  ");  
  myLog.print(luxVal);
  myLog.println();
  myLog.syncFile();

//wait to start next cycle
  delay(1000);    //Wait 1 second (1000 milliseconds

  
}   //Bracket to close void loop
