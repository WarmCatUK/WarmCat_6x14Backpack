/*
 *  6 digit, 14 segment display packpack.
 *  Created April 2019. v0.9.2
 *  Wayne K Jones - WarmCat Solutions Ltd.
 *  Released under Creative Commons Attribution-ShareAlike 3.0 Licence
 *  https://github.com/WarmCatUK/WarmCat_6x14Backpack
 *  Connections:
 *  SDA - Arduino Uno A4 / Arduino Leonardo/Pro Micro 2
 *  SCL - Arduino Uno A5 / Arduino Leonardo/Pro Micro 3
 *  VDD - 5v/3.3v
 *  VI2C - Logic voltage (Arduino Uno/Leonardo use 5v)
 *  GND - GND
 *  
 *  Connect up to 8 backpacks, solder jumpers
 *  A2, A1, A0 in a binary fashion ie:
 *  000 = backpack 0
 *  001 = backpack 1
 *  010 = backpack 2
 *  011 = backpack 3 and so on.
 */

#include <WarmCat_6x14Backpack.h>
// create an instance of the WarmCat6x14 class and
// SPECIFY NUMBER OF BACKPACKS CONNECTED:
WarmCat6x14 myDisp(2);

void setup() {
  Serial.begin(9600);
  myDisp.begin();  // initialise the display(s) (required)
  exampleCode();  // comment out and play with your own code :-)
  Serial.println("Hello, type to display!");
}

void loop() {
  // ## PRINT FROM SERIAL MONITOR ## //
  while (! Serial.available()) return;
  char c = Serial.read();
  if (! isprint(c)) return;
  myDisp.scrollSerialText(c, 150); //150 = Scroll Rate
}



void exampleCode() {
  
  // scroll text across display(s) 
  // first specify the message to be displayed,
  // then scroll speed (smaller is faster)
  char message[] = "MODULAR 6X14 SEGMENT ALPHANUMERIC DISPLAY BACKPACK";
  myDisp.scrollText(message,150);
  delay(800);
  // scroll speed will default to 120 if not specified
  myDisp.scrollText("Awesome scrolly text");

  // scroll dots across the display
  myDisp.dots();

  // display 6 characters to specified display
  myDisp.disp6Char("123456", 0);
  myDisp.disp6Char(" MEOW ", 1);
  delay(1000);

  // swirl each digit one at a time
  myDisp.swirly(10);

  // swirl all digits at once
  // scroll speed for "swirly" and "swirlyAll will default to 20 if not specified
  myDisp.swirlyAll();
  
  delay(800);

  // manually clear the display if required
  myDisp.clear();

  // to display a single character
  // backpack number, digit, character, display dp
  myDisp.dispChar(0,1,'$');
  myDisp.dispChar(0,2,'3',true); 
  myDisp.dispChar(0,3,'9');
  myDisp.dispChar(0,4,'5');

  myDisp.disp6Char("BLINKY", 0);
  // blink displays 0=off 3=fastest
  myDisp.blink(2);
  delay(2000);
  // turn off blinking
  myDisp.blink(0);

  myDisp.scrollText("BrightBright",80);
  
  // set brightness from 0 - 15
  // 15 being brightest
  myDisp.setBrightness(5);
  delay(1600);
  myDisp.setBrightness(15);
  delay(800);

  myDisp.clear();
}
