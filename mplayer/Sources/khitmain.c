#include <hidef.h>
#include "derivative.h"

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

int period;
unsigned int score[] = {
 D4,B3,G3,B3,D4,G4,B4,A4,G4,B3,C4S,
 D4,ZZ,D4,ZZ,D4,B4,A4,G4,F4S,E4,F4S,G4,ZZ,G4,D4,B3,G3,
 D4,B3,G3,B3,D4,G4,B4,A4,G4,B3,C4S,D4,ZZ,D4,ZZ,D4,
 B4,A4,G4,F4S,E4,F4S,G4,ZZ,G4,D4,B3,G3,B4,ZZ,B4,
 B4,C5,D5,ZZ,D5,C5,B4,A4,B4,C5,ZZ,C5,ZZ,C5,B4,A4,G4,
 F4S,E4,F4S,G4,B3,C4S,D4,ZZ,D4,G4,ZZ,G4,ZZ,G4,F4S,
 E4,ZZ,E4,ZZ,E4,A4,C5,B4,A4,G4,ZZ,G4,F4S,D4,ZZ,D4,
 G4,A4,B4,C5,D5,G4,A4,B4,C5,A4,G4,0};
unsigned int dur[] = {
 30,10,40,40,40,80,30,10,40,40,40,
 80,3,20,3,20,60,20,40,80,20,20,40,3,40,40,40,40,
 30,10,40,40,40,80,30,10,40,40,40,80,3,20,3,20,
 60,20,40,80,20,20,40,3,40,40,40,60,20,3,20,
 40,40,40,3,80,20,20,40,40,40,3,80,3,40,60,20,40,
 80,20,20,40,40,40,80,3,40,40,3,40,3,20,20,
 40,3,40,3,40,40,20,20,20,20,3,40,40,20,3,20,
 60,20,20,20,80,20,20,60,20,40,80};

int main(void)
{
    // Initialise timer stuff

    while(score[j])
    {
        period = score[j];
        delay_xsn(0.01 * dur[j]);
        j++;
    }

    // Disable timers

    for(;;);
}

interrupt 13 void speakerISR()
{
    TC5 = TC5 + period;
}

void delay_xsn(int time){
     
    int s = 366; //duration of 1 second is 366 cycles based on cycle rate and TCNT max value
    
    int d = s * time  ;
    int i;
    
    TSCR1 = 0x80;
    TSCR2 = 0x00;
    
    for (i = 0; i < d; i++){
        
        TFLG2 = 0x80;
        while ( !(TFLG2 & TFLG2_TOF_MASK) );
    
    } 
}