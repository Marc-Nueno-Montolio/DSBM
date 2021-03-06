/*************************************************************

   m a i n . c

   Practica 1

 *************************************************************/

#include "Base.h"     // Basic definitions
#include "lcd.h"
volatile uint32_t i = 0;

// Function that blinks the green LED
void ledBlink(void)
 {
 while(1)
   {
	 i++;
   // Turn on the green LED using the BSRR register
   (LEDS_PORT->BSRR.H.set)=BIT(GREEN_LED_PAD);

   // Wait 200ms
   SLEEP_MS(200);

   // Turn off the green LED using the BSRR register
   (LEDS_PORT->BSRR.H.clear)=BIT(GREEN_LED_PAD);

   // Wait 200ms
   SLEEP_MS(200);
   }
 }

void ledSequence(void){
	int leds[4] = {GREEN_LED_PAD, ORANGE_LED_PAD, RED_LED_PAD, BLUE_LED_PAD};

	while(1){
		i++;
		int j;
		for(j = 0; j < 4; j++){
			// Turn on the LED using the BSRR register
			(LEDS_PORT->BSRR.H.set)=BIT(leds[j]);

			// Wait 0.5s
			SLEEP_MS(500);

			// Turn off the LED using the BSRR register
			(LEDS_PORT->BSRR.H.clear)=BIT(leds[j]);

		}

	}
}

int main(void)
{
	baseInit(); // Basic initialization
	LCD_Init(); // Program the GPIO I/O lines
	LCD_Config(1,1,0);
	SLEEP_MS(2000);
	LCD_Config(1,0,1);
	SLEEP_MS(2000);
	LCD_ClearDisplay();
	LCD_SendString("Ainna");
	SLEEP_MS(2000);
	LCD_GotoXY(0,1);
	LCD_SendString("Marc");
	LCD_Config(1,0,0);
	SLEEP_MS(2000);
	LCD_Backlight(0);
	SLEEP_MS(2000);
	LCD_Config(0,0,0);
	while (1); // Infinite loop so we don't exit main( )
}




