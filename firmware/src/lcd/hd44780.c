#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <time.h>

#include "definitions.h"
#include "peripheral/coretimer/plib_coretimer.h"
#include "hd44780.h"

#define LCD_DATA_TRIS TRISD
#define LCD_DATA_TRIS_SET TRISDSET
#define LCD_DATA_TRIS_CLR TRISDCLR
#define LCD_DATA_LAT LATD
#define LCD_DATA_SET LATDSET
#define LCD_DATA_CLR LATDCLR
#define LCD_DATA_PORT PORTD
#define LCD_DATA_SHIFT 2 //DB4 at bit
#define LCD_DATA_MASK (0x0000000F<<LCD_DATA_SHIFT)

#define SET_RS 	LATGbits.LATG12 = 1
#define CLR_RS 	LATGbits.LATG12 = 0

#define SET_RW 	LATGbits.LATG14 = 1
#define CLR_RW 	LATGbits.LATG14 = 0

#define SET_E 	LATEbits.LATE1 = 1
#define CLR_E 	LATEbits.LATE1 = 0

#define SET_BACKLIGHT LATCbits.LATC15 = 1
#define CLR_BACKLIGHT LATCbits.LATC15 = 0

#define LCD_HOME    0x00
#define LCD_CLS     0x01
#define LCD_LOCATE  0x02

void lcd_write_cmd(uint8_t cmd);
void lcd_write_data(uint8_t data);

#if USE_RW
uint8_t check_BF(void);
#endif

static inline void data_dir_out(void) {
    LCD_DATA_TRIS_CLR = LCD_DATA_MASK;
}

#if USE_RW
static inline void data_dir_in(void) {
    LCD_DATA_TRIS_SET = LCD_DATA_MASK;
}
#endif

static inline void lcd_sendHalf(uint8_t data) {
    LCD_DATA_CLR = LCD_DATA_MASK;
    uint32_t to_send = ((uint32_t)data << LCD_DATA_SHIFT) & LCD_DATA_MASK;
    LCD_DATA_SET = to_send;
}

#if USE_RW == 1
static inline uint8_t lcd_readHalf(void)
{    
    uint8_t result = (LCD_DATA_PORT & LCD_DATA_MASK) >> LCD_DATA_SHIFT;
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
    CORETIMER_DelayUs(120);
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
    lcd_write_data(c);
}


void lcd_str(const char * str) {
	register char znak;
	while ( (znak=*(str++)) ) lcd_char( znak );
}

uint16_t lcd_str_part(const char* str, const uint16_t len) {
    register char znak;
    uint16_t cnt = 0;
    while ( (znak=*(str++)) ) {
        lcd_char( znak );
        cnt++;
        if (cnt >= len) {
            break;
        }
    }
    return len-cnt;
}

uint16_t lcd_utf8str_part(const char* str, const uint16_t len) {
    char znak;
    uint16_t cnt=0;
    while ( (znak=*str) ) {
        str++;
        if (znak & (1<<7)) {
            uint16_t znak_utf = znak<<8 | (uint8_t)*str;
            switch (znak_utf) {
                case 0xC485:
                lcd_char('a');
                break;

                case 0xC487:
                lcd_char('c');
                break;
                
                case 0xC499:
                lcd_char('e');
                break;
                
                case 0xC582:
                lcd_char('l');
                break;
                
                case 0xC584:
                lcd_char('n');
                break;
                
                case 0xC3B3:
                lcd_char('o');
                break;
                
                case 0xC59B:
                lcd_char('s');
                break;
                
                case 0xC5BA:
                case 0xC5BC:
                lcd_char('z');
                break;
                
                case 0xC484:
                lcd_char('A');
                break;
                
                case 0xC486:
                lcd_char('C');
                break;
                
                case 0xC498:
                lcd_char('E');
                break;
                
                case 0xC581:
                lcd_char('L');
                break;
                
                case 0xC583:
                lcd_char('N');
                break;
                
                case 0xC393:
                lcd_char('O');
                break;
                
                case 0xC59A:
                lcd_char('S');
                break;
                
                case 0xC5B9:
                case 0xC5BB:
                lcd_char('Z');
                break;
                
                default:
                lcd_char('?');
                break;
            }
            str++;
        }
        else {
            lcd_char(znak);
        }
        cnt++;
        if (cnt >= len) {
            break;
        }
    }
    return len-cnt;
}

void lcd_str_padd_rest(const char* str, const uint16_t len, char padd) {
	uint8_t rest = lcd_str_part(str, LCD_COLS);
	while (rest) {
		lcd_char(padd);
		rest--;
	}
}

void lcd_utf8str_padd_rest(const char* str, const uint16_t len, char padd) {
	uint8_t rest = lcd_utf8str_part(str, LCD_COLS);
	while (rest) {
		lcd_char(padd);
		rest--;
	}
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
}


void lcd_home(void) {
    lcd_write_cmd( LCDC_CLS|LCDC_HOME );;
}

void lcd_set_backlight_state(bool state) {
    if (state == true) {
        CLR_BACKLIGHT;
    }
    else {
        SET_BACKLIGHT;
    }
}



void lcd_init( void ) {
    //LCD_LED_ON;

    vTaskDelay(15);
    CLR_E;
    CLR_RS;
    #if USE_RW == 1
    CLR_RW;
    #endif

	// jeszcze nie mozna uzywac Busy Flag
	SET_E;
	lcd_sendHalf(0x03);	// tryb 8-bitowy
	CLR_E;
	vTaskDelay(5);

	SET_E;
	lcd_sendHalf(0x03);	// tryb 8-bitowy
	CLR_E;
	vTaskDelay(100);

	SET_E;
	lcd_sendHalf(0x03);	// tryb 8-bitowy
	CLR_E;
	vTaskDelay(100);

	SET_E;
	lcd_sendHalf(0x02);// tryb 4-bitowy
	CLR_E;
	vTaskDelay(100);

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
	//lcd_cls();
    lcd_write_cmd( LCDC_CLS );
    #if USE_RW == 0
    vTaskDelay(5);
    #endif
}
