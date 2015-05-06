#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>

TinyScreen display = TinyScreen(0);

int RX=0;
int RY=0;
int LX=0;
int LY=0;
byte leftButton=0;
byte rightButton=0;
int getout = 0;

void setup(void) {
  Wire.begin();
  display.begin();
}
int x = 0;
void loop() {
  for(;;) { 
     x++;
     display.drawRect(0,0,96,64, 255, x);
     display.setFont(liberationSans_10ptFontInfo);
     display.setCursor(32,24);
     display.print(x);
     display.setCursor(0,50);
     display.print("Press Any Key");
     for(int n = 0; n < 250; n++) { 
     delay(1); getJoystick();  if(anyKey()) { getout=1; } } 
     if(getout == 1) { refresh(); break; } 
     if(x == 256) { x = 0; } 
  }

  for(;;) { 
   getJoystick();
   if(LX < -250) { x--; if (x == -1) {x = 255; } refresh(); }  
   if(RX < -250) { x--; if (x == -1) { x = 255; } refresh(); } 
   
   if(LX > 250) { x++; if (x == 256){ x = 0;} refresh(); }  
   if(RX > 250) { x++; if (x == 256){x = 0;} refresh(); }  

   if(LX < -80 & LX > -250) { delay(250); x--; if (x == -1){ x = 255;} refresh(); }
   if(RX < -80 & RX > -250) { delay(250); x--; if (x == -1){ x = 255;} refresh(); }     
   
   if(LX > 80 & LX < 250) { delay(250); x++; if(x == 256){ x = 0;} refresh(); }
   if(RX > 80 & RX < 250) { delay(250); x++; if(x == 256){ x = 0;} refresh(); }     
  }
}

void getJoystick(){
  Wire.requestFrom(0x22,6);
  int data[4];
  for(int i=0;i<4;i++){
    data[i]=Wire.read();
  }
  byte lsb=Wire.read();
  byte buttons=~Wire.read();
  leftButton=buttons&4;
  rightButton=buttons&8;
  for(int i=0;i<4;i++){
    data[i]<<=2;
    data[i]|= ((lsb>>(i*2))&3);
    data[i]-=511;
  }
  RX=data[0];
  RY=-data[1];
  LX=-data[2];
  LY=data[3];
}

bool anyKey() { 
 if(LX > 200) { return true; } 
 if(LY > 200) { return true; } 
 if(LX < -200) { return true; }  
 if(LY < - 200) { return true; }   
 if(RX > 200) { return true; } 
 if(RY > 200) { return true; } 
 if(RX < -200) { return true; }  
 if(RY < - 200) { return true; }   
 if(leftButton) { return true; }
 if(rightButton) { return true; } 
 return false;
}

void refresh() { 
     display.drawRect(0,0,96,64, 255, x);
     display.setFont(liberationSans_10ptFontInfo);
     display.setCursor(32,24);
     display.print(x);
     display.setCursor(8,50);
     display.print("Selector Mode");
     delay(50);
}

