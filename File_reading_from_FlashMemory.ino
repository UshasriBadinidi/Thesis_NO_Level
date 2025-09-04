#include <SPIFFS.h>

// Function to list file contents
void listFile() {
  File file = SPIFFS.open("/sensor_data.csv");  // Change name if different
  if (!file) {
    Serial.println("Failed to open file!");
    return;
  }

  Serial.println("---- Start of CSV ----");
  while (file.available()) {
    Serial.write(file.read()); // Print raw contents
  }
  Serial.println("\n---- End of CSV ----");
  file.close();
}

void setup() {
  Serial.begin(115200);       // Must match Serial Monitor
  delay(2000);                // Give time for Serial Monitor to open

  // Mount SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // List the file contents
  listFile();
}

void loop() {
  // Nothing here. File listing done in setup()
}
