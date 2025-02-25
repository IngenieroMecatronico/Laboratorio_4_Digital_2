/*
 * Lab3_Slave.c
 *
 * Created: 11/02/2025 16:46:22
 * Author : Juanfer Maldonado - 22449.
 * Descripción: 
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "I2C/I2C.h"
uint8_t buffer = 0;
#define SlaveAddress 0x30

//***********
// Prototipos de función
//***********
volatile uint8_t contador;			//Inicializo mi contador.
volatile uint8_t aumenta;           // Variable para ver si se oprimió el botón de aumentar el contador.
volatile uint8_t disminuye;         // Variable para ver si se oprimió el botón de disminuir el contador.
volatile uint8_t salida;


//***********
// Variables globales
//***********
//void puertos(uint8_t puertod, uint8_t puertob, uint8_t apagapuerto);
//0x3C por que allí estan los leds, de D2 a D5.
//0xFC ya que los botones están en PB1 y PB2
//0 para apagar los botones.
int main(void)
{
	DDRB = 0xFF;
	PORTB = 0;
	DDRC &= ~(1<<PORTC0);
	DDRC &= ~(1<<PORTC1);
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT8)|(1<<PCINT9);
	I2C_Slave_Init(SlaveAddress);
	//PORTC = 0;
	//puertos(0x3C, 0xFC, 0);
	sei();
    while (1) 
    {
		//PORTB = (contador);
		//salida = (contador << 2);
		//PORTD = salida;
		
		if (aumenta == 1)
		{
			if (contador < 15)
			{
				contador++;
				aumenta = 0;
			}
			else if (contador == 15)
			{
				contador = 15;
				aumenta = 0;
			}
		}
		if (disminuye == 1)
		{
			if (contador > 0)
			{
				contador--;
				disminuye = 0;
			}
			else if (contador == 0)
			{
				contador = 0;
				disminuye = 0;
			}
			
			//contador--;
		}
		PORTB = (contador);
	
	}
	//PORTD = (contador);
}



//***********
// Subrutinas
//***********


//*****************
// ISR de UART
//*****************
ISR(PCINT1_vect){
	if (!(PINC & (1<<PORTC0)))   //Si se detecta un cambio en PINB y fué en el puerto PB0, aumenta el contador de los leds.
	{
		
		//contador++;
		aumenta = 1;
	}
	if (!(PINC & (1<<PORTC1))) //Si se detecta cambio en PINB y fué Puerto PB1, disminuye el contador.
	{
		disminuye = 1;
		//contador--;
	}
}

// ISR(TWI_vect){
// 	uint8_t estado;
// 	estado = TWSR & 0xFC;
// 	switch(estado){
// 		case 0x60:
// 		case 0x70:
// 		TWCR |= (1<<TWINT);
// 		break;
// 		case 0x80:
// 		case 0x90:
// 		buffer = TWDR; //Limpia la bandera
// 		break;
// 		case 0xA8:
// 		case 0xB8:
// 		TWDR = contador; //Cargar el dato
// 		TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA); //Inicia el envío
// 		break;
// 		default: // Se libera el bus de cualquier error
// 		TWCR |= (1<<TWINT)|(1<<TWSTO);
// 		break;
// 	}
// }

ISR(TWI_vect){
	uint8_t estado = TWSR & 0xF8; // Máscara correcta para el estado
	switch(estado){
		case 0x60: // Dirección recibida, listo para recibir
		case 0x70: // Dirección recibida en modo general call
		TWCR |= (1<<TWINT)|(1<<TWEA);
		break;
		case 0x80: // Datos recibidos y ACK enviado
		case 0x90:
		buffer = TWDR; // Guardar dato recibido
		TWCR |= (1<<TWINT)|(1<<TWEA); // Responder con ACK
		break;
		case 0xA8: // Dirección con bit de lectura recibida
		case 0xB8: // Maestro está esperando datos
		TWDR = contador; // Enviar el dato
		TWCR |= (1<<TWINT)|(1<<TWEA); // Continuar transmisión
		break;
		case 0xC0: // Maestro envió NACK, fin de la transmisión
		TWCR |= (1<<TWINT)|(1<<TWEA);
		break;
		default: // Liberar el bus en caso de error
		TWCR |= (1<<TWINT)|(1<<TWSTO);
		break;
	}
}
