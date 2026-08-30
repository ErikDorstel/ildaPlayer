#include "esp_adc_cal.h"

struct adcStruct { int16_t x; int16_t y; int16_t z; bool t;
                   int16_t xCal=1600; int16_t yCal=1600; int16_t zCal=1600; };
struct adcStruct adc1,adc2;

void initADC() {
  pinMode(5,INPUT_PULLUP); pinMode(9,INPUT_PULLUP);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_12);
  adc1_config_width(ADC_WIDTH_BIT_12);
  for (int n=0;n<100;n++) {
    adc1.xCal=(adc1.xCal+adc1_get_raw(ADC1_CHANNEL_0))/2;
    adc1.yCal=(adc1.yCal+adc1_get_raw(ADC1_CHANNEL_1))/2;
    adc1.zCal=(adc1.zCal+adc1_get_raw(ADC1_CHANNEL_3))/2;
    adc2.xCal=(adc2.xCal+adc1_get_raw(ADC1_CHANNEL_5))/2;
    adc2.yCal=(adc2.yCal+adc1_get_raw(ADC1_CHANNEL_6))/2;
    adc2.zCal=(adc2.zCal+adc1_get_raw(ADC1_CHANNEL_7))/2;} }

void getADC1() {
  adc1.x=(adc1.x+adc1_get_raw(ADC1_CHANNEL_0)-adc1.xCal)/2;
  adc1.y=(adc1.y+adc1_get_raw(ADC1_CHANNEL_1)-adc1.yCal)/2;
  adc1.z=(adc1.z+adc1_get_raw(ADC1_CHANNEL_3)-adc1.zCal)/2;
  adc1.t=!digitalRead(5); }

void getADC2() {
  adc2.x=(adc2.x+adc1_get_raw(ADC1_CHANNEL_5)-adc2.xCal)/2;
  adc2.y=(adc2.y+adc1_get_raw(ADC1_CHANNEL_6)-adc2.yCal)/2;
  adc2.z=(adc2.z+adc1_get_raw(ADC1_CHANNEL_7)-adc2.zCal)/2;
  adc2.t=!digitalRead(9); }
