#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
#include "SPIFFS.h"
#include "heltec.h"
// NO sensor ADC pins
const int ref_channel = 4;
const int gas_channel = 3;


// Fuel gauge object
SFE_MAX1704X lipo;

unsigned long sampleCount = 0;
unsigned long sampleInterval = 1000; // 1 second
const float voltageAlertThreshold = 3.3; // Voltage alert threshold

const char* filename = "/nosensor_samples_data.csv";

void setup() {
  Heltec.begin(false /*Display OFF*/, false /*LoRa*/, true /*Serial*/);
  Serial.println("Board alive!");
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    while(1); // fail-safe
  }

  // Initialize I2C fuel gauge
  Wire.begin(41,42);
  if (!lipo.begin()) while(1);

  lipo.quickStart();
  

  // Create CSV header if file doesn't exist
  if(!SPIFFS.exists(filename)){
    File file = SPIFFS.open(filename, FILE_WRITE);
    if(file){
      file.println("Timestamp(ms),Vref_ADC,Vref_V,Vgas_ADC,Vgas_V,Batt_V,Batt_SOC,VoltageAlert,Sample_count");
      file.close();
    }
  }
}

void loop() {
  unsigned long timestamp = millis();

  // Read NO sensor ADC values
  int vref_adc = analogRead(ref_channel);
  int vgas_adc = analogRead(gas_channel);
  float vref_volt = vref_adc * (3.3 / 4095.0);
  float vgas_volt = vgas_adc * (3.3 / 4095.0);

  // Read battery voltage and SOC
  float voltage = lipo.getVoltage();
  float soc = lipo.getSOC();
  
  Serial.println(voltage);
  // Voltage alert
  bool voltageAlert = (voltage <= voltageAlertThreshold);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  // LED alert
  if(voltageAlert){
    digitalWrite(LED_BUILTIN,HIGH);
    while(1);
  }
  
  
  sampleCount++;

  // Write CSV data to flash
  File file = SPIFFS.open(filename, FILE_APPEND);
  if(file){
    file.printf("%lu,%d,%.4f,%d,%.4f,%.3f,%.1f,%d,%lu\n",
                timestamp,
                vref_adc, vref_volt,
                vgas_adc, vgas_volt,
                voltage, soc,
                voltageAlert,
               
                sampleCount);
    file.close();
  }

  delay(sampleInterval);
}
