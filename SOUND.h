#define pinSoundA      47
#define pinSoundB      48
#define channelSoundA   4
#define channelSoundB   6
#define audioDepth     13
#define audioMiddle  4095

uint16_t note[128];
uint8_t elise[]={38,255,0,76,2,75,2,76,2,75,2,76,2,71,2,74,2,72,2,69,4,60,2,64,2,69,2,71,4,64,2,68,2,71,2,72,4,255,20};

void initSound() {
  ledcAttachPin(pinSoundA,channelSoundA); ledcWrite(channelSoundA,0);
  ledcAttachPin(pinSoundB,channelSoundB); ledcWrite(channelSoundB,0);
  for (uint8_t ton=0;ton<128;ton++) { note[ton]=440.0f*pow(2,(float)(ton-69)/12.0f); } }

void soundWorker() {
  static uint32_t soundTimer;
  static uint8_t pointer=1;
  if (millis()>=soundTimer) {
    if (elise[pointer]==255) { if (elise[pointer+1]==0) { ledcWrite(channelSoundA,audioMiddle); ledcWrite(channelSoundB,audioMiddle); }
      else { ledcWrite(channelSoundA,0); ledcWrite(channelSoundB,0); } }
    else { ledcSetup(channelSoundA,note[elise[pointer]],audioDepth);
           ledcSetup(channelSoundB,note[elise[pointer]-24],audioDepth); }
    soundTimer=millis()+(100*elise[pointer+1]);
    pointer+=2; if (pointer>elise[0]) { pointer=1; } } }
