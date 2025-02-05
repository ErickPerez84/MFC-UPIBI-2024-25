#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include <SD.h>

// Create an INA219 instance
Adafruit_INA219 ina219;

// SD card settings
const int chipSelect = 9;  // Chip select pin for your SD card module

void setup() {
  Serial.begin(9600);

  // Initialize the INA219 sensor
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1);
  }
  // Configure INA219 calibration
  ina219.setCalibration_300mV_100uA(); // Default calibration
  Serial.println("INA219 ready.");

  // Initialize SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);  // Halt the program if SD card initialization fails
  }
  Serial.println("SD card ready.");
}

void loop() {
  // Read sensor values
  float shuntVoltage = ina219.getShuntVoltage_mV();  // Shunt voltage in uV
  float busVoltage = ina219.getBusVoltage_mV(); // Bus voltage in uV
  float current = ina219.getCurrent_uA();            // Current in uA
  float power = shuntVoltage * current;          // Power in uW

  // Debugging: Print the values to Serial Monitor
  Serial.print("Shunt Voltage: ");
  Serial.print(shuntVoltage);
  Serial.print(" uV, Bus Voltage: ");
  Serial.print(busVoltage);
  Serial.print(" uV, Current: ");
  Serial.print(current);
  Serial.print(" uA, Power: ");
  Serial.print(power);
  Serial.println(" uW");

  Serial.print("Shunt Voltage Raw: ");
  Serial.println(ina219.getRawShuntVoltage());
  Serial.print("Bus Voltage Raw: ");
  Serial.println(ina219.getRawBusVoltage());
  Serial.print("Current Raw: ");
  Serial.println(ina219.getRawCurrent());

  // Log the readings (time, voltage, current, power) to SD card
  Serial.println("Logging data to SD card...");
  logDataToSD(millis(), shuntVoltage, busVoltage, current, power);

  // Wait for 5 minutes (300,000 ms)
  delay(3000);
}

void logDataToSD(unsigned long time, float svoltage, float bvoltage, float current, float power) {
  File dataFile = SD.open("data.csv", FILE_WRITE);

  // Check if the file opened correctly
  if (dataFile) {
    Serial.println("File opened successfully.");

    // Write time, voltage (uV), current (uA), and power (uW) to SD card
    dataFile.print(time / 60000.0);  // Time in minutes
    dataFile.print(",min,");
    dataFile.print(svoltage);
    dataFile.print(",uV,");
    dataFile.print(bvoltage);
    dataFile.print(",uV,");
    dataFile.print(current);
    dataFile.print(",uA,");
    dataFile.print(power);
    dataFile.println(",uW");

    dataFile.flush();  // Ensure data is written
    dataFile.close();  // Close the file to save resources
    Serial.println("Data written to SD card.");
  } else {
    Serial.println("Error opening file!");
  }
}
