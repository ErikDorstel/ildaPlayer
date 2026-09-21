#define pinSoundA      47
#define pinSoundB      48
#define channelSoundA   4
#define channelSoundB   6
#define audioDepth     13
#define audioMiddle  4095

uint16_t note[128];
uint8_t entertainer[]={50,255,0,86,1,88,1,84,1,81,3,83,1,79,2,74,1,76,1,72,1,69,3,71,1,67,2,74,1,76,1,72,1,69,3,71,1,69,1,70,1,67,2,67,2,74,1,75,1,255,20};

void initSound() {
  ledcAttachPin(pinSoundA,channelSoundA); ledcWrite(channelSoundA,0);
  ledcAttachPin(pinSoundB,channelSoundB); ledcWrite(channelSoundB,0);
  for (uint8_t ton=0;ton<128;ton++) { note[ton]=440.0f*pow(2,(float)(ton-69)/12.0f); } }

void soundWorker() {
  static uint32_t soundTimer;
  static uint8_t pointer=1;
  if (millis()>=soundTimer) {
    if (entertainer[pointer]==255) { if (entertainer[pointer+1]==0) { ledcWrite(channelSoundA,audioMiddle); ledcWrite(channelSoundB,audioMiddle); }
      else { ledcWrite(channelSoundA,0); ledcWrite(channelSoundB,0); } }
    else { ledcSetup(channelSoundA,note[entertainer[pointer]],audioDepth);
           ledcSetup(channelSoundB,note[entertainer[pointer]-24],audioDepth); }
    soundTimer=millis()+(250*entertainer[pointer+1]);
    pointer+=2; if (pointer>entertainer[0]) { pointer=1; } } }
