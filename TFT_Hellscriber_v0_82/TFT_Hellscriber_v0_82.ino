// Seven Line Hellscriber Painter
// v0.82

#include <stdint.h>
#include <TFT.h>

int analogPin = 5; //  The audio input pin
int analogPot1 = 4; // Lower Limit Pot pin
int analogPot2 = 3; // Upper Limit Pot pin


int x, y, x2; // plotting varables
int sig = 0; // Audio signal varable 
int pot1, pot2; // Analog pot values 
int greendot, dot; // The Hellscrieber dot varables
int lower, upper; // Contrast mapped pot values




void setup()
{

Tft.init();  //init TFT library
Tft.setDisplayDirect(UP2DOWN);
Tft.drawString("FH v0.82",10,10,1,WHITE);
}

void loop()
{
 // Each Line is 28 pixels high
  for (int y=0; y<320; y++)
  {
  for (int x=214-x2; x<228-x2; x++)
  {
    sig=analogRead(analogPin); // signal input
    pot1=analogRead(analogPot1); //get lower limit
    pot2=analogRead(analogPot2); //get upper limit
    
    lower=map(pot1,0,1023,0,511); // map pot1 to lower range
    upper=map(pot2,0,1023,512,1023); // map pot2 to upper range
    
    dot=map(sig,lower,upper,0,63); // Signal mapped into 5 bits with a lower & upper limit for contrast
    
    greendot= dot << 5; // shift the dot value into the green range of the display RGB 565
   
    Tft.setPixel(x, y, greendot); // plot green dot
    Tft.setPixel(x+13 ,y ,greendot); // plot sample green dot again shifted by 14 pixels
    
    delayMicroseconds(3480L); // master time delay for Hellscrieber

  }
  }
  x2=x2+33; // paint line offset with boundary to seperate each line with black space
  if(x2>210) x2=0; // if seventh line, start at the top again
}
