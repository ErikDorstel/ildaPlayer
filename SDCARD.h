#include <SD.h>
File dir;
File file;

#define sdPinCS   15

uint8_t buffer[100000];

void initSD() {
  SD.begin(sdPinCS); }

void readILDA(String value) {
  Serial.print("Get File: "); Serial.println(value);
  laserOn();
  initPalette();
  file=SD.open(value);
  while (file.available()) {
    if (file.available()>=32) { file.read(buffer,32);
      uint8_t format=buffer[7];
      uint16_t count=(buffer[24]<<8)+(buffer[25]);
      if (format==0) {
        Serial.print("Format 0: "); Serial.println(count);
        if (file.available()>=count*8) { file.read(buffer,count*8);
          for (uint16_t n=0;n<count;n++) {
            x=((int16_t)((float)ildaSize*(int16_t)((buffer[n*8+0]<<8)+buffer[n*8+1])/100.0f)+32768)>>4;
            y=((int16_t)((float)ildaSize*(int16_t)((buffer[n*8+2]<<8)+buffer[n*8+3])/100.0f)+32768)>>4;
            s=buffer[n*8+6];
            r=palette[buffer[n*8+7]][0];
            g=palette[buffer[n*8+7]][1];
            b=palette[buffer[n*8+7]][2];
            if (bitRead(s,6)) { r=0; g=0; b=0; }
            dacSet(); } } }
      if (format==1) {
        Serial.print("Format 1: "); Serial.println(count);
        if (file.available()>=count*6) { file.read(buffer,count*6);
          for (uint16_t n=0;n<count;n++) {
            x=((int16_t)((float)ildaSize*(int16_t)((buffer[n*6+0]<<8)+buffer[n*6+1])/100.0f)+32768)>>4;
            y=((int16_t)((float)ildaSize*(int16_t)((buffer[n*6+2]<<8)+buffer[n*6+3])/100.0f)+32768)>>4;
            s=buffer[n*6+4];
            r=palette[buffer[n*6+5]][0];
            g=palette[buffer[n*6+5]][1];
            b=palette[buffer[n*6+5]][2];
            if (bitRead(s,6)) { r=0; g=0; b=0; }
            dacSet(); } } }
      if (format==2) {
        Serial.print("Format 2: "); Serial.println(count);
        paletteCount=0;
        if (file.available()>=count*3) { file.read(buffer,count*3);
          for (uint16_t n=0;n<count;n++) {
            palette[paletteCount][0]=buffer[count*3+0];
            palette[paletteCount][1]=buffer[count*3+1];
            palette[paletteCount][2]=buffer[count*3+2];
            paletteCount++; } } }
      if (format==4) {
        Serial.print("Format 4: "); Serial.println(count);
        if (file.available()>=count*10) { file.read(buffer,count*10);
          for (uint16_t n=0;n<count;n++) {
            x=((int16_t)((float)ildaSize*(int16_t)((buffer[n*10+0]<<8)+buffer[n*10+1])/100.0f)+32768)>>4;
            y=((int16_t)((float)ildaSize*(int16_t)((buffer[n*10+2]<<8)+buffer[n*10+3])/100.0f)+32768)>>4;
            s=buffer[n*10+6];
            b=(float)buffer[n*10+7]*(float)blueBright/(float)100;
            g=(float)buffer[n*10+8]*(float)greenBright/(float)100;
            r=(float)buffer[n*10+9]*(float)redBright/(float)100;
            if (bitRead(s,6)) { r=0; g=0; b=0; }
            dacSet(); } } }
      if (format==5) {
        Serial.print("Format 5: "); Serial.println(count);
        if (file.available()>=count*8) { file.read(buffer,count*8);
          for (uint16_t n=0;n<count;n++) {
            x=((int16_t)((float)ildaSize*(int16_t)((buffer[n*8+0]<<8)+buffer[n*8+1])/100.0f)+32768)>>4;
            y=((int16_t)((float)ildaSize*(int16_t)((buffer[n*8+2]<<8)+buffer[n*8+3])/100.0f)+32768)>>4;
            s=buffer[n*8+4];
            b=(float)buffer[n*8+5]*(float)blueBright/(float)100;
            g=(float)buffer[n*8+6]*(float)greenBright/(float)100;
            r=(float)buffer[n*8+7]*(float)redBright/(float)100;
            if (bitRead(s,6)) { r=0; g=0; b=0; }
            dacSet(); } } } } }
  file.close();
  laserOff(); }

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
