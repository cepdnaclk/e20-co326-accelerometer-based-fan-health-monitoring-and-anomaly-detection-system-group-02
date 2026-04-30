#include <Wire.h>

#define MPU9250_ADDR 0x68
#define SDA_PIN 8
#define SCL_PIN 9

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Serial.begin(115200);
  
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Starting MPU9250 test...");

  // Wake up MPU9250
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x6B);   // Power management register
  Wire.write(0x00);   // Wake up sensor
  Wire.endTransmission(true);

  delay(100);

  Serial.println("MPU9250 initialized");
}

void loop() {

  // -------- Read Accelerometer --------
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x3B); // ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDR, 6, true);

  ax = (Wire.read() << 8 | Wire.read());
  ay = (Wire.read() << 8 | Wire.read());
  az = (Wire.read() << 8 | Wire.read());

  // -------- Read Gyroscope --------
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x43); // GYRO_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDR, 6, true);

  gx = (Wire.read() << 8 | Wire.read());
  gy = (Wire.read() << 8 | Wire.read());
  gz = (Wire.read() << 8 | Wire.read());

  // -------- Print Data --------
  Serial.print("ACC -> ");
  Serial.print("AX: ");
  Serial.print(ax);
  Serial.print("  AY: ");
  Serial.print(ay);
  Serial.print("  AZ: ");
  Serial.println(az);

  Serial.print("GYRO -> ");
  Serial.print("GX: ");
  Serial.print(gx);
  Serial.print("  GY: ");
  Serial.print(gy);
  Serial.print("  GZ: ");
  Serial.println(gz);

  Serial.println("-----------------------------");

  delay(500);
}
