#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define G3 7653
#define B3 6074
#define C4 5733
#define C4S 5412
#define D4 5108
#define E4 4551
#define F4 4295
#define F4S 4054
#define G4 3827
#define A4 3409
#define B4F 3218
#define B4 3037
#define C5 2867
#define D5 2554
#define E5 2275
#define F5 2148
#define ZZ 20       // delay count to create an inaudible sound
#define toggle 0x04 // value to toggle OC5 pin

void delayby10ms(unsigned int time);
interrupt 13 void speakerISR();

int period;
int j =1;

unsigned int score[500] = {
 D4,B3,G3,B3,D4,G4,B4,A4,G4,B3,C4S,
 D4,ZZ,D4,ZZ,D4,B4,A4,G4,F4S,E4,F4S,G4,ZZ,G4,D4,B3,G3,
 D4,B3,G3,B3,D4,G4,B4,A4,G4,B3,C4S,D4,ZZ,D4,ZZ,D4,
 B4,A4,G4,F4S,E4,F4S,G4,ZZ,G4,D4,B3,G3,B4,ZZ,B4,
 B4,C5,D5,ZZ,D5,C5,B4,A4,B4,C5,ZZ,C5,ZZ,C5,B4,A4,G4,
 F4S,E4,F4S,G4,B3,C4S,D4,ZZ,D4,G4,ZZ,G4,ZZ,G4,F4S,
 E4,ZZ,E4,ZZ,E4,A4,C5,B4,A4,G4,ZZ,G4,F4S,D4,ZZ,D4,
 G4,A4,B4,C5,D5,G4,A4,B4,C5,A4,G4,0};
unsigned int dur[500] = {
 30,10,40,40,40,80,30,10,40,40,40,
 80,3,20,3,20,60,20,40,80,20,20,40,3,40,40,40,40,
 30,10,40,40,40,80,30,10,40,40,40,80,3,20,3,20,
 60,20,40,80,20,20,40,3,40,40,40,60,20,3,20,
 40,40,40,3,80,20,20,40,40,40,3,80,3,40,60,20,40,
 80,20,20,40,40,40,80,3,40,40,3,40,3,20,20,
 40,3,40,3,40,40,20,20,20,20,3,40,40,20,3,20,
 60,20,20,20,80,20,20,60,20,40,80};

void main(void)
{
	// Initialise timer stuff

     // Disable all interrupts
	asm("sei");
	
	// Enable timer and fast flag clear
	TSCR1 = 0x90; 
	
	// Set prescaler to 8
	TSCR2 = 0x03;
	
	// Choose output compare for channel 5
	TIOS = 0x20;
	
	// Toggle upon successful output compare 
	TCTL1 = 0x04; 
	
	// Enable interrupts for timer 5
	TIE = 0x20;
	DDRB = 0xFF;
	
	period = score[0];
	TC5 = TC5 + period; 
	EnableInterrupts;



	// Disable timers

	for(;;){
	    
	    while(score[j])
    	{
    		period = score[j];
    		PORTB = period;
    		delayby10ms(dur[j]);
    		j++;
    	}
	}
}

void delayby10ms(unsigned int time){
    int i;
    int k;
    for(i=0;i<time;i++)
      for(k=0;k<400;k++);
}

interrupt 13 void speakerISR()
{
	TC5 = TC5 + period;
}