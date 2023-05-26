#ifndef WifiAccessPoint_h
#define WifiAccessPoint_h

#include <Arduino.h>

class WifiAccessPoint
{
  public:
    WifiAccessPoint();
    bool setupWifiConnection(const char *ssid, const char *password);
    char requestDollicloud(String productRef);
    // char findByProductRef(String productRef);
    
  private:
};

#endif
