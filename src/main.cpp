#include <stdio.h>
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <time.h>
#include <Servo.h>
void pid(void);


#define nb_valeur_tableau 40

//SETUP 

Adafruit_MPU6050 mpu;

Servo servoDroit;
Servo servoGauche;


float correcteur_Kp = 25;        // correcteur à action proportionelle; correcteur 'P'

float correcteur_Ki = 3;        // correcteur à action intégrale; correcteur 'I'

float correcteur_Kd = 20;        // correcteur à action dérivée; correcteur 'D'      Dérivée de l'erreur donc ce calcul grâce à l'acceleration


float Es = 0;                   // Erreur du système E(s)

float valeur_cible = -0.1;         // Valeur en degrès cible pour que le pendule soit droit 

float tableau_erreur[nb_valeur_tableau];       // tableau des erreurs; sert pour l'intégration

int valeur_tableau_i = 0;       // index de la valeur actuelle du tableau

float somme_tableau = 0;        // somme pour l'intégrations

float angle_mesurer = 0;         // angle actuelle du robot

float correction = 0;           // valeur de la correction à passer aux moteurs
float delta_temps = 0.1;          // temps necessaire à faire 40 valeurs de moyenne

float acceleration_z = 0;

time_t temps_depart;


float erreur_integre = 0;       // somme de toutes les erreurs

float vitesse_ordonne = 0;

float systeme_conversion = 0.2;


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

  for(int i = 0;i<nb_valeur_tableau;i++){
    tableau_erreur[i] = 0;
  }
  time(&temps_depart);
}

void loop() {
  // put your main code here, to run repeatedly:


  pid();
  delay(100);


}










void pid(void){
    
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    angle_mesurer = a.acceleration.z;
    acceleration_z = g.gyro.x;


    // Mise a jour du tableau de valeur et de la moyenne
    Es = valeur_cible-angle_mesurer;    

    somme_tableau-= tableau_erreur[valeur_tableau_i];
    somme_tableau+= Es;

    tableau_erreur[valeur_tableau_i] = Es;
    valeur_tableau_i = (valeur_tableau_i++)%nb_valeur_tableau;

    erreur_integre +=Es*delta_temps;

    // calcul de la prochaine correction

    correction = correcteur_Kp * Es + correcteur_Ki * erreur_integre + correcteur_Kd*acceleration_z;

    Serial.println(correction);

    vitesse_ordonne = correction * systeme_conversion;

    servoGauche.write(90+vitesse_ordonne);
    servoDroit.write(90-vitesse_ordonne);
}

