
#include "WarmCat6x14Backpack.h"
 
//const uint8_t addr = 0x70; // HT16K33 default address

//const int DispCount = 2;  // How many backpacks are you using?

//uint16_t displayBuffer[8];
//int scrollBuffer [DispCount][8];


char stringtrumpet[] = "Testing some serious shit here right now like.....";
char message[] = "MODULAR 6 X 14 SEGMENT ALPHANUMERIC DISPLAY BACKPACK PROTOTYPE";

WarmCat6x14 disp(2);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  disp.init();

}



 
void loop() {


  disp.dots();
  disp.disp6Char(" WARMC", 1);
  disp.disp6Char("AT.UK ", 0);
  delay(800);
  disp.clear();
  disp.scrollText(message,150);
  //disp.emptyScrollBuffer();
  
  
  
}
