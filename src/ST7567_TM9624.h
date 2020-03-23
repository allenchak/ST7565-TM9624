

#ifndef _AC_ST7567_TM9624_H
#define _AC_ST7567_TM9624_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 96
#define LCDHEIGHT 24



//https://www.newhavendisplay.com/appnotes/datasheets/LCDs/ST7567.pdf  #page 21
#define CMD_DISPLAY_OFF            0xAE    // (1) Display ON / OFF
#define CMD_DISPLAY_ON             0xAF
#define CMD_SET_DISP_START_LINE    0x40    // (2) Initial Display Line

#define CMD_SET_PAGE               0xB0    // (3) Set Page Address
#define CMD_SET_COLUMN_UPPER       0x10    // (4.1) Set Column Address (Upper 4 bit)
#define CMD_SET_COLUMN_LOWER       0x00    // (4.2) Set Column Address (Lower 4 bit)

#define CMD_SET_ADC_NORMAL         0xA0    // (8) ADC Select
#define CMD_SET_ADC_REVERSE        0xA1
#define CMD_SET_DISP_NORMAL        0xA6    // (9) Normal Display
#define CMD_SET_DISP_REVERSE       0xA7
#define CMD_SET_ALLPTS_NORMAL      0xA4    // (10) Entire Display (Display All Points)
#define CMD_SET_ALLPTS_ON          0xA5

#define CMD_SET_BIAS_9             0xA2    // (11) Set LCD Bias 1/9
#define CMD_SET_BIAS_8             0xA2    //              Bias 1/9
#define CMD_SET_BIAS_7             0xA3    //              Bias 1/7
#define CMD_SET_BIAS_6             0xA3    //              Bias 1/7
#define CMD_RMW                    0xE0    // (12) Read-Modify-Write
#define CMD_RMW_CLEAR              0xEE    // (13) `END`, Clear Read-Modify-Write
#define CMD_INTERNAL_RESET         0xE2    // (14) Reset;;
#define CMD_SET_COM_NORMAL         0xC0    // (15) Set Common Output Status Register
#define CMD_SET_COM_REVERSE        0xC8

#define CMD_SET_POWER_CONTROL      0x28    // (16) Power Control
#define CMD_SET_PWRCTRL_VF_ON      0x1
#define CMD_SET_PWRCTRL_VF_OFF     0
#define CMD_SET_PWRCTRL_VR_ON      0x2
#define CMD_SET_PWRCTRL_VR_OFF     0
#define CMD_SET_PWRCTRL_VB_ON      0x4
#define CMD_SET_PWRCTRL_VB_OFF     0

#define CMD_SET_REG_RATIO          0x20    // (17) Regulation Ratio (Contrast related?)
#define CMD_SET_REG_RATIO_LEVEL    0x7     // valid last 3 bits

#define CMD_SET_CONTRAST           0x81    // (18) Electronic Volume Register Set (Double byte) (Contrast)
#define CMD_SET_CONTRAST_LEVEL     0x3F    // Also called "Electronic Control Register", valid last 6 bits

#define CMD_SET_BOOSTER            0xF8    // (19) Booster (Double byte) (Contrast)
#define CMD_SET_BOOSTER_4X         0x0
#define CMD_SET_BOOSTER_5X         0x1
#define CMD_SET_BOOSTER_6X         0x2

#define CMD_NOP                    0xE3    // (21) Not sure usefull or not
#define CMD_TEST                   0xFE    // (22) Full Doc Page 146



class ST7567_TM9624 {
  public:
    ST7567_TM9624(int8_t SID, int8_t SCLK, int8_t A0, int8_t RST, int8_t CS) :sid(SID), sclk(SCLK), a0(A0), rst(RST), cs(CS) {}
    ST7567_TM9624(int8_t SID, int8_t SCLK, int8_t A0, int8_t RST) :sid(SID), sclk(SCLK), a0(A0), rst(RST), cs(-1) {}

    ST7567_TM9624(int8_t A0, int8_t RST, int8_t CS) :sid(-1), sclk(-1), a0(A0), rst(RST), cs(CS) {} //For SPI
    ST7567_TM9624(int8_t A0, int8_t RST) :sid(-1), sclk(-1), a0(A0), rst(RST), cs(-1) {} //For SPI


    void st7567_tm9624_init(void);
    void begin(uint8_t contrast);
    void st7567_tm9624_command(uint8_t c);
    void st7567_tm9624_data(uint8_t c);
    void set_brightness(uint8_t val);
    void clear_display(void);
    void clear();
    void display();

    void setpixel(uint8_t x, uint8_t y, uint8_t color);
    uint8_t getpixel(uint8_t x, uint8_t y);
    void fillcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
    void drawcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
    void drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
    void fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
    void drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
    void drawchar(uint8_t x, uint8_t line, uint8_t c);
    void drawstring(uint8_t x, uint8_t line, char *c);
    void drawstring_P(uint8_t x, uint8_t line, const char *c);

    void drawbitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);

  private:
    int8_t sid, sclk, a0, rst, cs;
    void spiwrite(uint8_t c);
    void my_setpixel(uint8_t x, uint8_t y, uint8_t color);
    //uint8_t buffer[128*64/8]; 
};

#endif