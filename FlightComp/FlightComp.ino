//---- Included Libraries ----//
#include <Wire.h>                           // I²C library
#include <math.h>                           // trig functions
#include <Adafruit_Sensor.h>                // Base library for sensors
#include <Adafruit_BNO055.h>                // BNO055 (IMU) specific lib
#include <Adafruit_MPL3115A2.h>             // MPL3115A2 (Barometer) specic lib
#include <utility/imumaths.h>               // Vector, Matrix, and IMUMath library

#include <XLR8Float.h>                      // XLR8 accelerated floating point math

#define BNO055_SAMPLERATE_DELAY_MS (50)     // Set pause between samples

//---- Variable Declaration ----//

boolean debug = false;                       // true/false = extra/no information over serial

                        // Digital pin for pitch

float roll, pitch, yaw;                     // Variable to hold roll, pitch, yaw information

Adafruit_BNO055 bno = Adafruit_BNO055();        // Use object var for IMU information
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2(); // Use object var for Baro information


float varVolt = 1.12184278324081E-05;  // variance determined using excel and reading samples of raw sensor data
float varProcess = 1e-8;
float Pc = 0.0;
float G = 0.0;
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;


void setup(void) {


  Serial.begin(115200);                     // Create serial connection at 115,000 Baud

  if (!bno.begin())                         // Attempt communication with sensors 
  {Serial.print("Ooops, no IMU detected!");}
  if (!baro.begin())                        
  {Serial.print("Ooops, no Barometer detected!");}

  delay(100);                               // Wait 0.1 seconds to allow it to initialize
  bno.setExtCrystalUse(true);               // Tell sensor to use external crystal
}

//---- Main Program Loop ----//
void loop() {

  //---- Request Euler Angles from Sensor ----//
  //imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  //----Request for measurement Barometer ----// 
  float pascals = baro.getPressure();
  float altm = baro.getAltitude();
  float tempC = baro.getTemperature();

   // kalman process
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);    // kalman gain
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(tempC-Zp)+Xp;   // the kalman estimate of the sensor voltage

  if (debug) {                             // If debug is true, send information over serial
//    Serial.print("Measured Euler Roll-Pitch-Yaw");
//    Serial.print(" y :"); Serial.print(euler.x());
//    Serial.print("/ p :"); Serial.print(euler.z());
//    Serial.print("/ r "); Serial.print(euler.y());
    // Printing data Baro to serial 
    Serial.print("/ (Hg) "); Serial.print(pascals/3377);   // Inches (Hg)
    Serial.print("/ m "); Serial.print(altm);              // Meters 
    Serial.print("/ *C "); Serial.println(tempC);          //Temperture
  }
  
  if(!debug){
    Serial.println(Xe);
    Serial.println(altm);
    //Serial.println(Xe);
    
    }
  
  // If debug is true, send information over serial
  

  //delay(BNO055_SAMPLERATE_DELAY_MS);       // Wait before rerunning loop
  delay(10);
}
