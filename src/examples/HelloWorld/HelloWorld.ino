
#include <ST7567_TM9624.h>
#include <glcdfont.c>


#define PIN_LCD_BL     6 // LCD pin 1; any PWM
#define PIN_LCD_SCK   13 // LCD pin 5; SPI SCK
#define PIN_LCD_DIN   11 // LCD pin 6; SPI MOSI
#define PIN_LCD_CS    10 // LCD pin 2; SPI SS
#define PIN_LCD_A0     9 // LCD pin 4; A0, sometime it called D/C or RS
#define PIN_LCD_RST    8 // LCD pin 3;

ST7567_TM9624 glcd(PIN_LCD_A0, PIN_LCD_RST, PIN_LCD_CS); // Hardware SPI with CS
//ST7567_TM9624 glcd(PIN_LCD_DIN, PIN_LCD_SCK, PIN_LCD_A0, PIN_LCD_RST, PIN_LCD_CS); //Software SPI
//ST7567_TM9624 glcd(-1, -1, PIN_LCD_A0, PIN_LCD_RST, PIN_LCD_CS); // Hardware SPI with CS
//ST7567_TM9624 glcd(PIN_LCD_A0, PIN_LCD_RST); // Hardware SPI without CS


// the buffer for the image display
extern uint8_t st7567_tm9624_buffer[288];

// The setup() method runs once, when the sketch starts
void setup(){
  Serial.begin(115200);
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, LOW);
  
  // initialize and set the contrast
  glcd.begin(8); //0-63, good for 6-10

  glcd.display(); // show splashscreen
  delay(3000);
  
  glcd.clear();
  glcd.clear_display();
  
  glcd.drawstring(0, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz");
  glcd.display();
}

void blinkBL(){
  digitalWrite(PIN_LCD_BL, HIGH);
  delay(1000);
  digitalWrite(PIN_LCD_BL, LOW);
  delay(250);
}

int con = 8;
void loop()
{
  // test the contrast
  Serial.println( con, DEC);
  glcd.set_brightness( con );
  con++;
  if(con > 20)
    con = 0;
  blinkBL();
}
