#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <NewPing.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "SoftwareSerial.h"

#ifndef HAVE_HWSERIAL1
  SoftwareSerial Serial1(RX1, TX1); // Declaration de l'esp8266
#endif

const char ssid[] = "XXXXX"; //SSID et PSWD wifi
const char pass[] = "XXXXX";
int status = WL_IDLE_STATUS;
IPAddress server(172,20,10,2); //IP de l'api

WiFiEspClient client; // initialisation de la librairie

#define TriggerPIN  7
#define EchoPIN     6

#define ledSonar    2   //Declaration des leds correspondant aux capteurs
#define ledGyro     3
#define ledLum      4
#define ledWater    5

int distance = 0;  // Declaration de la variable distance qui sert pour la fonction readDisatance
int waterlvl = 0;

boolean stateMPU; 
boolean oldStateMPU = false;

boolean stateSonar;
boolean oldStateSonar = false;

boolean stateLum;
boolean oldStateLum = false;

boolean stateWater;
boolean oldStateWater = false;


Adafruit_MPU6050 mpu;       //Declaration du gyroscope
NewPing sonar (TriggerPIN, EchoPIN, 6);    //Declaration du capteur ultrasons
         

void setup() {
  pinMode(ledSonar, OUTPUT);  //Declaration des modes de fonctionnement des leds
  pinMode(ledGyro, OUTPUT);
  pinMode(ledLum, OUTPUT);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.init(&Serial1);
  
  Serial.println("Tentative de connexion..."); //Connexion de l'esp au wifi
  Serial.print("SSID: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, pass);

  if(status == 1)
  {
    Serial.println("Connexion reussie");
  }
  delay(500);
  
  Serial.println(F("Initialisation du gyroscope"));
  if(!mpu.begin()){  //Verifie que le gyro est bien détecté
    while(1) {
      Serial.println("Failed to find chip. please retry");
      delay(1000);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G); //initialise les données du gyro
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  readMPU();
  delay(100);
  readDistance();
  delay(100);
  readLuminosity();
  delay(100);
  readWaterlvl();
  delay(100);
}


//Fonction qui lit les donnees du gyro 
void readMPU(){         
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: ");
  Serial.println(a.acceleration.x);
  Serial.print("Accelaration Y: ");
  Serial.println (a.acceleration.y);
  
  if(a.acceleration.x == 0 && a.acceleration.y == 0){
    Serial.println("Error sensor");
    api(1,666);
    return;
  }else{
    if(a.acceleration.x < -2 || a.acceleration.x > 2 || a.acceleration.y < -2 || a.acceleration.y > 2){
      Serial.println("c'est vide waw");
      digitalWrite(ledGyro,HIGH);
      stateMPU = true;
    }else{
      digitalWrite(ledGyro,LOW);
      stateMPU = false;
    }
  
    if(stateMPU != oldStateMPU){
      api(1,stateMPU);
    }
  
    oldStateMPU = stateMPU;
  }
}


//Fonction qui lit les donnees du capteur ultrasons
void readDistance(){
  distance = sonar.ping_cm();
  Serial.print("Distance = ");
  Serial.println(distance);

  if(distance == 0){
    Serial.println("Error Distance Sensor");
    api(2,666);
    return;
  }else{
    if (distance < 5){
      digitalWrite(ledSonar,HIGH);
      Serial.println("Trash full");
      stateSonar = true;
    }else{
      digitalWrite(ledSonar,LOW);
      stateSonar = false;
    }
  
    if(stateSonar != oldStateSonar){
      api(2,stateSonar);
    }
    oldStateSonar = stateSonar;
  }
}

//Fonction qui lit les donnés de la photoresistance
void readLuminosity(){
  int lumos = analogRead(A0);

    if(lumos > 30){
      digitalWrite(ledLum, HIGH);
      stateLum = true;
    }else{
      digitalWrite(ledLum, LOW);
      stateLum = false;
    }
  
    if(stateLum != oldStateLum){
      api(3,stateLum);
    }
  
    oldStateLum = stateLum;
  }
}


void readWaterlvl(){
  waterlvl = analogRead(A1);

  if(waterlvl == 0){
    Serial.println("Error Waterlvl");
    api(4,666);
    return;
  }else{
    if(waterlvl >= 4){
    digitalWrite(ledWater, HIGH);
    stateWater = true;
    }else{
      digitalWrite(ledWater, LOW);
      stateWater = false;
    }

    if(stateWater != oldStateWater){
    api(4,stateWater);
    }

    oldStateWater = stateWater;
  }
}

void api(int id,int count)
{
    // if you get a connection, report back via serial:
    Serial.println("Connecting to api...");
    if (client.connect(server, 1880))
    {
      Serial.println("connected to api");
      Serial.println("Envoi de la requete " + id);
      client.println("GET /sensor?id="+String(id)+"&count="+String(count)+"&key=424242 HTTP/1.1");
      client.println("Connection: close");
      client.println("");
      
      if (client.println() != 0) {
        Serial.println("Failed to send request");
      }
      else
      {
        Serial.println("Request sent successfully");
      }

      if(!client.connected())
      {
        // if the server's disconnected, stop the client:
        Serial.println("disconnected");
        client.stop();
      }
    }
    else
    {
      Serial.println("connection failed");
    }
}
