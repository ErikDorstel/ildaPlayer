#include "esp_adc_cal.h"

struct adcStruct { int16_t x; int16_t y; int16_t z; bool t;
                   int16_t xCal=1600; int16_t yCal=1600; int16_t zCal=1600; } adc;

void initADC() {
  pinMode(9,INPUT_PULLUP);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_12);
  adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_12);
  adc1_config_width(ADC_WIDTH_BIT_12);
  for (int n=0;n<100;n++) {
    adc.xCal=(adc.xCal+adc1_get_raw(ADC1_CHANNEL_0))/2;
    adc.yCal=(adc.yCal+adc1_get_raw(ADC1_CHANNEL_1))/2;
    adc.zCal=(adc.zCal+adc1_get_raw(ADC1_CHANNEL_7))/2; } }

void getADC() {
  adc.x=(adc.x+adc1_get_raw(ADC1_CHANNEL_0)-adc.xCal)/2;
  adc.y=(adc.y+adc1_get_raw(ADC1_CHANNEL_1)-adc.yCal)/2;
  adc.z=(adc.z+adc1_get_raw(ADC1_CHANNEL_7)-adc.zCal)/2;
  adc.t=!digitalRead(9);
  Serial.println(adc.x);
  Serial.println(adc.y);
  Serial.println(adc.z);
  Serial.println(adc.t); }
