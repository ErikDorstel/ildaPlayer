#include "DAC.h"
#include "PALETTE.h"
#include "SDCARD.h"
#include "WLAN.h"
#include "SERVER.h"

void setup() {
  Serial.begin(115200);
  initDAC();
  initSD();
  initWLAN();
  initServer();
  readILDA("/3D Tour/Kugel-3d.ild"); }

void loop() { dacWorker(); serverWorker(); wlanWorker(); }
