#include <SPIFFS.h>

// Pin definitions
const int ref_channel = 4;  // Reference sensor analog pin
const int gas_channel = 3;  // Gas sensor analog pin

// ADC settings
const float ADC_REF_VOLTAGE = 3.3;  // ESP32 reference voltage
const int ADC_RESOLUTION = 4095;    // 12-bit ADC



void setup() {
  Serial.begin(115200);
  delay(1000); // wait for serial to stabilize

  // Mount SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // List existing files
 

  // Create CSV file and write header if it doesn't exist
  if(!SPIFFS.exists("/sensor_data.csv")) {
    File file = SPIFFS.open("/sensor_data.csv", FILE_WRITE);
    if(file){
      file.println("Timestamp(ms),Vref_ADC,Vref_V,Vgas_ADC,Vgas_V");
      file.close();
      Serial.println("CSV file created with header");
    } else {
      Serial.println("Failed to create CSV file");
    }
  }
}

void loop() {
  unsigned long timestamp = millis();

  // Read ADC values
  int vref_adc = analogRead(ref_channel);
  int vgas_adc = analogRead(gas_channel);

  // Convert to voltage
  float vref_volt = vref_adc * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
  float vgas_volt = vgas_adc * (ADC_REF_VOLTAGE / ADC_RESOLUTION);

  // Print to Serial
  Serial.print("Timestamp: "); Serial.print(timestamp);
  Serial.print(" | Vref: "); Serial.print(vref_volt, 3);
  Serial.print(" V | Vgas: "); Serial.println(vgas_volt, 3);

  // Append to CSV
  File file = SPIFFS.open("/sensor_data.csv", FILE_APPEND);
  if(file){
    file.print(timestamp); file.print(",");
    file.print(vref_adc); file.print(",");
    file.print(vref_volt,3); file.print(",");
    file.print(vgas_adc); file.print(",");
    file.println(vgas_volt,3);
    file.close();
  } else {
    Serial.println("Failed to open CSV file for appending");
  }

  delay(1000); // 1-second interval
}
