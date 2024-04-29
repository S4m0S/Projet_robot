#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <time.h>


//SETUP 

Adafruit_MPU6050 mpu;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if(!mpu.begin()){
    Serial.println("Capteur mal initalisé");

  }

  Serial.println("Capteur trouvé");

}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t accel, gyrroscope, temp;

  mpu.getEvent(&accel, &gyrroscope, &temp);

 


  Serial.print("Gyroscope ");
  Serial.print("X: ");
  Serial.print(gyrroscope.gyro.x, 1);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(gyrroscope.gyro.y, 1);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(gyrroscope.gyro.z, 1);
  Serial.println(" rps");


}