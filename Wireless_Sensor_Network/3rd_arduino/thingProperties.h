// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)

void onMessageeChange();
void onBrightnessChange();

String messagee;
int brightness;

void initProperties(){

  ArduinoCloud.addProperty(messagee, READWRITE, ON_CHANGE, onMessageeChange);
  ArduinoCloud.addProperty(brightness, READWRITE, ON_CHANGE, onBrightnessChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);