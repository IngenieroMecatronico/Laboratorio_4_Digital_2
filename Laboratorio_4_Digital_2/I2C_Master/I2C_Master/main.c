/*
 * I2C_Master.c
 *
 * Created: 18/2/2025 01:48:21
 * Author : Fer
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>       // Librería estándar para funciones como dtostrf
#include <stdio.h>
#include "I2C/I2C.h"
#include "LCD/LCD.h"      // Incluir la librería de la LCD

#define slave1 0x30
#define slave2 0x40


uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C;
uint8_t valorI2C = 0;

void initPorts(void);
void refreshPORT(uint8_t valor);



int main(void){
    I2C_Master_Init(100000, 1); //inicializar como Master Fscl 100kHz, prescaler 1
	initPorts();
	LCD_CMD(0X01);
	initLCD8bits();  // Inicializar la LCD en modo de 8 bits
	char str[10];             // Buffer para almacenar cadenas de caracteres
	char volt[10];             // Buffer para almacenar cadenas de caracteres

// 	LCD_CMD(0X06);
// 	LCD_CMD(0X01);
	LCD_Set_Cursor(1, 1);  // Posicionar el cursor en la fila 1, columna 1
	LCD_Write_String("S1:     CONT:");  // Mostrar el encabezado en la LCD
    while (1) 
    {	
		
		PORTB |= (1<<PORTB5);
		I2C_Master_Start();
		
		
		//Escritura
		bufferI2C = slave1 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}
		
		I2C_Master_Write('R');
		I2C_Master_Stop();
		_delay_ms(500);
	
		PORTB &= ~(1<<PORTB5);
		I2C_Master_Start();
		
		//Lectura
		bufferI2C = slave1 << 1 | 0b00000001;
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}else{
			TWCR = (1<<TWINT)|(1<<TWEN); //apagamos la bandera de interrupción y le digo que vamos a leer (MASTER READ)
			while(!(TWCR & (1<<TWINT))); //Esperamos al flag de la interrupción
			
			valorI2C = TWDR;
				
			
			I2C_Master_Stop();
			
			
		}
			
			PORTB |= (1<<PORTB5);
			I2C_Master_Start();
		// Convertir el valor del contador a cadena para mostrarlo en la LCD
		sprintf(str, "%02d",valorI2C);
		LCD_Set_Cursor(1,2);
		LCD_Write_String(str);
		//Escritura
		bufferI2C = slave2 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}
		
		I2C_Master_Write('R');
		I2C_Master_Stop();
		_delay_ms(500);
		
		PORTB &= ~(1<<PORTB5);
		I2C_Master_Start();
		
		//Lectura
		bufferI2C = slave2 << 1 | 0b00000001;
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			}else{
			TWCR = (1<<TWINT)|(1<<TWEN); //apagamos la bandera de interrupción y le digo que vamos a leer (MASTER READ)
			while(!(TWCR & (1<<TWINT))); //Esperamos al flag de la interrupción
			
			valorI2C = TWDR;
			
			
			I2C_Master_Stop();
			
		}
			/*sprintf(str, "%02d",valorI2C);*/
			dtostrf((5.0 / 255) * valorI2C, 2, 1, volt);  // Convertir a cadena
			LCD_Set_Cursor(6,2);
			LCD_Write_String(volt);
	}
}

void initPorts(void){
	//Salidas
	DDRB |= (1<<DDB5); //Únicamente para ver en qué estado estamos (LED DEL ARDUINO NANO)
}