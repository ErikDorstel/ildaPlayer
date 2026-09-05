struct shipStruct1 { float posX; float posY; float moveX; float moveY; float throttle; float turn; } ship1;
struct shipStruct2 { float posX; float posY; float moveX; float moveY; float throttle; float turn; } ship2;

void initDogfight() { }

void doDogfight() {
  static uint32_t triggerTimer;
  ildaCount=0;
  getADC1(); getADC2();

  doLine(-5000,20000,0,0,0);
  doLine(-5000,5000,0,0,255);
  doLine(-20000,5000,0,0,255);
  doLine(-20000,-5000,0,0,0);
  doLine(-5000,-5000,0,0,255);
  doLine(-5000,-20000,0,0,255),
  doLine(5000,-20000,0,0,0);
  doLine(5000,-5000,0,0,255);
  doLine(20000,-5000,0,0,255);
  doLine(20000,5000,0,0,0);
  doLine(5000,5000,0,0,255);
  doLine(5000,20000,0,0,255);

  if (adc1.x>500) { ship1.turn-=0.1; }
  if (adc1.x<-500) { ship1.turn+=0.1; }
  if (adc1.y>500) { ship1.throttle+=3; }
  else if (adc1.y<-500) { ship1.throttle-=3; }
  else { ship1.throttle-=0.5; }
  if (ship1.throttle<0) { ship1.throttle=0; }
  if (ship1.throttle>100) { ship1.throttle=100; }
  float angle1=ship1.turn+0.785398;
  ship1.moveX=cos(angle1)*ship1.throttle-sin(angle1)*ship1.throttle;
  ship1.moveY=sin(angle1)*ship1.throttle+cos(angle1)*ship1.throttle;
  ship1.posX+=ship1.moveX;
  ship1.posY+=ship1.moveY;
  if (ship1.posX>32767-1100) { ship1.posX=32767-1100; }
  else if (ship1.posX<-32768+1100) { ship1.posX=-32768+1100; }
  if (ship1.posY>32767-1100) { ship1.posY=32767-1100; }
  else if (ship1.posY<-32768+1100) { ship1.posY=-32768+1100; }
  if (adc1.y>100) { doObject(shipEngineData,ship1.posX,ship1.posY,ship1.turn); }
  else { doObject(shipData,ship1.posX,ship1.posY,ship1.turn); }

  if (adc2.x>500) { ship2.turn-=0.1; }
  if (adc2.x<-500) { ship2.turn+=0.1; }
  if (adc2.y>500) { ship2.throttle+=3; }
  else if (adc2.y<-500) { ship2.throttle-=3; }
  else { ship2.throttle-=0.5; }
  if (ship2.throttle<0) { ship2.throttle=0; }
  if (ship2.throttle>100) { ship2.throttle=100; }
  float angle2=ship2.turn+0.785398;
  ship2.moveX=cos(angle2)*ship2.throttle-sin(angle2)*ship2.throttle;
  ship2.moveY=sin(angle2)*ship2.throttle+cos(angle2)*ship2.throttle;
  ship2.posX+=ship2.moveX;
  ship2.posY+=ship2.moveY;
  if (ship2.posX>32767-1100) { ship2.posX=32767-1100; }
  else if (ship2.posX<-32768+1100) { ship2.posX=-32768+1100; }
  if (ship2.posY>32767-1100) { ship2.posY=32767-1100; }
  else if (ship2.posY<-32768+1100) { ship2.posY=-32768+1100; }
  if (adc2.y>100) { doObject(shipEngineData,ship2.posX,ship2.posY,ship2.turn); }
  else { doObject(shipData,ship2.posX,ship2.posY,ship2.turn); }

  static uint32_t dacTimer;
  static uint8_t oldR=0,oldG=0,oldB=0;
  for (int n=0;n<ildaCount;n++) {
    while (micros()<=dacTimer) { } dacTimer=micros()+dacSpeed;
    if (ilda[n].r!=oldR) { oldR=ilda[n].r; ledcWrite(channelRed,oldR); }
    if (ilda[n].g!=oldG) { oldG=ilda[n].g; ledcWrite(channelGreen,oldG); }
    if (ilda[n].b!=oldB) { oldB=ilda[n].b; ledcWrite(channelBlue,oldB); }
    digitalWrite(csPin,LOW);
    hspi->write16(ilda[n].x | 0b0011000000000000);
    digitalWrite(csPin,HIGH);
    digitalWrite(csPin,LOW);
    hspi->write16(ilda[n].y | 0b1011000000000000);
    digitalWrite(csPin,HIGH);
    digitalWrite(ldacPin,LOW); digitalWrite(ldacPin,HIGH); } }
