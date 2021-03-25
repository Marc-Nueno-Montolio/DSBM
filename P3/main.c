/*************************************************************

 m a i n . c

 Practica 3

 *************************************************************/

// Basic definitions
#include "Base.h"     
#include "lcd.h"
#include "accel.h"

// Llegir
/*int main(void){
	char cx[10], cy[10], cz[10];
	int32_t x, y, z;
	baseInit();
	LCD_Init();
	LCD_ClearDisplay();
	LCD_Config(1, 0, 0);
	initAccel();
	SLEEP_MS(500);
	who_am_i = readAccel(0x0F, 1);
	LCD_SendString(itoa(who_am_i, who, 16));
	while (1) {
		x = readAccel(0x29, 1);
		y = readAccel(0x2b, 1);
		z = readAccel(0x2d, 1);
		//Imprimir els valors usant itoa sobre la lectura x, y i z
		LCD_GotoXY(0, 0);
		LCD_SendString("X:");
		LCD_SendString(itoa(x, cx, 10));
		LCD_SendString("  Y:");
		LCD_SendString(itoa(y, cy, 10));
		LCD_SendString("  Z:");
		LCD_SendString(itoa(z, cz, 10));
		SLEEP_MS(50);
		LCD_ClearDisplay();
	}
}*/

void displayAccel(int32_t x, int32_t y) {
	int32_t maxX, minX, maxY, minY;
	maxX = 35;
	minX = -35;
	maxY = 20;
	minY = 20;

	// Fer que els màxims d'inclinació siguin +- 45º
	if (x > maxX)
		x = maxX;
	else if (x < minX)
		posDegreesX = minX;
	if (y > maxY)
		y = maxY;
	else if (y < minY)
		y = minY;
	// Com que per filera tenim 16 columnes, la columna central serà la 8
	// Els asteriscs es podran moure 7 columnes a esquerra i dreta
	posX = round(x*7/maxX + 7);
	posY = round(y*7/maxY + 7);
	LCD_GotoXY(posX, 0);
	LCD_SendString("*");
	LCD_GotoXY(posY, 1);
	LCD_SendString("*");
}

int main(void) {
	int32_t x, y;
	baseInit();
	LCD_Init();
	LCD_ClearDisplay();
	LCD_Config(1, 0, 0); // Apaguen cursor i intermitència
	initAccel();
	//Llegim x i y i les assignem com les posicions inicials
	int32_t xini = readAccel(0x29, 1);
	int32_t yini = readAccel(0x2b, 1);
	SLEEP_MS(200);
	// Imatge quan arrenca
	//Com la LCD té 2 files de 16 columnes, el centre sera la col pos 7
	LCD_GotoXY(7, 0);
	LCD_SendChar("*");
	LCD_GotoXY(7, 1);
	LCD_SendChar("*");

	while (1) {
		x = readAccel(0x29, 1);
		x -= xini;
		y = readAccel(0x2b, 1);
		y -= yini;
		displayAccel(x, y);
SLEEP_MS(100);
LCD_ClearDisplay();
	}
}
