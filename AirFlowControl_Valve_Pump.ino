
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
#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

//////////////////////////
// LiPo Fuel Gauge Setup
//////////////////////////
SFE_MAX1704X lipo; // MAX17043/48

//////////////////////////
// NO Sensor Setup
//////////////////////////
const int ref_channel = 4;  // Reference sensor pin
const int gas_channel = 3;  // Gas sensor pin
float calibration_factor = 0.0025; // Adjust after calibration
const float ADC_REF_VOLTAGE = 3.3; // ESP32 ADC reference voltage
const int ADC_RESOLUTION = 4095;   // 12-bit ADC

//////////////////////////
// Solenoid/PWM Setup
//////////////////////////
const int solenoidPin = 2;    // Solenoid control pin
const int pwmChannel = 0;     // PWM channel
const int pwmFreq = 1000;     // 1 kHz
const int pwmResolution = 8;  // 8-bit resolution

//////////////////////////
// Function for averaging ADC readings
//////////////////////////
int readADCaverage(int pin, int samples = 10, int delayMs = 5) {
  long sum = 0;
  for(int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delay(delayMs);
  }
  return sum / samples;
}

//////////////////////////
// Setup
//////////////////////////
void setup() {
  Serial.begin(115200);
  while (!Serial);

  //////////////////////////
  // LiPo Initialization
  //////////////////////////
  Wire.begin();
  if (!lipo.begin()) {
    Serial.println("MAX17043 not detected. Check wiring.");
    while (1);
  }
  lipo.quickStart();
  lipo.setThreshold(20); // Alert threshold 20%

  //////////////////////////
  // Solenoid/PWM Initialization
  //////////////////////////
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(solenoidPin, pwmChannel);
}

//////////////////////////
// Loop
//////////////////////////
void loop() {

  //////////////////////////
  // --- NO Sensor Reading ---
  //////////////////////////
  // IR ON phase
  int vref_on_adc = readADCaverage(ref_channel);
  int vgas_on_adc = readADCaverage(gas_channel);

  delay(100); // IR OFF phase delay

  int vref_off_adc = readADCaverage(ref_channel);
  int vgas_off_adc = readADCaverage(gas_channel);

  // Convert ADC to voltage
  float vref_on  = vref_on_adc  * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
  float vgas_on  = vgas_on_adc  * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
  float vref_off = vref_off_adc * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
  float vgas_off = vgas_off_adc * (ADC_REF_VOLTAGE / ADC_RESOLUTION);

  // Compute delta and NO ppm
  float delta_on  = vref_on - vgas_on;
  float delta_off = vref_off - vgas_off;
  float net_delta = delta_on - delta_off;
  float no_ppm = net_delta / calibration_factor;

  //////////////////////////
  // --- LiPo Reading ---
  //////////////////////////
  float voltage = lipo.getVoltage();
  float soc = lipo.getSOC();
  bool alert = lipo.getAlert();

  //////////////////////////
  // --- Solenoid Control Example ---
  //////////////////////////
  int duty = 255; // 100% ON
  ledcWrite(pwmChannel, duty);
  delay(2000);
  duty = 0; // OFF
  ledcWrite(pwmChannel, duty);
  delay(2000);

  //////////////////////////
  // --- Serial Output ---
  //////////////////////////
  Serial.println("=== NO Sensor ===");
  Serial.print("NO Concentration (ppm): ");
  Serial.println(no_ppm);

  Serial.println("=== LiPo Status ===");
  Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
  Serial.print("SOC: "); Serial.print(soc); Serial.println(" %");
  Serial.print("Alert: "); Serial.println(alert);

  Serial.println("=== Solenoid Status ===");
  Serial.print("Duty Cycle: "); Serial.println(duty);

  Serial.println("------------------------------------");
}

