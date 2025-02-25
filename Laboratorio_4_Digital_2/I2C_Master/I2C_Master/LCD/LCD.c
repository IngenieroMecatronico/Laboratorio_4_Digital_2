/*
 * LCD.c
 *
 * Created: 3/2/2025 23:48:44
 *  Author: Fer
 */ 
#include "LCD.h"
//Funcion para inicializar LCD en modo de 8 bits
void initLCD8bits(void){
	DDRD |= 0xFF;
	PORTD = 0;
	DDRC |= 0xFF;
	PORTC = 0;
	DDRB |= 0xFF;
	PORTB = 0;


//POWER ON
LCD_Port(0X00);
_delay_ms(20);
//Function set
LCD_CMD(0x38);
_delay_us(1);
//Display ON/OFF
LCD_CMD(0b00001100);
_delay_us(1);
//Entry mode set
LCD_CMD(0x06);
_delay_us(1);
//Clear display
LCD_CMD(0x01);
_delay_ms(5);

UCSR0B = 0;

}

//Funcion para enviar un comando
void LCD_CMD(char a){
	//RS = 0; // => RS = 0 //Dato en el puerto lo va a interpretar como comando
	PORTB &= ~(1<<PORTB0);
	LCD_Port(a);
	//EN  = 1; // => E = 1
	PORTB |= (1<<PORTB5);
	_delay_ms(4);
	//EN = 0; // => E = 0
	PORTB &= ~(1<<PORTB5);
	_delay_ms(50);
}

//Funcion para colocar en el puerto un valor
void LCD_Port(char a){
	if (a & 1)
	//D0 = 1
	PORTC |= (1<<PORTC0);
	else
	//D0 = 0
	PORTC &= ~(1<<PORTC0);
	if (a & 2)
	//D1 = 1
	PORTC |= (1<<PORTC1);
	else
	//D1 = 0
	PORTC &= ~(1<<PORTC1);
	if (a & 4)
	//D2 = 1
	PORTD |= (1<<PORTD2);
	else
	//D2 = 0
	PORTD &= ~(1<<PORTD2);
	if (a & 8)
	//D3 = 1
	PORTD |= (1<<PORTD3);
	else
	//D3 = 0
	PORTD &= ~(1<<PORTD3);
	if (a & 16)
	//D4 = 1
	PORTD |= (1<<PORTD4);
	else
	//D4 = 0
	PORTD &= ~(1<<PORTD4);
	if (a & 32)
	//D5 = 1
	PORTD |= (1<<PORTD5);
	else
	//D5 = 0
	PORTD &= ~(1<<PORTD5);
	if (a & 64)
	//D6 = 1
	PORTD |= (1<<PORTD6);
	else
	//D6 = 0
	PORTD &= ~(1<<PORTD6);
	if (a & 128)
	//D7 = 1
	PORTD |= (1<<PORTD7);
	else
	//D7 = 0
	PORTD &= ~(1<<PORTD7);
	
}

//Funcion para colocar en el puerto un valor
void LCD_Write_Char(char c){
	//RS = 1; // => RS = 1 //Dato en el puerto lo va a interpretar como CARACTER (DATO)
	PORTB |= (1<<PORTB0);
	LCD_Port(c);
	//E = 1; // => E = 1
	PORTB |= (1<<PORTB5);
	_delay_ms(4);
	//E = 0; // => E = 0
	PORTB &= ~ (1<<PORTB5);
	_delay_ms(50);
	
}

//Funcion para enviar una cadena
void LCD_Write_String(char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++)
	LCD_Write_Char(a[i]);
}

//Desplazamiento hacia la derecha
void LCD_Shift_Right(void){
	LCD_CMD(0x1C);
}

//Desplazamiento hacia la izquierda
void LCD_Shift_Left(void){
	LCD_CMD(0x18);
}

//Establecer el cursor
void LCD_Set_Cursor(char c, char f){
	char temp;
	if (f==1){
		temp = 0x80 + c - 1;
		LCD_CMD(temp);
	} else if (f == 2){
		temp = 0xC0 + c - 1;
		LCD_CMD(temp);
	}
}
