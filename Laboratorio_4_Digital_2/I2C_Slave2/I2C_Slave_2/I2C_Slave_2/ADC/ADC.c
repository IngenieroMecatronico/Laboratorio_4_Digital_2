#include "ADC.h"
#include <util/delay.h>

void ADC_init(void) {
	DDRC &= ~(1 << PORTC0); // Configurar PC1 como entrada
	ADMUX = (1 << REFS0); // Voltaje de referencia AVcc (5V)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC y preescaler de 128 (125kHz)
}

uint16_t ADC_read(uint8_t canal) {
	canal &= 0b00001111;
	ADMUX = (ADMUX & 0xF0) | canal; // Seleccionar el canal
	ADCSRA |= (1 << ADSC); // Iniciar la conversión
	while (ADCSRA & (1 << ADSC)); // Esperar a que la conversión termine
	_delay_ms(100);
	return (ADC<<2);// Leer el resultado
}
