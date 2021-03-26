/*************************************************************

 m a i n . c

 Practica 3

 *************************************************************/

// Basic definitions
#include "Base.h"
#include "lcd.h"
#include "accel.h"

//// Llegeix XYZ
//int main(void){
//	char cx[10], cy[10], cz[10], who[10];
//	int32_t x, y, z, who_am_i;
//	baseInit();
//	LCD_Init();
//	LCD_ClearDisplay();
//	LCD_Config(1, 0, 0);
//	initAccel();
//	SLEEP_MS(500);
//	who_am_i = readAccel(0x0F, 1);
//	LCD_SendString(itoa(who_am_i, who, 16));
//	while (1) {
//		x = readAccel(0x29, 1);
//		y = readAccel(0x2b, 1);
//		z = readAccel(0x2d, 1);
//		//Imprimir els valors usant itoa sobre la lectura x, y i z
//		LCD_GotoXY(0, 0);
//		LCD_SendString("X:");
//		LCD_SendString(itoa(x, cx, 10));
//		LCD_SendString("  Y:");
//		LCD_SendString(itoa(y, cy, 10));
//		LCD_SendString("  Z:");
//		LCD_SendString(itoa(z, cz, 10));
//		SLEEP_MS(50);
//		LCD_ClearDisplay();
//	}
//}
//

void displayAccel(int32_t x, int32_t y) {
	int32_t maxX, minX, maxY, minY;
	maxX = 35;
	minX = -35;
	maxY = 35;
	minY = -35;

	// Fer que els màxims d'inclinació siguin +- 45º
	if (x > maxX)
		x = maxX;
	else if (x < minX)
		x = minX;

	if (y > maxY)
		y = maxY;
	else if (y < minY)
		y = minY;

	// Com que per filera tenim 16 columnes, la columna central serà la 8
	// Els asteriscs es podran moure 7 columnes a esquerra i dreta
	int32_t posX = (int32_t) x * 7 / maxX + 7;
	int32_t posY = (int32_t) y * 7 / maxY + 7;
	LCD_GotoXY(posX, 0);
	LCD_SendString("*");
	LCD_GotoXY(posY, 1);
	LCD_SendString("*");
}

int main(void) {
	baseInit();
	LCD_Init();
	initAccel();
	LCD_ClearDisplay();
	LCD_Config(1, 0, 0); // Apaguen cursor i intermitència

	LCD_ClearDisplay();

	int32_t x, y, xini, yini;
	int32_t i;

	// Farem la mitja de 1000 mostres per evitar fluctuacions
	int32_t N = 100;
	// Emmagatzarem les mostres en una matriu, 1a columna per les x i 2a per y
	int32_t mostres[N][2];

	xini = readAccel(0x29, 1);
	yini = readAccel(0x2b, 1);

	// Imatge quan arrenca
	// Com la LCD té 2 files de 16 columnes, el centre sera la col pos 7
	LCD_GotoXY(7, 0);
	LCD_SendString("*");
	LCD_GotoXY(7, 1);
	LCD_SendString("*");

	SLEEP_MS(1000);

	while (1) {
		for (i = 0; i < N; i++) {
			mostres[i][0] = readAccel(0x29, 1);
			mostres[i][1] = readAccel(0x2b, 1);
		}
		// Fem la mitjana i la assignem a les posicions inicials
		for (i = 0; i < N; i++) {
			x += (mostres[i][0]- xini);
			y += (mostres[i][1]-yini);
		}
		x /= N;
		y /= N;

		SLEEP_MS(20);
		LCD_ClearDisplay();
		displayAccel(x, y);

	}
}

