#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdlib.h>

// Funci�n para inicializar el ADC
void ADC_init(void);

// Funci�n para leer un canal ADC (0-5V mapeado a 0-1023)
uint16_t ADC_read(uint8_t canal);

#endif
