
#include <heltec.h>
#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
#define SERIALON

SFE_MAX1704X lipo; // Defaults to the MAX17043
const int solenoid1Pin=2;
//const int solenoid2Pin=37;

const int pwmFreq=1000;
const int pwmResolution=8;
void activateAirFlow();
void 
void setup() {

  //pinMode(solenoid1Pin,OUTPUT);//intake
  //pinMode(solenoid2Pin,OUTPUT);//outtake

  //Set initial states
  //digitalWrite(solenoid1Pin,HIGH);
  //digitalWrite(solenoid2Pin,LOW);

  //Set up serial communication baudrate to 115200

  Serial.begin(115200);

  

  // Attach PWM channel to solenoid1Pin
  ledcAttach(solenoid1Pin, pwmFreq, pwmResolution);
}




void loop() {
  //digitalWrite(solenoid1Pin, LOW);  // turn solenoid ON by pulling gate of PMOSFET to LOW 
  //digitalWrite(solenoid2Pin, HIGH);
  //delay(20000);
  
  //digitalWrite(solenoid1Pin, HIGH);
  //digitalWrite(solenoid2Pin, LOW);
  // Print a message to the Serial Monitor
 // Turn on the air pump
  //delay(20000); // Wait for the specified duration
  int duty = 255;
  Serial.println("Setting duty cycle to 100%");
  ledcWrite(solenoid1Pin, duty);
  delay(2000);
  duty = 0;
  Serial.println("Setting duty cycle to s0%");
  ledcWrite(solenoid1Pin, duty);
  delay(2000);

  
}

