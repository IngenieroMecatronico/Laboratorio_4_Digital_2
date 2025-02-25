#ifndef LCD_H_
#define LCD_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define E (1<<PORTB1)

//Funcion para inicializar LCD en modo 8 bits
void initLCD8bits(void);
//Funcion para colocar en el puerto un valor
void LCD_Port(char a);
//Funcion para enviar un comando
void LCD_CMD(char a);
//Funcion para enviar un caracter
void LCD_Write_Char(char c);
//Funcion para enviar una cadena
void LCD_Write_String(char *a);
//Desplazamiento a la derecha
void LCD_Shift_Right(void);
//Desplazamiento a la izquierda
void LCD_Shift_Left(void);
//Establecer el cursor
void LCD_Set_Cursor(char c, char f);

#endif /* LCD_H_ */