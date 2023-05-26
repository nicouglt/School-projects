#include <Arduino.h>
#include <M5Stack.h>
#include <Arduino_JSON.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include "WifiAccessPoint.h"

WiFiMulti wifiMulti;
HTTPClient http;
int httpCode;

WifiAccessPoint::WifiAccessPoint()
{
}

bool WifiAccessPoint::setupWifiConnection(const char *ssid, const char *password)
{
  wifiMulti.addAP(ssid, password);
  M5.Lcd.println("Connexion Wifi en cours ...");
  
  if ((wifiMulti.run() != WL_CONNECTED))
  {
    M5.Lcd.println("Une erreur est survenue lors de la connection wifi");
    return false;
  }
  
  M5.Lcd.println("Connexion wifi effectuée avec succès");
  return true;
}

char WifiAccessPoint::requestDollicloud(String productRef)
{
    M5.Lcd.println("Requête dollicloud en cours ...");
    // http.begin("http://10.136.77.139/dolibarr-14.0.2/htdocs/api/index.php/products/ref/" + productRef);
    // http.addHeader("DOLAPIKEY", "07phW828MxTswXZzEhKINjg79zVU46g1");
    http.begin("https://t-iot-901-2.saas2.doliondemand.fr/api/index.php/products/ref/" + productRef);
    http.addHeader("DOLAPIKEY", "e681ac2104e28a8c7119998d549f6e55de24ad97");

    httpCode = http.GET();
    Serial.println(httpCode);

    if (httpCode <= 0 || httpCode != HTTP_CODE_OK)
    {
      M5.Lcd.printf("Erreur de requête sur la référence du produit, erreur: %s\n", http.errorToString(httpCode).c_str());
      http.end();
      return 'Z';
    }

    JSONVar product = JSON.parse(http.getString());
    http.end();

    // http.begin("http://10.136.77.139/dolibarr-14.0.2/htdocs/api/index.php/warehouses/" + product["fk_default_warehouse"]);
    // http.addHeader("DOLAPIKEY", "07phW828MxTswXZzEhKINjg79zVU46g1");

    String a = "https://t-iot-901-2.saas2.doliondemand.fr/api/index.php/warehouses/";
    String b = product["fk_default_warehouse"];
    String c = a+b;
    Serial.println(c);
    http.begin(c);
    http.addHeader("DOLAPIKEY", "e681ac2104e28a8c7119998d549f6e55de24ad97");
    

    httpCode = http.GET();
    Serial.println(httpCode);

    if (httpCode <= 0 || httpCode != HTTP_CODE_OK)
    {
      M5.Lcd.printf("Erreur de requête sur la warehouse, erreur: %s\n", http.errorToString(httpCode).c_str());
      http.end();
      return 'Z';
    }
    
    M5.Lcd.printf("Requete effectuee avec succes, code: %d\n", httpCode);

    JSONVar warehouse = JSON.parse(http.getString());
    http.end();

    M5.Lcd.print("Label de la warehouse : ");
    M5.Lcd.println((const char*) warehouse["label"]);
    
    // Serial.println(warehouse);
    // Serial.println(warehouse["label"]);
    // Serial.println(warehouse["libelle"]);
    // Serial.println((const char*) warehouse["label"]);
    // Serial.println((const char*) warehouse["libelle"]);

    http.end();

    return warehouse["label"];
}
