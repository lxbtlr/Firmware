//---- Included Libraries ----//
#include <Wire.h>                           // I²C library
#include <math.h>                           // trig functions
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>                // Base library for sensors
#include <Adafruit_BNO055.h>                // BNO055 (IMU) specific lib
#include <Adafruit_MPL3115A2.h>             // MPL3115A2 (Barometer) specic lib
#include <utility/imumaths.h>               // Vector, Matrix, and IMUMath library

#include <XLR8Float.h>                      // XLR8 accelerated floating point math

#define BNO055_SAMPLERATE_DELAY_MS (50)     // Set pause between samples

//---- Variable Declaration ----//

boolean debug = false;                       // true/false = extra/no information over serial

                        // Digital pin for pitch

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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  Serial.println("card initialized.");
  if (!bno.begin())                         // Attempt communication with sensors 
  {Serial.print("Ooops, no IMU detected!");}
  if (!baro.begin())                        
  {Serial.print("Ooops, no Barometer detected!");}

  delay(100);                               // Wait 0.1 seconds to allow it to initialize
  bno.setExtCrystalUse(true);               // Tell sensor to use external crystal
  
  int measurements[570][6]
 /*
  col 1-4 = w.x.y.z
  col 5 = altm
  col 6 = tempc
  col 7 = time (millis)
 */
  word counter = 0;
  imu::Quaternion quat = bno.getQuat(); 
  String initial = str(quat.w()+","+quat.x()+","+quat.y()+","+quat.z()+baro.getAltitude()+","+baro.getTemperature()+","+millis());
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(initial);
    dataFile.close();
    Serial.println(initial);
  }  
  else {
    Serial.println("error opening datalog.txt");
  } 
}

//---- Main Program Loop ----//
void loop() {

  if( counter == 0 ){
    if(baro.getAltitude() > 4){ // if altitute is read to be greater than 4 meters, begin recording
    counter++;
    }
    else{
      continue;
    }
  }
  
  counter++;

  //---- Request Euler Angles from Sensor ----//
  //imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  
  imu::Quaternion quat = bno.getQuat();           // Request quaternion data from BNO055
  
  //----Request for measurement Barometer ----// 
  int pascals = int(baro.getPressure()*10);
  int altm = int(baro.getAltitude()*10);
  int tempC = int(baro.getTemperature()*100);

   // kalman process
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);    // kalman gain
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(tempC-Zp)+Xp;   // the kalman estimate of the sensor voltage

  //everything multiplied by 10 to add an extra bit before the . for rounding purposes
  //everything is *10 except the time.
  measurements[counter][0]= int(quat.w()*10);
  measurements[counter][1]= int(quat.x()*10);
  measurements[counter][2]= int(quat.y()*10);
  measurements[counter][3]= int(quat.z()*10);
  measurements[counter][4]= int(baro.getAltitude()*10);
  measurements[counter][5]= int(baro.getTemperature()*10);
  measurements[counter][6]= millis()

  if (debug) { // If debug is true, send information over serial
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
  // make a string for assembling the data to log:
  if(counter > 560){
    counter = 0;
    String dataString = "";

    // // read three sensors and append to the string:
    // for (int analogPin = 0; analogPin < 3; analogPin++) {
    //   int sensor = analogRead(analogPin);
    //   dataString += String(sensor);
    //   if (analogPin < 2) {
    //     dataString += ","; 
    //   }
    // }
    for (word j = 0; j < 561; j++){
      for (byte i = 0; i < 7; i++){
        dataString += str(measurements[j][i]+",");
      }
      dataString += "\n";
    }
    
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println(dataString);
    }  
    else {
      Serial.println("error opening datalog.txt");
    } 
    
  }
  //delay(BNO055_SAMPLERATE_DELAY_MS);       // Wait before rerunning loop
  delay(10);

}
