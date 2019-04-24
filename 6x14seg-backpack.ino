#include <Wire.h>
#include "WarmCat6x14Backpack.h"
 
//const uint8_t addr = 0x70; // HT16K33 default address

const int DispCount = 2;  // How many backpacks are you using?

uint16_t displayBuffer[8];
int scrollBuffer [DispCount][8];


char stringtrumpet[] = "6x14 SEGMENT CYBERCACHE - WARMCAT.UK";
char stringbitch[] = "THIS IS THE MOTHERFUCKING CYBERCACHE DISPLAY - TAKING GEOCACHING TO THE NEXT LEVEL WITH STATE-OF-THE-ART 8-BIT TECHNOLOGY";
char catmonster[] = "HELLO GRAEME, THIS IS MY MOTHERFUCKING CYBERCACHE";
char message[] = "MODULAR 6 X 14 SEGMENT ALPHANUMERIC DISPLAY BACKPACK PROTOTYPE";

void setup() {
  Serial.begin(115200);

  for (int disp=0; disp<DispCount; disp++) {
    Wire.begin();
    Wire.beginTransmission(DisplayNo[disp]);
    Wire.write(0x20 | 1); // turn on oscillator
    Wire.endTransmission();
    setBrightness(disp, 15);
    delay(2);
  }
 
 
  blink(0,0);
  blink(1,0);
  clear();
  //blink(0);
}

 
void loop() {
  const int dTime = 50;

 clear();

 dots();
 delay(500);
 clear();
 //loopSwirly();
 loopSwirly2();
 //loopSwirly3();
 clear();


 disp6Char(" WARMC", 1);
 showOnDisp(1);
 delay(2);
 disp6Char("AT.UK ", 0);
 showOnDisp(0);
 delay(5000);
 //scrollText(stringtrumpet, 200);
 //scrollyText(stringbitch, 80);
 //scrollText(stringbitch, 180);
 scrollText(message, 160);
 scrollText("Hello Fucktrumpets", 100);

  // Testing Dimming
  for (int boo = 0; boo <10; boo++) {
    for(int i = 15; i >= 0; i--){
      setBrightness(8, i);
      delay(40);
    }
    for(int i = 0; i <= 15; i++){
      setBrightness(8, i);
      delay(40);
    }
  }
  // End Test Dimming

 loopASCIIbasic();
 delay(1000);
 clear();

/*

  // Test blinking
  for(int i = 3 ; i > 0; i--){
    blink(i);
    delay(1000);
  }
  blink(0); // Turn blinking off
 
  // Test blanking
  for(int i = 0; i < 10; i++){
    blank();
    delay(dTime * 2);
  }
 

  // Test dimming
  for(int i = 15; i >= 0; i--){
    setBrightness(i);
    delay(dTime * 2);
  }
  clear();
 
  setBrightness(15);
*/
}





 


void showOnDisp(uint8_t disp) {
  Wire.beginTransmission(DisplayNo[disp]);
  Serial.print("Outputting to : ");
  Serial.println(DisplayNo[disp]);
  Wire.write(0x00); // start at address 0x0
 
  for (int i = 0; i < 8; i++) {
    //  Wire.write(displayBuffer[i] & 0xFF);   //why? in case byte is less than 8 bits?
    Wire.write(displayBuffer[i]);   
    Wire.write(displayBuffer[i] >> 8);    
  }
  Wire.endTransmission();  
}

void showall() {
  for (int disp = 0; disp<DispCount; disp++) {
    Wire.beginTransmission(DisplayNo[disp]);
    Serial.print("Outputting to : ");
    Serial.println(DisplayNo[disp]);
    Wire.write(0x00); // start at address 0x0
    for (int i = 0; i < 8; i++) {
      // Wire.write(displayBuffer[i] & 0xFF);   //why? in case byte is less than 8 bits?
      Wire.write(scrollBuffer[disp][i]);   
      Wire.write(scrollBuffer[disp][i] >> 8);    
    }
  Wire.endTransmission();  
  }
  
}

void clear(){
  for(int i = 0; i < 8; i++){
    displayBuffer[i] = 0;
  }
  for(int disp=0; disp<DispCount; disp++) {
    showOnDisp(disp);
  }
}
 
void setBrightness(uint8_t selectedDisplay, uint8_t b){
  if(b > 15) return;
  if (selectedDisplay > 7) { //then dim all displays
    for (int disp=0; disp<DispCount;disp++) {
      Wire.beginTransmission(DisplayNo[disp]);
      Wire.write(0xE0 | b); // Dimming command
      Wire.endTransmission();
    }
  } else { // else dim only selected display
    Wire.beginTransmission(DisplayNo[selectedDisplay]);
    Wire.write(0xE0 | b); // Dimming command
    Wire.endTransmission();
  }
  
}
 
void blank(){
  static boolean blankOn;
  for (int disp=0; disp<DispCount; disp++) {
    Wire.beginTransmission(DisplayNo[disp]);
    Wire.write(0x80 | blankOn); // Blanking / blinking command
    Wire.endTransmission();
  }
  blankOn = !blankOn;
}
 
void blink(uint8_t disp, uint8_t b){
  if(b > 3) return;
  Wire.beginTransmission(DisplayNo[disp]);
  Wire.write(0x80 | b << 1 | 1); // Blinking / blanking command
  Wire.endTransmission();
}

void dispChar(uint8_t digit, byte ascii) {
  byte index = ascii - 32;
  displayBuffer[digit] = FourteenSegmentASCII[index];
}

void emptyScrollBuffer() {
  for (int x = 0; x < DispCount; x++) {
    memset(scrollBuffer[x], 0, sizeof(scrollBuffer));
  }
  
}

void scrollText(char text[], int scrollrate) {
  //empty scrollBuffer
  emptyScrollBuffer();
  int textLen = strlen(text);  //get length of string
  //Test shit
  Serial.print("Scrolling Text: '");
  Serial.print(text);
  Serial.println("'");
  Serial.print("String length: ");
  Serial.println(textLen);
  //end of test shit
  clear(); //Clear Display first
  for (int i=0; i<=textLen-1; i++) {
    //for each character in string
    for (int disp = DispCount; disp >= 0; disp--) {
      // for each display backpack
      //for each digit
      scrollBuffer[disp][0]=scrollBuffer[disp][1];
      scrollBuffer[disp][1]=scrollBuffer[disp][2];
      scrollBuffer[disp][2]=scrollBuffer[disp][3];
      scrollBuffer[disp][3]=scrollBuffer[disp][4];
      scrollBuffer[disp][4]=scrollBuffer[disp][5];
      if (disp==0) {
        scrollBuffer[disp][5]=FourteenSegmentASCII[text[i]-32];
      } else {
        scrollBuffer[disp][5]=scrollBuffer[disp-1][0];
      }
    Serial.print(text[5+i]);
    if (i<textLen-1) { Serial.print("-"); }
    }
    delay(scrollrate);
    showall();
  }
  Serial.println("");
  Serial.println("Done!");
  delay(2000);
  
  
  
}

/*
 // OLD SCROLL 
void scrollyText(char text[], int scrollrate) {
  int textLen = strlen(text);  //get length of string
  char appendedText[textLen+6] = "";
  strcat (appendedText, "      ");
  strcat (appendedText, text);
  int appendedTextLen = strlen(appendedText);
  //Test shit
  Serial.print("Scrolling Text: '");
  Serial.print(appendedText);
  Serial.println("'");
  Serial.print("Orig String length: ");
  Serial.println(textLen);
  Serial.print("New String length: ");
  Serial.println(appendedTextLen);
  //end of test shit
  clear(); //Clear Display first
  for (int i=0; i<appendedTextLen-5; i++) {
    dispChar(0, appendedText[0+i]);
    dispChar(1, appendedText[1+i]);
    dispChar(2, appendedText[2+i]);
    dispChar(3, appendedText[3+i]);
    dispChar(4, appendedText[4+i]);
    dispChar(5, appendedText[5+i]);
    Serial.print(appendedText[5+i]);
    if (i<appendedTextLen-1) { Serial.print("-"); }
    show();
    delay(scrollrate);
  }
  Serial.println("");
  Serial.println("Done!");
  delay(2000);
}
*/

void disp6Char(char text[], uint8_t disp) {
  //clear();
  for (int x=0;x<6;x++) {
    displayBuffer[x] = FourteenSegmentASCII[text[x]-32];
  }
  showOnDisp(disp);
}



void loopASCII() {
  for (int x = 0 ; x < 96 ; x++) {
    for (int d=0; d<6; d++) {
      displayBuffer[d] = FourteenSegmentASCII[x];
    }
    for (int disp=0; disp<DispCount; disp++) {
      showOnDisp(disp);
    }
    delay(140);
  }
}
void loopASCIIbasic() {
  for (int x = 33 ; x < 59 ; x++) {
    for (int d=0; d<6; d++) {
      displayBuffer[d] = FourteenSegmentASCII[x];
    }
    for (int disp=0; disp<DispCount; disp++) {
      showOnDisp(disp);
    }
    delay(140);
  }
}
void loopSwirly() {
  for (int x = 0 ; x < 14 ; x++) {
    for (int d=0; d<6; d++) {
      displayBuffer[d] = Swirly[x];
    }
    for (int disp=0; disp<DispCount; disp++) {
      showOnDisp(disp);
    }
    delay(40);
  }
}

void loopSwirly2() {
  clear();
  for (int x = 0 ; x < 14 ; x++) {
    for (int d=0; d<6; d++) {
      displayBuffer[d] |= Swirly[x];
    }
    for (int disp=0; disp<DispCount; disp++) {
      showOnDisp(disp);
    }
    delay(40);
  }
  delay(80);
}

void loopSwirly3() {
  clear();
  for (int disp=0; disp<DispCount; disp++) {
    for (int d=5; d>=0; d--) {
      for (int x=0; x<14; x++) {
        displayBuffer[d] |= Swirly[x];
        showOnDisp(disp);
        delay(40);
      }
    }
    memset(displayBuffer, 0, sizeof(displayBuffer));
  }
}


void loopSwirly4() {
  for (int disp=0; disp<DispCount; disp++) {
    for (int x = 0 ; x < 14 ; x++) {
      for (int d=0; d<6; d++) {
        displayBuffer[d] |= Swirly[x];
      }
      showOnDisp(disp);
      delay(40);
    }
    memset(displayBuffer, 0, sizeof(displayBuffer));
  }
}

void AllDisplayOn() {
  for (int d=0; d<6; d++) {
    displayBuffer[d] = AllOn;
    }
    for (int disp=0; disp<DispCount; disp++) {
      showOnDisp(disp);
    }
  }

void dots() {
  for (int cunt=0; cunt<DispCount; cunt++) {
    for (int d=5; d>=0; d--) {
      displayBuffer[d] = 0b100000000000000;
      //dispChar(d, '.');
      showOnDisp(cunt);
      delay(100);
      }
      memset(displayBuffer, 0, sizeof(displayBuffer));
    }
  }
