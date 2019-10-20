//---- Included Libraries ----//
#include <Wire.h>                           // I²C library
#include <math.h>                           // trig functions
#include <Adafruit_Sensor.h>                // Base library for sensors
#include <Adafruit_BNO055.h>                // BNO055 specific library
#include <utility/imumaths.h>               // Vector, Matrix, and IMUMath library
//#include <servo.h>                        // Standard Servo library
#include <XLR8Float.h>                      // XLR8 accelerated floating point math

#define BNO055_SAMPLERATE_DELAY_MS (50)     // Set pause between samples

//---- Variable Declaration ----//

boolean debug = true;                       // true/false = extra/no information over serial

                        // Digital pin for pitch

float roll, pitch, yaw;                     // Variable to hold roll, pitch, yaw information

Adafruit_BNO055 bno = Adafruit_BNO055();    // Use object bno to hold information


void setup(void) {


  Serial.begin(115200);                     // Create serial connection at 115,000 Baud

  if (!bno.begin())                         // Attempt communication with sensor
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }

  delay(100);                               // Wait 0.1 seconds to allow it to initialize
  bno.setExtCrystalUse(true);               // Tell sensor to use external crystal
}

//---- Main Program Loop ----//
void loop() {

  //---- Request Euler Angles from Sensor ----//
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  if (debug) {                             // If debug is true, send information over serial
//    Serial.print("Measured Euler Roll-Pitch-Yaw");
    Serial.print(" y "); Serial.println(euler.x());
    Serial.print(" p "); Serial.println(euler.z());
    Serial.print(" r "); Serial.println(euler.y());
  }
  

  
  // If debug is true, send information over serial
  

  delay(BNO055_SAMPLERATE_DELAY_MS);       // Wait before rerunning loop
}
