#include <SPI.h>

#define ledPinRed   38
#define ledPinGreen 39
#define ledPinBlue  40
#define shutterPin  41
#define channelRed   0
#define channelGreen 1
#define channelBlue  2
#define freq    100000
#define depth        8
#define csPin       10
#define sckPin      16
#define mosiPin     17
#define misoPin     18
#define ldacPin     14
#define lockPin     21

SPIClass *hspi = new SPIClass(HSPI);

struct ildaStruct { uint16_t x; uint16_t y; uint8_t r; uint8_t g; uint8_t b; uint8_t s; };
ildaStruct *ilda=(ildaStruct*)heap_caps_malloc(sizeof(ildaStruct)*1000000,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
uint32_t ildaCount,paletteCount,dacAvailable;
uint16_t dacSpeed=50;
uint8_t redBright=50,greenBright=50,blueBright=50,ildaSize=100;
uint16_t x,y;
uint8_t s,r,g,b;

void initDAC() {
  pinMode(csPin,OUTPUT); digitalWrite(csPin,HIGH);
  pinMode(ldacPin,OUTPUT); digitalWrite(ldacPin,HIGH);
  pinMode(lockPin,OUTPUT); digitalWrite(lockPin,LOW);
  pinMode(shutterPin,OUTPUT); digitalWrite(shutterPin,LOW);

  hspi->begin(sckPin,misoPin,mosiPin,csPin);
  hspi->beginTransaction(SPISettings(20000000,MSBFIRST,SPI_MODE0));

  ledcSetup(channelRed,freq,depth); ledcAttachPin(ledPinRed,channelRed); ledcWrite(channelRed,0);
  ledcSetup(channelGreen,freq,depth); ledcAttachPin(ledPinGreen,channelGreen); ledcWrite(channelGreen,0);
  ledcSetup(channelBlue,freq,depth); ledcAttachPin(ledPinBlue,channelBlue); ledcWrite(channelBlue,0); }

void colorOff() { ledcWrite(channelRed,0); ledcWrite(channelGreen,0); ledcWrite(channelBlue,0); }
void laserOn() { digitalWrite(lockPin,HIGH); colorOff(); digitalWrite(shutterPin,HIGH); }
void laserOff() { digitalWrite(lockPin,LOW); colorOff(); digitalWrite(shutterPin,LOW); }

void dacSet() {
  static uint32_t dacTimer;
  static uint8_t oldR=0,oldG=0,oldB=0;
  while (micros()<=dacTimer) { } dacTimer=micros()+dacSpeed;
  if (r!=oldR) { oldR=r; ledcWrite(channelRed,oldR); }
  if (g!=oldG) { oldG=g; ledcWrite(channelGreen,oldG); }
  if (b!=oldB) { oldB=b; ledcWrite(channelBlue,oldB); }
  digitalWrite(csPin,LOW);
  hspi->write16(x | 0b0011000000000000);
  digitalWrite(csPin,HIGH);
  digitalWrite(csPin,LOW);
  hspi->write16(y | 0b1011000000000000);
  digitalWrite(csPin,HIGH);
  digitalWrite(ldacPin,LOW); digitalWrite(ldacPin,HIGH); }
