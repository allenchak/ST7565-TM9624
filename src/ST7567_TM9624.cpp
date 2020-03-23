

#ifdef __AVR__
#include <avr/pgmspace.h>
#include <util/delay.h>
#endif

#ifndef _delay_ms
  #define _delay_ms(t) delay(t)
#endif

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif


#include <stdlib.h>
#include <SPI.h>
#include "ST7567_TM9624.h"

#define ST7567_STARTBYTES    35
#define CMD_DELAY_INTERVAL    5  // uS, microsecond
#define BUFFER_SIZE         288


// a handy reference to where the pages are on the screen
const uint8_t pagemap[] = { 1, 2, 3, 0, 4, 5, 6, 7 };  //Actually 96 x 24 only need 3 pages

// a 5x7 font table
const extern uint8_t PROGMEM font[];

// the memory buffer for the LCD
uint8_t st7567_tm9624_buffer[288] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0x30,0xF0,0x00,0x00,0x00,0x00,
	0x00,0x00,0xF8,0x1C,0x00,0x00,0x00,0xF8,0x1C,0x00,0x00,0x00,0x00,0x80,0x40,0xC0,
	0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x30,
	0x00,0x00,0x00,0x80,0x40,0x40,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x80,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x60,0x30,0x1C,0x0E,0x0B,0x08,0x04,0x05,0x07,0x1C,0x70,0x60,
	0x00,0xFF,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x3E,0x4B,0x44,0x42,0x21,
	0x10,0x00,0x00,0x7F,0x38,0x04,0x02,0x01,0x03,0x3E,0x20,0x20,0x00,0x00,0x03,0x00,
	0x00,0x40,0x47,0x44,0x48,0x38,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
	0xFC,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x20,0x10,0x10,0xE0,0x00,
	0x00,0x00,0xF0,0x9F,0x41,0x20,0x10,0x10,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,
	0x18,0x18,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x0F,0x08,0x08,0x06,0x03,0x0F,0x08,
	0x00,0x00,0x0F,0x09,0x08,0x04,0x06,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

uint8_t st7567_tm9624_buffer_flipped[288] = { /* 96 * 24 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x0C,0x0F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x1F,0x38,0x00,0x00,0x00,0x1F,0x38,0x00,0x00,0x00,0x00,0x01,0x02,0x03,
	0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0C,
	0x00,0x00,0x00,0x01,0x02,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x06,0x0C,0x38,0x70,0xD0,0x10,0x20,0xA0,0xE0,0x38,0x0E,0x06,
	0x00,0xFF,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x7C,0xD2,0x22,0x42,0x84,
	0x08,0x00,0x00,0xFE,0x1C,0x20,0x40,0x80,0xC0,0x7C,0x04,0x04,0x00,0x00,0xC0,0x00,
	0x00,0x02,0xE2,0x22,0x12,0x1C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x3F,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x08,0x08,0x07,0x00,
	0x00,0x00,0x0F,0xF9,0x82,0x04,0x08,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,
	0x18,0x18,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0xF0,0x10,0x10,0x60,0xC0,0xF0,0x10,
	0x00,0x00,0xF0,0x90,0x10,0x20,0x60,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


void ST7567_TM9624::drawbitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color) {
  for (uint8_t j=0; j<h; j++) {
    for (uint8_t i=0; i<w; i++ ) {
      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
        my_setpixel(x+i, y+j, color);
      }
    }
  }
}

void ST7567_TM9624::drawstring(uint8_t x, uint8_t line, char *c) {
  while (c[0] != 0) {
    drawchar(x, line, c[0]);
    c++;
    x += 6; // 6 pixels wide
    if (x + 6 > LCDWIDTH) {
      x = 0;    // ran out of this line
      line++;
    }
    if (line >= (LCDHEIGHT/8))
      return;        // ran out of space :(
  }
}

void ST7567_TM9624::drawstring_P(uint8_t x, uint8_t line, const char *str) {
  while (1) {
    char c = pgm_read_byte(str++);
    if (! c)
      return;
    drawchar(x, line, c);
    x += 6; // 6 pixels wide
    if (x + 6 > LCDWIDTH) {
      x = 0;    // ran out of this line
      line++;
    }
    if (line >= (LCDHEIGHT/8))
      return;        // ran out of space :(
  }
}

void ST7567_TM9624::drawchar(uint8_t x, uint8_t line, uint8_t c) {
  for (uint8_t i =0; i<5; i++ ) {
    st7567_tm9624_buffer[x + (line*LCDWIDTH) ] = pgm_read_byte(font+(c*5)+i);
    x++;
  }
}


// bresenham's algorithm - thx wikpedia
void ST7567_TM9624::drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  uint8_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}


  for (; x0<=x1; x0++) {
    if (steep) {
      my_setpixel(y0, x0, color);
    } else {
      my_setpixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// filled rectangle
void ST7567_TM9624::fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    for (uint8_t j=y; j<y+h; j++) {
      my_setpixel(i, j, color);
    }
  }
}

// draw a rectangle
void ST7567_TM9624::drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  // stupidest version - just pixels - but fast with internal buffer!
  for (uint8_t i=x; i<x+w; i++) {
    my_setpixel(i, y, color);
    my_setpixel(i, y+h-1, color);
  }
  for (uint8_t i=y; i<y+h; i++) {
    my_setpixel(x, i, color);
    my_setpixel(x+w-1, i, color);
  }
}

// draw a circle outline
void ST7567_TM9624::drawcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  my_setpixel(x0, y0+r, color);
  my_setpixel(x0, y0-r, color);
  my_setpixel(x0+r, y0, color);
  my_setpixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    my_setpixel(x0 + x, y0 + y, color);
    my_setpixel(x0 - x, y0 + y, color);
    my_setpixel(x0 + x, y0 - y, color);
    my_setpixel(x0 - x, y0 - y, color);
    
    my_setpixel(x0 + y, y0 + x, color);
    my_setpixel(x0 - y, y0 + x, color);
    my_setpixel(x0 + y, y0 - x, color);
    my_setpixel(x0 - y, y0 - x, color);
  }
}

void ST7567_TM9624::fillcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color) {
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  for (uint8_t i=y0-r; i<=y0+r; i++) {
    my_setpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    for (uint8_t i=y0-y; i<=y0+y; i++) {
      my_setpixel(x0+x, i, color);
      my_setpixel(x0-x, i, color);
    } 
    for (uint8_t i=y0-x; i<=y0+x; i++) {
      my_setpixel(x0+y, i, color);
      my_setpixel(x0-y, i, color);
    }
  }
}

void ST7567_TM9624::my_setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    st7567_tm9624_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);
  else
    st7567_tm9624_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8);
}

// the most basic function, set a single pixel
void ST7567_TM9624::setpixel(uint8_t x, uint8_t y, uint8_t color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    st7567_tm9624_buffer[x+ (y/8)*LCDWIDTH] |= _BV(y%8);
  else
    st7567_tm9624_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(y%8);
}

// the most basic function, get a single pixel
uint8_t ST7567_TM9624::getpixel(uint8_t x, uint8_t y) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return 0;

  return (st7567_tm9624_buffer[x+ (y/8)*LCDWIDTH] >> (7-(y%8))) & 0x1;  
}

void ST7567_TM9624::set_brightness(uint8_t val) {
    st7567_tm9624_command(CMD_SET_CONTRAST);
    st7567_tm9624_command(val & CMD_SET_CONTRAST_LEVEL);
}

void ST7567_TM9624::begin(uint8_t contrast) {
  // set pin directions
  if (sid > 0) {
    pinMode(sid, OUTPUT);
    pinMode(sclk, OUTPUT);
  } else {
    SPI.begin();
    /*
    //TODO
    //SPI.setClockDivider(SPI_CLOCK_DIV4); // SPI_CLOCK_DIV8 for UNO ??, 2, 4, 8, 16, 32, 64, 128
    */
  }
  pinMode(a0, OUTPUT);
  pinMode(rst, OUTPUT);
  if (cs > 0)
    pinMode(cs, OUTPUT);
  
  st7567_tm9624_init(); // must be completed within 5 ms; Full doc page 148
  set_brightness(contrast);
}

void ST7567_TM9624::st7567_tm9624_init(void) {
  // toggle RST low to reset; CS low so it'll listen to us
  if (cs > 0)
    digitalWrite(cs, LOW);

  //https://www.newhavendisplay.com/appnotes/datasheets/LCDs/ST7567.pdf
  // according to page 29
  digitalWrite(rst, LOW);
  _delay_ms(10);
  digitalWrite(rst, HIGH);
  _delay_ms(10);

  // LCD bias select
  st7567_tm9624_command(CMD_SET_BIAS_8); //(11)
  // ADC select
  st7567_tm9624_command(CMD_SET_ADC_NORMAL); //(8)
  // SHL select (Output Status Register)
  st7567_tm9624_command(CMD_SET_COM_REVERSE); //(15)

  // Setting the built-in resistance radio for regulation of the V5 voltage
  st7567_tm9624_command(CMD_SET_REG_RATIO | (7 & CMD_SET_REG_RATIO_LEVEL)); //(17) val= 0-7
  // Electronic Volume Register (Contrast)
  set_brightness( 8 ); //(18) val= 0-63

  // Set Power Control (VB=1, VR=1, VF=1)
  st7567_tm9624_command(CMD_SET_POWER_CONTROL | CMD_SET_PWRCTRL_VB_ON | CMD_SET_PWRCTRL_VR_ON | CMD_SET_PWRCTRL_VF_ON); //(16)

  //Entire Display = OFF(Normal)
  st7567_tm9624_command(CMD_SET_ALLPTS_NORMAL); //Needed?
  //Normal/Reverse Display = Normal
  st7567_tm9624_command(CMD_SET_DISP_NORMAL); //Needed?
  

  // according to page 44
  // Initial display line
  st7567_tm9624_command(CMD_SET_DISP_START_LINE); //(2)
  // set page address
  st7567_tm9624_command(CMD_SET_PAGE);  //(3)
  // set column address
  st7567_tm9624_command(CMD_SET_COLUMN_UPPER);  //(4)
  st7567_tm9624_command(CMD_SET_COLUMN_LOWER);
  //Display ON
  st7567_tm9624_command(CMD_DISPLAY_ON);

  // write display data
}

inline void ST7567_TM9624::spiwrite(uint8_t c) {
  #if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
      if (sid > 0) {
        shiftOut(sid, sclk, MSBFIRST, c);
      }
      else {
        SPI.transfer(c);
      }
  #else
      int8_t i;
      for (i=7; i>=0; i--) {
          digitalWrite(sclk, LOW);
          delayMicroseconds(CMD_DELAY_INTERVAL);      //need to slow down the data rate for Due and Zero
          if (c & _BV(i))
              digitalWrite(sid, HIGH);
          else
              digitalWrite(sid, LOW);
          //delayMicroseconds(CMD_DELAY_INTERVAL);      //need to slow down the data rate for Due and Zero
          digitalWrite(sclk, HIGH);
      }
  #endif
}


void ST7567_TM9624::st7567_tm9624_command(uint8_t c) {
  digitalWrite(a0, LOW);
        
  spiwrite(c);
}


void ST7567_TM9624::st7567_tm9624_data(uint8_t c) {
  digitalWrite(a0, HIGH);
  
  spiwrite(c);
}


void ST7567_TM9624::display(void) {
  uint8_t col, p;

  for(p = 0; p < 3; p++) {
    // according to page 25
    st7567_tm9624_command(CMD_SET_PAGE | pagemap[p]);

    col = 0;

    st7567_tm9624_command(CMD_SET_COLUMN_LOWER | ((col+ST7567_STARTBYTES) & 0xf));
    st7567_tm9624_command(CMD_SET_COLUMN_UPPER | (((col+ST7567_STARTBYTES) >> 4) & 0x0F));

    st7567_tm9624_command(CMD_RMW);
    for(; col < LCDWIDTH ; col++) {
      st7567_tm9624_data( st7567_tm9624_buffer[ (LCDWIDTH * p) + col ]);
    }
    st7567_tm9624_command(CMD_RMW_CLEAR);
  }
}


// clear everything
void ST7567_TM9624::clear(void) {
  memset(st7567_tm9624_buffer, 0, BUFFER_SIZE);
}


// this doesnt touch the buffer, just clears the display RAM - might be handy
void ST7567_TM9624::clear_display(void) {
  uint8_t p, c;
  
  for(p = 0; p < 3; p++) {
    st7567_tm9624_command(CMD_SET_PAGE | p);
    for(c = 0; c < LCDWIDTH; c++) {
      st7567_tm9624_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7567_tm9624_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7567_tm9624_data(0x0);
    }     
  }
}