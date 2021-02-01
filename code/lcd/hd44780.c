#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <plib.h>
#include <time.h>
#include "../delay/delay.h"

#include "hd44780.h"
#include "i2c.h"

#define SET_RS 	mpxLCD |= (1<<LCD_RS); 	SEND_I2C
#define CLR_RS 	mpxLCD &= ~(1<<LCD_RS); SEND_I2C

#define SET_RW 	mpxLCD |= (1<<LCD_RW); 	SEND_I2C
#define CLR_RW 	mpxLCD &= ~(1<<LCD_RW); SEND_I2C

#define SET_E 	mpxLCD |= (1<<LCD_E); 	SEND_I2C
#define CLR_E 	mpxLCD &= ~(1<<LCD_E); 	SEND_I2C


#if USE_RW
uint8_t check_BF(void);
#endif

uint8_t	mpxLCD;

static inline void data_dir_out(void) {
    mpxLCD	&= ~(1<<LCD_D7);
    mpxLCD	&= ~(1<<LCD_D6);
    mpxLCD	&= ~(1<<LCD_D5);
    mpxLCD	&= ~(1<<LCD_D4);
    SEND_I2C;
}

#if USE_RW
static inline void data_dir_in(void) {
    mpxLCD |= (1<<LCD_D7);
    mpxLCD |= (1<<LCD_D6);
    mpxLCD |= (1<<LCD_D5);
    mpxLCD |= (1<<LCD_D4);
    SEND_I2C;
}
#endif

static inline void lcd_sendHalf(uint8_t data) {
    if (data&(1<<0)) mpxLCD |= (1<<LCD_D4); else mpxLCD &= ~(1<<LCD_D4);
    if (data&(1<<1)) mpxLCD |= (1<<LCD_D5); else mpxLCD &= ~(1<<LCD_D5);
    if (data&(1<<2)) mpxLCD |= (1<<LCD_D6); else mpxLCD &= ~(1<<LCD_D6);
    if (data&(1<<3)) mpxLCD |= (1<<LCD_D7); else mpxLCD &= ~(1<<LCD_D7);
    SEND_I2C;
}

#if USE_RW == 1

static inline uint8_t lcd_readHalf(void)
{
	uint8_t result=0;
    uint8_t res=0;

    res = RECEIVE_I2C;
    if(res&(1<<LCD_D4)) result |= (1<<0);
    if(res&(1<<LCD_D5)) result |= (1<<1);
    if(res&(1<<LCD_D6)) result |= (1<<2);
    if(res&(1<<LCD_D7)) result |= (1<<3);

	return result;
}
#endif

void _lcd_write_byte(unsigned char _data) {

	data_dir_out();

	#if USE_RW == 1
		CLR_RW;
	#endif

	SET_E;
	lcd_sendHalf(_data >> 4);
	CLR_E;

	SET_E;
	lcd_sendHalf(_data);
	CLR_E;

	#if USE_RW == 1

		while( (check_BF() & (1<<7)) );

	#else
		_delay_us(120);
	#endif

}

#if USE_RW == 1
uint8_t _lcd_read_byte(void) {

	uint8_t result=0;
	data_dir_in();

	SET_RW;

	SET_E;

	result = (lcd_readHalf() << 4);	

	CLR_E;

	SET_E;
	result |= lcd_readHalf();
	CLR_E;

	return result;
}
#endif


#if USE_RW == 1
uint8_t check_BF(void) {

	CLR_RS;

	return _lcd_read_byte();
}
#endif


void lcd_write_cmd(uint8_t cmd) {

	CLR_RS;

	_lcd_write_byte(cmd);
}

void lcd_write_data(uint8_t data) {

	SET_RS;
	_lcd_write_byte(data);
}


void lcd_char(char c) {
	lcd_write_data( ( c>=0x80 && c<=0x87 ) ? (c & 0x07) : c);
}


void lcd_str(char * str) {
	register char znak;
	while ( (znak=*(str++)) ) lcd_char( znak );

}



void lcd_locate(uint8_t y, uint8_t x) {

	switch(y) {
		case 0: y = LCD_LINE1; break;

#if (LCD_ROWS>1)
	    case 1: y = LCD_LINE2; break;
#endif
#if (LCD_ROWS>2)
    	case 2: y = LCD_LINE3; break;
#endif
#if (LCD_ROWS>3)
    	case 3: y = LCD_LINE4; break;
#endif
	}

	lcd_write_cmd( (0x80 + y + x) );
}


void lcd_cls(void) {

	lcd_write_cmd( LCDC_CLS );

	#if USE_RW == 0
		_delay_ms(4.9);
	#endif
}


void lcd_home(void) {

	lcd_write_cmd( LCDC_CLS|LCDC_HOME );

	#if USE_RW == 0
		_delay_ms(4.9);
	#endif
}



void lcd_init( void ) {
    mpxLCD = 0;

    //LCD_LED_ON;

    delay_ms(15);
    mpxLCD &= ~(1<<LCD_E);
    mpxLCD &= ~(1<<LCD_RS);
    #if USE_RW == 1
        mpxLCD &= ~(1<<LCD_RW);
    #endif
    SEND_I2C;

	// jeszcze nie mozna uzywac Busy Flag
	SET_E;
	lcd_sendHalf(0x03);	// tryb 8-bitowy
	CLR_E;
	delay_ms(5);

	SET_E;
	lcd_sendHalf(0x03);	// tryb 8-bitowy
	CLR_E;
	delay_us(100);

	SET_E;
	lcd_sendHalf(0x03);	// tryb 8-bitowy
	CLR_E;
	delay_us(100);

	SET_E;
	lcd_sendHalf(0x02);// tryb 4-bitowy
	CLR_E;
	delay_us(100);

	// juz mozna uzywac Busy Flag
	// tryb 4-bitowy, 2 wiersze, znak 5x7
	lcd_write_cmd( LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7 );

	// wylaczenie kursora
	lcd_write_cmd( LCDC_ONOFF|LCDC_CURSOROFF );
	// wlaczenie wyswietlacza
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON );
	// przesuwanie kursora w prawo bez przesuwania zawartosci ekranu
	lcd_write_cmd( LCDC_ENTRY|LCDC_ENTRYR );

	// kasowanie ekranu
	lcd_cls();
}
