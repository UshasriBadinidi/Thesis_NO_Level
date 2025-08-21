// Pin definitions
const int ref_channel = 4;  // Reference sensor analog pin
const int gas_channel = 3;  // Gas sensor analog pin

// ADC settings
const float ADC_REF_VOLTAGE = 3.3;  // LoRa32/ESP32 reference voltage
const int ADC_RESOLUTION = 4095;    // 12-bit ADC

void setup() {
  Serial.begin(115200); // Fast baud for ESP32
  delay(1000);          // Wait for serial to stabilize
  // CSV header
  Serial.println("Timestamp(ms),Vref_ADC,Vref_V,Vgas_ADC,Vgas_V");
}

void loop() {
  unsigned long timestamp = millis();

  // Read ADC values
  int vref_adc = analogRead(ref_channel);
  int vgas_adc = analogRead(gas_channel);

  // Convert to voltage
  float vref_volt = vref_adc * (ADC_REF_VOLTAGE / ADC_RESOLUTION);
  float vgas_volt = vgas_adc * (ADC_REF_VOLTAGE / ADC_RESOLUTION);

  // Print CSV line: timestamp, raw ADC, voltage
  Serial.print(timestamp);
  Serial.print(",");
  Serial.print(vref_adc);
  Serial.print(",");
  Serial.print(vref_volt, 3);
  Serial.print(",");
  Serial.print(vgas_adc);
  Serial.print(",");
  Serial.println(vgas_volt, 3);

  delay(1000); // 1-second interval
}
