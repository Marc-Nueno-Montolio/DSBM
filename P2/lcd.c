/****************************************************

 l c d . c

 P2 LCD Functions

 *****************************************************/

#include "Base.h"  // Basic definitions
#include "lcd.h"   // LCD definitions
// Some of the function need to be completed
// as is requested on the manual

/***************** PRIVATE FUNCTIONS TO BE IMPLEMENTED *************
 In C, all functions are public by default.
 Those functions, as they are not needed outside of lcd.c
 won't have prototypes on lcd.h.
 They will also be declared static so that the compiler can
 optimize their usage knowing they are not used outside this file.
 ****************************************************************/

// Initializes DB7..DB4, RS, E i BL in Push-Pull output mode
// and sets the ouptuts to "0"
void GPIO_ModePushPull(GPIO_TypeDef *port, int32_t linia){
	// Fixem el registre MODEr amb sortida "01"
	port->MODER = (port->MODER)| (BIT(2*linia));
	port->MODER = (port->MODER)& (~BIT(2*linia+1));

	// Fixem el registre OTYPER amb sortida Push-Pull, '0'
	port->OTYPER = (port->OTYPER)&(~BIT(linia));
// Fixem el registre OSPEEDR (2 MHz per evitar soroll)
	// LCD dispositiu lent --> amb 2Mhz ja n'hi ha prou i menys soroll.
	// 2MHz --> configurar el registre amb "00
	port->OSPEEDR = (port->OSPEEDR)&(~BIT(2*linia))&(~BIT(2*linia+1));

	// Fixem el registre PUPDR sense PP ni PD, el deixem en "00"
	port->PUPDR = (port->PUPDR)&(~BIT(2*linia))&(~BIT(2*linia+1));
}
static void lcdGPIOInit(void){
	uint8_t linia[] = {LCD_BL_PAD, LCD_E_PAD, LCD_RS_PAD, LCD_DB4_PAD,
			LCD_DB5_PAD, LCD_DB6_PAD, LCD_DB7_PAD };
	uint8_t i;
	//for (i =0; i< sizeof(linies); i++){
	//	GPIO_ModePushPull(LCD_PORT, linies[i]);
	//}
	//uint8_t linies[] = {LCD_BL_BIT, LCD_E_BIT, LCD_RS_BIT, LCD_DB4_BIT,
	//	LCD_DB5_BIT, LCD_DB6_BIT, LCD_DB7_BIT};
	for (i =0; i< sizeof(linia); i++){
		//Bits 2n++ a '0'
		LCD_PORT -> MODER=(LCD_PORT->MODER)& (~BIT(2*linia[i]+1));
		//Bits 2n a ‘1’
		LCD_PORT -> MODER=(LCD_PORT->MODER)| (BIT(2*linia[i]));
		LCD_PORT -> ODR = (LCD_PORT->ODR) | (BIT(linia[i]));
	}
 }


// Send 4 bits to the LCD and generates an enable pulse
//     nibbleCmd : Bits 3..0 Nibble to send to DB7...DB4
//     RS        : TRUE (RS="1")   FALSE (RS="0")
static void lcdNibble(int32_t nibbleCmd, int32_t RS) {
	// This code is requested in P2.6
	//Posem els bits alts a 0
	LCD_PORT->ODR = (LCD_PORT->ODR)& (~(0xF<<LCD_DB4_PAD));
	//assignem els valors de nibbleCmd als bits baixos
	LCD_PORT->ODR = (LCD_PORT->ODR) | ((nibbleCmd & 0xF)<<LCD_DB4_PAD);
	if (RS) {
		(LCD_PORT->BSRR.H.set) = LCD_RS_BIT;
	} else {
		(LCD_PORT->BSRR.H.clear) = LCD_RS_BIT;
	}
	// Fer pols de senyal d'habilitaciÃ³
	DELAY_US(10);
	(LCD_PORT->BSRR.H.set) = LCD_E_BIT;
	DELAY_US(10);
	(LCD_PORT->BSRR.H.clear) = LCD_E_BIT;
	DELAY_US(10);
}

/********** PUBLIC FUNCTIONS TO IMPLEMENT ************************
 Those functions will be used outside of lcd.c so there will
 be prototypes for them in lcd.h.
 ******************************************************************/

// Backlight configuration
//     on evaluates to TRUE   Turn on backlight
//     on evaluates to FALSE  Turn off backlight
void LCD_Backlight(int32_t on) {
	if (on) {
		// Turn on the Backlight using the BSRR register
		(LCD_PORT->BSRR.H.set) = LCD_BL_BIT;
	} else {
		// Turn off the Backlight using the BSRR register
		(LCD_PORT->BSRR.H.clear) = LCD_BL_BIT;
	}
}

// Clear the LCD and set the cursor position at 0,0
void LCD_ClearDisplay(void){
	lcdNibble(0, 0);
	lcdNibble(1,0);
	DELAY_US(1000);
}


// Configures the display
//     If Disp is TRUE turn on the display, if not, turns off
//     If Cursor is TRUE show the cursor, if not, hides it
//     If Blink is TRUE turn on blinking, if not, deactivate blinking
void LCD_Config(int32_t Disp,int32_t Cursor,int32_t Blink){
	lcdNibble(0,0);
	lcdNibble((1<<3)+(Disp<<2)+(Cursor<<1)+(Blink),0);
	DELAY_US(40);
}

// Set the cursor at the given position
//    col: Columnn (0..LCD_COLUMNS-1)
//    row: Row     (0..LCD_ROWS-1)
void LCD_GotoXY(int32_t col, int32_t row) {
	// This code is requested in P2.7
	int32_t num=8;//Primera fila
	if(!(row==0)){ 
	// 2a fila C=12=8+4 (HEXA) 
	num=num|0x04;
	}
	lcdNibble(num,0);
	lcdNibble(col,0);
	DELAY_US(40);
} 


// Send a character to the LCD at the current position
// car: Charater to send
void LCD_SendChar(char car) {
// This code is requested in P2.7
	lcdNibble(car>>4,1); // 1a transferÃ¨ncia (Ãºltims 4 bits)
	lcdNibble(car,1); // 2a transferÃ¨ncia
	DELAY_US(50); //Esperem 50 us
}
// Send a string to the LCD at the current position
// string: String to send
void LCD_SendString(char *string) {
	// This code is requested in P2.7
	uint32_t i = 0;
	// Enviem l'string caracter a caracter
	while(string[i]!=0){
		LCD_SendChar(string[i]);
		i++;
	}
} 

/********** PUBLIC FUNCTIONS ALREADY IMPLEMENTED ***************
 Those functions are provided to reduce the codding effort
 for the LCD module
 ****************************************************************/

// Init the LCD in 4 bits mode
// This function depends on the implementation of the
// private functions lcdSendNibble and lcdGPIOInit
void LCD_Init(void) {
	// Initializes the LCD GPIOs
	lcdGPIOInit();

	// Wait 50ms for vdd to stabilize
	SLEEP_MS(50);

	// Init command 1
	lcdNibble(0x03, 0);
	SLEEP_MS(10); // Wait > 4.1ms

	// Repeat command
	lcdNibble(0x03, 0);

	// Wait > 100us
	DELAY_US(500);

	// Repeat command
	lcdNibble(0x03, 0);

	// Init command 2
	lcdNibble(0x02, 0);

	// Mode 4 bits 2 lines 5x8 chars
	lcdNibble(0x08, 0);
	lcdNibble(0x02, 0);
	DELAY_US(50);

	// Display on blink on and cursor on
	lcdNibble(0x00, 0);
	lcdNibble(0x0E, 0);
	DELAY_US(50);

	// Clear Display
	lcdNibble(0x00, 0);
	lcdNibble(0x01, 0);
	SLEEP_MS(5);

	// Cursor set direction
	lcdNibble(0x00, 0);
	lcdNibble(0x06, 0);
	DELAY_US(50);

	// Set ram address
	lcdNibble(0x08, 0);
	lcdNibble(0x00, 0);
	DELAY_US(50);

	// Send OK
	lcdNibble(0x04, 1);
	lcdNibble(0x0F, 1);
	DELAY_US(50);
	lcdNibble(0x04, 1);
	lcdNibble(0x0B, 1);
	DELAY_US(50);
}



