#ifndef ADC_H
#define ADC_H
int ADC_read(uint8_t channel, uint8_t resolution, bool single_ended);
void ADC_print(uint16_t total);
#endif