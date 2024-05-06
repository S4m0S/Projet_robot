#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <time.h>
#include <Servo.h>

//SETUP 

Adafruit_MPU6050 mpu;

Servo servoDroit;
Servo servoGauche;


float angle_voulu = -1.4;
float angle_actuelle = 0;

float vitesse_moteur = 0;
float difference = 0;

int correction = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if(!mpu.begin()){
    Serial.println("Capteur mal initalisé");

  }

  Serial.println("Capteur trouvé");


  servoDroit.attach(11);
  servoGauche.attach(10);
  servoGauche.write(90);
  servoDroit.write(90);


  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  


  angle_actuelle = a.acceleration.z;

  difference = (angle_voulu-angle_actuelle);

  vitesse_moteur = difference/25*90;

  if(difference<-0.5 && difference >-1.5){
    vitesse_moteur= vitesse_moteur/5;
  }
  else{
    //vitesse_moteur=vitesse_moteur*2;
  }



  if(vitesse_moteur<10 && vitesse_moteur>-10){
    servoGauche.write(92);
    servoDroit.write(90);
  }
  else{
    servoGauche.write(90+vitesse_moteur);
    servoDroit.write(90-vitesse_moteur);
  }

  delay(40);
  

  


}