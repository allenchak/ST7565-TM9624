# TM9624ACCWGWA2
TianMa TM9624ACCWGWA2 is 96x24 LCD display using ST7567 controller, using serial interface.
I would like say thankyou to ladyada, because this library based on [ST7565-LCD](https://github.com/adafruit/ST7565-LCD/) version.


## Important Notes
- Strongly recommended using Hardware SPI, have a significant speed up.


## How to use
* `ST7567_TM9624 glcd(LCD_A0, LCD_RST, LCD_CS);`
* `glcd.begin(8);` // initialize and set the contrast to 16 (Range: 0-63)
* `glcd.display();` // show splashscreen


## Pinout
| Pin | Description | Arduino HW SPI |
| --- | --- | --- |
| 1 | Backlight | D6 *1 |
| 2 | /CS | SPI - SCK, D10 |
| 3 | /RST | D8 |
| 4 | D/C (A0) | D9 |
| 5 | CLK | SPI - SCK, D13 |
| 6 | DIN | SPI - MOSI, D11 |
| 7 | VCC | 3.3v |
| 8 | GND | GND |
| 9 | Cap | Capacitor 100nF (103) *2 |

Remark \*1: with 300 ohm
Remark \*2: should be work >= 1uF capacitor


## Schematic
![](https://github.com/allenchak/LM6023/blob/master/resources/LM6023.png?raw=true)


## TODO
- More detail of this readme file
- Test and allow to config the SPI clock speed
- Add Demo video
- Add Schematic
- Merge to using Adafruit GFX library