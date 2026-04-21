struct shipStruct { float posX; float posY; float moveX; float moveY; float throttle; float turn; } ship;
int16_t shipData[][5]={{5,0,0,0,0},{0,1000,0,0,0},{500,-1000,0,255,0},{-500,-1000,0,255,0},{0,1000,0,255,0}};
int16_t shipEngineData[][5]={{5,0,0,0,0},{0,1000,0,0,0},{500,-1000,0,255,0},{-500,-1000,255,0,0},{0,1000,0,255,0}};
struct phaserStruct { float posX; float posY; float moveX; float moveY; float throttle; float turn; } phaser[20];
int16_t phaserData[][5]={{3,0,0,0,0},{0,1000,0,0,0},{0,2000,255,0,0}};

void setPixel(int16_t x,int16_t y,uint8_t r,uint8_t g,uint8_t b) {
  ilda[ildaCount].x=((int16_t)((float)ildaSize*(int16_t)(x)/100.0f)+32768)>>4;
  ilda[ildaCount].y=((int16_t)((float)ildaSize*(int16_t)(y)/100.0f)+32768)>>4;
  ilda[ildaCount].r=(float)r*(float)redBright/(float)100;
  ilda[ildaCount].g=(float)g*(float)greenBright/(float)100;
  ilda[ildaCount].b=(float)b*(float)blueBright/(float)100;
  ildaCount++; }

int signum(int x) { return x > 0 ? +100 : x < 0 ? -100 : 0; }

void doLine(int xend, int yend,uint8_t r,uint8_t g,uint8_t b) {
  static int xstart,ystart;
  int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, deltaslowdirection, deltafastdirection, err;
  dx = xend - xstart;
  dy = yend - ystart;
  incx = signum(dx);
  incy = signum(dy);
  if (dx < 0) dx = -dx;
  if (dy < 0) dy = -dy;
  if (dx > dy) {
    pdx = incx; pdy = 0;
    ddx = incx; ddy = incy;
    deltaslowdirection = dy; deltafastdirection = dx; }
  else {
    pdx = 0;    pdy = incy;
    ddx = incx; ddy = incy;
    deltaslowdirection = dx; deltafastdirection = dy; }
  x = xstart;
  y = ystart;
  err = deltafastdirection / 2;
  setPixel(x, y,r,g,b);
  for (t = 0; t < deltafastdirection; t+=100) {
    err -= deltaslowdirection;
    if (err < 0) {
      err += deltafastdirection;
      x += ddx;
      y += ddy; }
    else {
      x += pdx;
      y += pdy; }
    setPixel(x, y,r,g,b); }
  setPixel(xend, yend,r,g,b);
  xstart=xend; ystart=yend; }

void doObject(int16_t object[][5],float posX,float posY,float turn) {
  for (int n=1;n<object[0][0];n++) {
    int endX=cos(turn)*object[n][0]-sin(turn)*object[n][1];
    int endY=sin(turn)*object[n][0]+cos(turn)*object[n][1];
    doLine(posX+endX,posY+endY,object[n][2],object[n][3],object[n][4]); } }

void startGame() {
  static uint32_t triggerTimer;
  while (true) {
    ildaCount=0; dacCount=0;
    getADC();

    if (adc.x>100) { ship.turn-=0.01; }
    if (adc.x<-100) { ship.turn+=0.01; }
    if (adc.y>100) { ship.throttle+=0.1; }
    else if (adc.y<-100) { ship.throttle-=0.1; }
    else { ship.throttle-=0.05; }
    if (ship.throttle<0) { ship.throttle=0; }
    if (ship.throttle>100) { ship.throttle=100; }
    float angle=ship.turn+0.785398;
    ship.moveX=cos(angle)*ship.throttle-sin(angle)*ship.throttle;
    ship.moveY=sin(angle)*ship.throttle+cos(angle)*ship.throttle;
    ship.posX+=ship.moveX;
    ship.posY+=ship.moveY;
    if (ship.posX>32767-1100) { ship.posX=32767-1100; }
    else if (ship.posX<-32768+1100) { ship.posX=-32768+1100; }
    if (ship.posY>32767-1100) { ship.posY=32767-1100; }
    else if (ship.posY<-32768+1100) { ship.posY=-32768+1100; }
    if (adc.y>100) { doObject(shipEngineData,ship.posX,ship.posY,ship.turn); }
    else { doObject(shipData,ship.posX,ship.posY,ship.turn); }

    if (adc.t && millis()>triggerTimer) { triggerTimer=millis()+1000;
      for (int n=0;n<20;n++) { if (phaser[n].throttle==0) {
        phaser[n].posX=ship.posX; phaser[n].posY=ship.posY;
        phaser[n].moveX=cos(angle)*200-sin(angle)*200;
        phaser[n].moveY=sin(angle)*200+cos(angle)*200;
        phaser[n].turn=ship.turn; phaser[n].throttle=1; break; } } }
    for (int n=0;n<20;n++) { if (phaser[n].throttle) {
      doObject(phaserData,phaser[n].posX,phaser[n].posY,phaser[n].turn);
      phaser[n].posX+=phaser[n].moveX; phaser[n].posY+=phaser[n].moveY;
      if (phaser[n].posX>32767-2100) { phaser[n].throttle=0; }
      else if (phaser[n].posX<-32768+2100) { phaser[n].throttle=0; }
      if (phaser[n].posY>32767-2100) { phaser[n].throttle=0; }
      else if (phaser[n].posY<-32768+2100) { phaser[n].throttle=0; } } }

    do { dacWorker(); } while (dacCount>0); } }
