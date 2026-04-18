#include <WiFi.h>

const char* wlanSSID="xxx";
const char* wlanPASS="xxx";

void initWLAN() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wlanSSID,wlanPASS); }

void wlanWorker() {
  static uint32_t wlanTimer=10000,dacAvailableOld;
  if (millis()>=wlanTimer) { wlanTimer=millis()+1000;
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());
    Serial.print("Heap Total: "); Serial.print(ESP.getHeapSize());
    Serial.print("   Free: "); Serial.print(ESP.getFreeHeap());
    Serial.print("   Used: "); Serial.println(ESP.getHeapSize()-ESP.getFreeHeap());
    Serial.print("PSRAM Total: "); Serial.print(ESP.getPsramSize());
    Serial.print("   Free: "); Serial.print(ESP.getFreePsram());
    Serial.print("   Used: "); Serial.println(ESP.getPsramSize()-ESP.getFreePsram());
    Serial.print("DAC available: "); Serial.println(dacAvailable-dacAvailableOld);
    dacAvailableOld=dacAvailable;
    getADC(); } }
