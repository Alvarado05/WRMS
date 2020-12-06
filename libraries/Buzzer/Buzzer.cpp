#include <Buzzer.h>

Buzzer::Buzzer(byte buzzerPin) {
    pinMode(buzzerPin, OUTPUT);
    _buzzerPin = buzzerPin;
}

void Buzzer::arrived(){
  tone(_buzzerPin, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(_buzzerPin);     // Stop sound...
  delay(1000);        // ...for 1 sec
  tone(_buzzerPin, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1sec
  noTone(_buzzerPin);     // Stop sound...
  delay(1000);        // ...for 1 sec
  tone(_buzzerPin, 1000); // Send 1KHz sound signal...
  delay(2000);        // ...for 1sec
  noTone(_buzzerPin);     // Stop sound...
}

void Buzzer::alert(){
  tone(_buzzerPin, 2000); // Send 1KHz sound signal...
  delay(400);        // ...for .4 sec
  noTone(_buzzerPin);     // Stop sound...
  delay(1000);        // ...for 1sec
}

void Buzzer::error(){
  tone(_buzzerPin, 300); // Send 300Hz sound signal...
  delay(500);        // ...for .5 sec
  tone(_buzzerPin, 200); // Send 200Hz sound signal...
  delay(1000);        // ...for 1sec
  noTone(_buzzerPin);     // Stop sound...
}

void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{ 
  int ledPin = 24;
    digitalWrite(ledPin, HIGH);  
    //use led to visualize the notes being played
    
    int x;   
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
    for (x=0;x<loopTime;x++)   
    {    
        digitalWrite(speakerPin,HIGH);
        delayMicroseconds(delayAmount);
        digitalWrite(speakerPin,LOW);
        delayMicroseconds(delayAmount);
    }    
    
    digitalWrite(ledPin, LOW);
    //set led back to low
    
    delay(20);
    //a little delay to make all notes sound separate
} 

void Buzzer::imperialMarch(){

#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

  beep(_buzzerPin, a, 500); 
  beep(_buzzerPin, a, 500);     
  beep(_buzzerPin, a, 500); 
  beep(_buzzerPin, f, 350); 
  beep(_buzzerPin, cH, 150);
  
  beep(_buzzerPin, a, 500);
  beep(_buzzerPin, f, 350);
  beep(_buzzerPin, cH, 150);
  beep(_buzzerPin, a, 1000);
  //first bit
  
  beep(_buzzerPin, eH, 500);
  beep(_buzzerPin, eH, 500);
  beep(_buzzerPin, eH, 500);    
  beep(_buzzerPin, fH, 350); 
  beep(_buzzerPin, cH, 150);
  
  beep(_buzzerPin, gS, 500);
  beep(_buzzerPin, f, 350);
  beep(_buzzerPin, cH, 150);
  beep(_buzzerPin, a, 1000);
  //second bit...
  
  beep(_buzzerPin, aH, 500);
  beep(_buzzerPin, a, 350); 
  beep(_buzzerPin, a, 150);
  beep(_buzzerPin, aH, 500);
  beep(_buzzerPin, gSH, 250); 
  beep(_buzzerPin, gH, 250);
  
  beep(_buzzerPin, fSH, 125);
  beep(_buzzerPin, fH, 125);    
  beep(_buzzerPin, fSH, 250);
  delay(250);
  beep(_buzzerPin, aS, 250);    
  beep(_buzzerPin, dSH, 500);  
  beep(_buzzerPin, dH, 250);  
  beep(_buzzerPin, cSH, 250);  
  //start of the interesting bit
  
  beep(_buzzerPin, cH, 125);  
  beep(_buzzerPin, b, 125);  
  beep(_buzzerPin, cH, 250);      
  delay(250);
  beep(_buzzerPin, f, 125);  
  beep(_buzzerPin, gS, 500);  
  beep(_buzzerPin, f, 375);  
  beep(_buzzerPin, a, 125); 
  
  beep(_buzzerPin, cH, 500); 
  beep(_buzzerPin, a, 375);  
  beep(_buzzerPin, cH, 125); 
  beep(_buzzerPin, eH, 1000); 
  //more interesting stuff (this doesn't quite get it right somehow)
  
  beep(_buzzerPin, aH, 500);
  beep(_buzzerPin, a, 350); 
  beep(_buzzerPin, a, 150);
  beep(_buzzerPin, aH, 500);
  beep(_buzzerPin, gSH, 250); 
  beep(_buzzerPin, gH, 250);
  
  beep(_buzzerPin, fSH, 125);
  beep(_buzzerPin, fH, 125);    
  beep(_buzzerPin, fSH, 250);
  delay(250);
  beep(_buzzerPin, aS, 250);    
  beep(_buzzerPin, dSH, 500);  
  beep(_buzzerPin, dH, 250);  
  beep(_buzzerPin, cSH, 250);  
  //repeat... repeat
  
  beep(_buzzerPin, cH, 125);  
  beep(_buzzerPin, b, 125);  
  beep(_buzzerPin, cH, 250);      
  delay(250);
  beep(_buzzerPin, f, 250);  
  beep(_buzzerPin, gS, 500);  
  beep(_buzzerPin, f, 375);  
  beep(_buzzerPin, cH, 125); 
         
  beep(_buzzerPin, a, 500);            
  beep(_buzzerPin, f, 375);            
  beep(_buzzerPin, c, 125);            
  beep(_buzzerPin, a, 1000);
}
