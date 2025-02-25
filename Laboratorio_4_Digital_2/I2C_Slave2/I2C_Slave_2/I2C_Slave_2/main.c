#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"

#define SlaveAddress 0x40

volatile uint8_t contador = 0;
uint8_t buffer = 0;
volatile uint8_t valoradc = 0; 
//***************************
// Prototipos de funcion
//***************************
void Ports_init(void);

//***************************
// Codigo principal
//***************************
int main(void)
{
	sei();
	ADC_init();
	DDRB |= (1<<DDB5);
	DDRB &= ~(1<<DDB5);
	I2C_Slave_Init(SlaveAddress);
	
    while (1) 
    {
		valoradc = ADC_read(0);
		if(buffer == 'R'){
			PORTB |= (1<<PINB5);  // Activar el pin PB5 (LED, relay, etc.)
			buffer = 0;         // Reiniciar el buffer
		}
    }
}

//***************************
// ISR
//***************************

ISR(TWI_vect){
	uint8_t estado;
	estado = TWSR & 0xF8;
	switch(estado){
		case 0x60:
		case 0x70:
		TWCR |= (1<< TWINT);
		break;
		case 0x80:
		case 0x90:
		buffer = TWDR;
		TWCR |= (1<< TWINT); // Se limpia la bandera
		break;
		case 0xA8:
		case 0xB8:
		TWDR = valoradc; // Cargar el dato
		TWCR = (1 << TWEN)|(1 << TWIE)|(1 << TWINT)|(1 << TWEA); // Inicia el envío
		break;
		default: // Se libera el bus de cualquier error
		TWCR |= (1 << TWINT)|(1 << TWSTO);
		break;
	}
}