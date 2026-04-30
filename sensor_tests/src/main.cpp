/*
 * ADXL345 Accelerometer Test — ESP32-S3 DevKitC-1
 * 
 * Wiring (as per circuit_guide.md):
 *   ADXL345 VCC  → ESP32 3.3V
 *   ADXL345 GND  → ESP32 GND
 *   ADXL345 SDA  → ESP32 GPIO8
 *   ADXL345 SCL  → ESP32 GPIO9
 *   ADXL345 I2C Address: 0x53
 *
 * This sketch reads X, Y, Z acceleration values and prints them
 * to Serial at 115200 baud. It also computes vibration magnitude RMS.
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Custom I2C pins for ESP32-S3 (matching actual wiring)
#define I2C_SDA 8
#define I2C_SCL 9

// Create sensor instance with unique ID
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void displaySensorDetails() {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("============================================");
  Serial.println("       ADXL345 Accelerometer Test");
  Serial.println("============================================");
  Serial.print("Sensor:       "); Serial.println(sensor.name);
  Serial.print("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("============================================");
}

void displayDataRate() {
  Serial.print("Data Rate:    ");
  switch (accel.getDataRate()) {
    case ADXL345_DATARATE_3200_HZ: Serial.print("3200"); break;
    case ADXL345_DATARATE_1600_HZ: Serial.print("1600"); break;
    case ADXL345_DATARATE_800_HZ:  Serial.print("800");  break;
    case ADXL345_DATARATE_400_HZ:  Serial.print("400");  break;
    case ADXL345_DATARATE_200_HZ:  Serial.print("200");  break;
    case ADXL345_DATARATE_100_HZ:  Serial.print("100");  break;
    case ADXL345_DATARATE_50_HZ:   Serial.print("50");   break;
    case ADXL345_DATARATE_25_HZ:   Serial.print("25");   break;
    case ADXL345_DATARATE_12_5_HZ: Serial.print("12.5"); break;
    case ADXL345_DATARATE_6_25HZ:  Serial.print("6.25"); break;
    default: Serial.print("??"); break;
  }
  Serial.println(" Hz");
}

void displayRange() {
  Serial.print("Range:        +/- ");
  switch (accel.getRange()) {
    case ADXL345_RANGE_16_G: Serial.print("16"); break;
    case ADXL345_RANGE_8_G:  Serial.print("8");  break;
    case ADXL345_RANGE_4_G:  Serial.print("4");  break;
    case ADXL345_RANGE_2_G:  Serial.print("2");  break;
    default: Serial.print("??"); break;
  }
  Serial.println(" g");
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // Wait for serial to stabilize
  
  Serial.println();
  Serial.println("Starting ADXL345 Test on ESP32-S3...");
  Serial.print("I2C Pins: SDA=GPIO"); Serial.print(I2C_SDA);
  Serial.print(", SCL=GPIO"); Serial.println(I2C_SCL);
  Serial.println();

  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // I2C Scanner — check what's on the bus
  Serial.println("--- I2C Bus Scan ---");
  int devicesFound = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("  Found device at 0x");
      if (addr < 16) Serial.print("0");
      Serial.print(addr, HEX);
      if (addr == 0x53) Serial.print(" ← ADXL345");
      if (addr == 0x3C) Serial.print(" ← OLED SSD1306");
      Serial.println();
      devicesFound++;
    }
  }
  if (devicesFound == 0) {
    Serial.println("  ⚠ No I2C devices found! Check wiring.");
  }
  Serial.println("--------------------");
  Serial.println();

  // Initialize ADXL345
  if (!accel.begin()) {
    Serial.println("❌ ERROR: No ADXL345 detected!");
    Serial.println("   Check your wiring:");
    Serial.println("   - VCC → 3.3V");
    Serial.println("   - GND → GND");
    Serial.println("   - SDA → GPIO8");
    Serial.println("   - SCL → GPIO9");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("✅ ADXL345 detected successfully!");
  Serial.println();

  // Configure for fan vibration monitoring
  accel.setRange(ADXL345_RANGE_16_G);
  accel.setDataRate(ADXL345_DATARATE_100_HZ);

  displaySensorDetails();
  displayDataRate();
  displayRange();
  Serial.println();
  Serial.println("--- Live Readings (every 500ms) ---");
  Serial.println("   X (m/s²) | Y (m/s²) | Z (m/s²) | Mag (m/s²)");
  Serial.println("   ---------|----------|----------|----------");
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;
  float mag = sqrt(x*x + y*y + z*z);

  Serial.print("   ");
  Serial.print(x, 2); Serial.print("\t | ");
  Serial.print(y, 2); Serial.print("\t | ");
  Serial.print(z, 2); Serial.print("\t | ");
  Serial.print(mag, 2);
  Serial.println(" m/s²");

  delay(500);
}
