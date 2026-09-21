#include "ADC.h"
#include "DAC.h"
#include "SOUND.h"
#include "PALETTE.h"
#include "SDCARD.h"
#include "WLAN.h"
#include "Asteroids.h"
#include "Dogfight.h"
#include "SERVER.h"

void setup() {
  Serial.begin(115200);
  initADC();
  initDAC();
  initSound();
  initSD();
  initWLAN();
  initServer();
  readILDA("/3D Tour/Kugel-3d.ild"); }

void loop() {
  if (game==1) { laserOn(); initAsteroids(); game=2; }
  if (game==2) { doAsteroids(); }
  if (game==3) { laserOn(); initDogfight(); game=4; }
  if (game==4) { doDogfight(); }
  serverWorker();
  wlanWorker();
  soundWorker(); }
