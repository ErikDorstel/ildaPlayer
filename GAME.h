struct shipStruct { int16_t posX; int16_t posY; int16_t turnX; int16_t turnY; } ship;
int16_t shipData[][5]={{0,1000,0,255,0},{500,-1000,0,255,0},{-500,-1000,0,255,0},{0,1000,0,255,0}};

void setPixel(int16_t x,int16_t y,uint8_t r,uint8_t g,uint8_t b) {
  ilda[ildaCount].x=((int16_t)((float)ildaSize*(int16_t)(x)/100.0f)+32768)>>4;
  ilda[ildaCount].y=((int16_t)((float)ildaSize*(int16_t)(y)/100.0f)+32768)>>4;
  ilda[ildaCount].r=(float)r*(float)redBright/(float)100;
  ilda[ildaCount].g=(float)g*(float)greenBright/(float)100;
  ilda[ildaCount].b=(float)b*(float)blueBright/(float)100;
  ildaCount++; }

int signum(int x) { return x > 0 ? +100 : x < 0 ? -100 : 0; }

void doLine(int xstart, int ystart, int xend, int yend,uint8_t r,uint8_t g,uint8_t b) {
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
  setPixel(xend, yend,r,g,b); }

void doObject(int16_t object[][5]) {
  for (int n=0;n<3;n++) { doLine(ship.posX+object[n][0],ship.posY+object[n][1],ship.posX+object[n+1][0],ship.posY+object[n+1][1],object[n][2],object[n][3],object[n][4]); } }

void startGame() {
  while (true) {
    ildaCount=0; dacCount=0;
    getADC();
    doObject(shipData);
    if (adc.y>100) { ship.turnY+=1; }
    if (adc.y<-100) { ship.turnY-=1; }
    if (ship.turnY<0) { ship.turnY=0; }
    if (ship.turnY>200) { ship.turnY=200; }
    if (ship.posY+ship.turnY<32767-1000) { ship.posY+=ship.turnY; }
    do { dacWorker(); } while (dacCount>0); } }
