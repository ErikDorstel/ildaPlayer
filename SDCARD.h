#include <SD.h>
File dir;
File file;

#define sdPinCS   15

uint8_t buffer[32];

void initSD() {
  SD.begin(sdPinCS); }

void readILDA(String value) {
  Serial.print("Get File: "); Serial.println(value);
  laserOff();
  ildaCount=0; paletteCount=0; dacCount=0;
  initPalette();
  file=SD.open(value);
  while (file.available()) {
    if (file.available()>=32) { file.read(buffer,32);
      uint8_t format=buffer[7];
      uint16_t count=(buffer[24]<<8)+(buffer[25]);
      if (format==0) {
        Serial.print("Format 0: "); Serial.println(count);
        for (uint16_t n=0;n<count;n++) {
          if (file.available()>=8) { file.read(buffer,8);
            ilda[ildaCount].x=((int16_t)((float)ildaSize*(int16_t)((buffer[0]<<8)+buffer[1])/100.0f)+32768)>>4;
            ilda[ildaCount].y=((int16_t)((float)ildaSize*(int16_t)((buffer[2]<<8)+buffer[3])/100.0f)+32768)>>4;
            ilda[ildaCount].s=buffer[6];
            ilda[ildaCount].r=palette[buffer[7]][0];
            ilda[ildaCount].g=palette[buffer[7]][1];
            ilda[ildaCount].b=palette[buffer[7]][2];
            if (bitRead(ilda[ildaCount].s,6)) { ilda[ildaCount].r=0; ilda[ildaCount].g=0; ilda[ildaCount].b=0; }
            ildaCount++; } } }
      if (format==1) {
        Serial.print("Format 1: "); Serial.println(count);
        for (uint16_t n=0;n<count;n++) {
          if (file.available()>=6) { file.read(buffer,6);
            ilda[ildaCount].x=((int16_t)((float)ildaSize*(int16_t)((buffer[0]<<8)+buffer[1])/100.0f)+32768)>>4;
            ilda[ildaCount].y=((int16_t)((float)ildaSize*(int16_t)((buffer[2]<<8)+buffer[3])/100.0f)+32768)>>4;
            ilda[ildaCount].s=buffer[4];
            ilda[ildaCount].r=palette[buffer[5]][0];
            ilda[ildaCount].g=palette[buffer[5]][1];
            ilda[ildaCount].b=palette[buffer[5]][2];
            if (bitRead(ilda[ildaCount].s,6)) { ilda[ildaCount].r=0; ilda[ildaCount].g=0; ilda[ildaCount].b=0; }
            ildaCount++; } } }
      if (format==2) {
        Serial.print("Format 2: "); Serial.println(count);
        for (uint16_t n=0;n<count;n++) {
          if (file.available()>=3) { file.read(buffer,3);
            palette[paletteCount][0]=buffer[0];
            palette[paletteCount][1]=buffer[1];
            palette[paletteCount][2]=buffer[2];
            paletteCount++; } } }
      if (format==4) {
        Serial.print("Format 4: "); Serial.println(count);
        for (uint16_t n=0;n<count;n++) {
          if (file.available()>=10) { file.read(buffer,10);
            ilda[ildaCount].x=((int16_t)((float)ildaSize*(int16_t)((buffer[0]<<8)+buffer[1])/100.0f)+32768)>>4;
            ilda[ildaCount].y=((int16_t)((float)ildaSize*(int16_t)((buffer[2]<<8)+buffer[3])/100.0f)+32768)>>4;
            ilda[ildaCount].s=buffer[6];
            ilda[ildaCount].b=(float)buffer[7]*(float)blueBright/(float)100;
            ilda[ildaCount].g=(float)buffer[8]*(float)greenBright/(float)100;
            ilda[ildaCount].r=(float)buffer[9]*(float)redBright/(float)100;
            if (bitRead(ilda[ildaCount].s,6)) { ilda[ildaCount].r=0; ilda[ildaCount].g=0; ilda[ildaCount].b=0; }
            ildaCount++; } } }
      if (format==5) {
        Serial.print("Format 5: "); Serial.println(count);
        for (uint16_t n=0;n<count;n++) {
          if (file.available()>=8) { file.read(buffer,8);
            ilda[ildaCount].x=((int16_t)((float)ildaSize*(int16_t)((buffer[0]<<8)+buffer[1])/100.0f)+32768)>>4;
            ilda[ildaCount].y=((int16_t)((float)ildaSize*(int16_t)((buffer[2]<<8)+buffer[3])/100.0f)+32768)>>4;
            ilda[ildaCount].s=buffer[4];
            ilda[ildaCount].b=(float)buffer[5]*(float)blueBright/(float)100;
            ilda[ildaCount].g=(float)buffer[6]*(float)greenBright/(float)100;
            ilda[ildaCount].r=(float)buffer[7]*(float)redBright/(float)100;
            if (bitRead(ilda[ildaCount].s,6)) { ilda[ildaCount].r=0; ilda[ildaCount].g=0; ilda[ildaCount].b=0; }
            ildaCount++; } } } } }
  Serial.print("Points: "); Serial.println(ildaCount);
  file.close();
  laserOn(); }

String readDir(String value) {
  Serial.print("Get Dir: "); Serial.println(value);
  colorOff();
  String result;
  dir=SD.open(value);
  if (value.length()>1) { value+="/"; }
  while (true) {
    File entry=dir.openNextFile();
    if (!entry) { break; }
    if (entry.isDirectory()) {
      result+=value; result+=entry.name(); result+=":DIR*"; }
    String name=String(entry.name()); name.toLowerCase();
    if (!entry.isDirectory() && name.endsWith(".ild")) {
      result+=value; result+=entry.name(); result+=":";
      result+=entry.size(); result+="*"; }
    entry.close(); }
  result+=value; result+="..:DIR";
  dir.close();
  return result; }
