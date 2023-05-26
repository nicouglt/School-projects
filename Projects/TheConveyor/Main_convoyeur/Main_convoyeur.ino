// Inclure vos librairies les bibliothèques dans la section correspondante
// Général : 
#include <Arduino_JSON.h>
#include <M5Stack.h>

// ServoMoteur :

// Moteur pas à pas :

// Serveur et communication :
#include "RfidReader.h"
#include "WifiAccessPoint.h"
#include "Switcher.h"
#include "StepMotor.h"

// Initaliser et déclarer vos composants dans cette section
    
// Déclarer ici les variables générales 
String nfcTag = "";
String newNfcTag = "";    
char Address = 'Z';
bool success = 0;

// Il faudra initialiser les bibliothèques ici
WifiAccessPoint wifiAccessPoint;
RfidReader rfidReader;
Switcher switcher;
StepMotor stepmotor;
  
// Sauf exception, toutes les constantes sont censées être déclarées dans vos bibliothèques
#define STEP 2000    // Nombre de pas que fera le moteur par boucle. A modifier selon le besoin
  
void setup() {

  M5.begin();
  M5.Power.begin();  

  while (success != 1) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    success = wifiAccessPoint.setupWifiConnection("Remiwifi", "remistiti");
    // success = wifiAccessPoint.setupWifiConnection("DESKTOP-NM6KC25 8243", "Epitech123");
  }

  rfidReader.setupRfid();
  stepmotor.Forward(STEP);
  M5.Lcd.println("Fin du setup");
  sleep(5);

}

void loop()
{  
  Address = 'Z';

  while(nfcTag == newNfcTag)
  {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    newNfcTag = rfidReader.readRfcTag();
    // newNfcTag = "046D43DAE86B80";
  }

  nfcTag = newNfcTag;

  Address = wifiAccessPoint.requestDollicloud(nfcTag);

  if (Address != 'Z') switcher.SetPosition(Address);

  M5.Lcd.println("Fin de la loop");
  sleep(5);
}
