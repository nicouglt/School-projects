

#include <Arduino.h>
#include "RfidReader.h"

#include <M5Stack.h>

#include "MFRC522_I2C.h"

MFRC522 mfrc522(0x28); // Create MFRC522 instance.
String UID;

RfidReader::RfidReader()
{
  // SetupRfid();
  //  Se servirde ce bloc comme setup
}

void RfidReader::setupRfid()
{
  Serial.begin(9600);
  M5.Lcd.println("Initialisation du lecteur rfid");
  Wire.begin(); // Wire init, adding the I2C bus.  Wire

  mfrc522.PCD_Init(); // Init MFRC522.
}

String RfidReader::readRfcTag()
{
  M5.Lcd.println("Please put the card\n");
  mfrc522.PCD_Init(); // Init MFRC522.
  UID = "";
  // M5.Lcd.setCursor(30, 0);
  if (!mfrc522.PICC_IsNewCardPresent() ||
      !mfrc522.PICC_ReadCardSerial())
  { //
    delay(100);
   // return UID;
  }


  UID="0";
  // M5.Lcd.fillRect(42, 47, 320, 20, BLACK);
  for (byte i = 0; i < mfrc522.uid.size;
        i++)
  { // Output the stored UID data.
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    // UID = UID + Serial.print(mfrc522.uid.uidByte[i], HEX);
      //M5.Lcd.println(mfrc522.uid.uidByte[i], HEX);
    // M5.Lcd.print(mfrc522.uid.uidByte[i]);
    //UID = UID + mfrc522.uid.uidByte[i];
    UID = UID + String(mfrc522.uid.uidByte[i], HEX);
  }


    UID.toUpperCase();
  //Serial.println(UID);
    // M5.Lcd.print("NFC tag lu avec succès, UID : ");
    // M5.Lcd.println(UID);
    // Serial.println(UID);
    /*
    if(UID.isEmpty())
      M5.Lcd.println("Aucun uid à scanner");
      */

    if(UID == "0") return "";

    return UID;
}
