#include <hidef.h> /* common defines and macros */
#include <string.h>
#include <math.h>
#include "derivative.h" /* derivative-specific definitions */                                         

char str_song[] = "1a`=P3*x/r";


// Global Value to all changing notes
int musicNote = 0;
unsigned char readValueSC;
unsigned char overflow = 0;

int current_song_time = 0;
 
void songPlayStr(char *); 
// Interrupt for Output Compare            s
interrupt(((0x10000 - Vtimch5)/2)-1) void TC5_ISR(void);

// Interrupt for Serial Communication
interrupt(((0x10000 - Vsci0)/2)-1) void SCI0_ISR(void);

// Interrupt for Timer Overflow 
interrupt(((0x10000 - Vtimovf)/2)-1) void TOF_ISR(void);


void SCI0_INIT(void){
    
    SCI0BDH = 0x00;  // Must be assigned
    SCI0BDL = 26;    // Set 9600 baud rate to new boards
    SCI0CR1 = 0x00;  // 8 Bit, no parity
    SCI0CR2 = 0xAC;  // Enable trans, recevive (0x04), send(0x08), receive+send (0x0C) 
                     // And Interrupt
}


// Interrupt Service Routine to Output Compare 
interrupt(((0x10000 - Vtimch5)/2)-1) void TC5_ISR(void){
 
 TC5   = TC5 + musicNote;             // Update to TC5 
 TFLG1 = 0x20;                        // Clear C5F

}

// Interrupt Service Routine to Serial Communication
interrupt (((0x10000-Vsci0)/2)-1) void SCI0_ISR(void){
  
   if(SCI0SR1_RDRF_MASK & SCI0SR1)   // Wait to Receive
     readValueSC = SCI0DRL;          // Save Value
}



// Interrupt Service Routine to Timer Overflow 
interrupt(((0x10000 - Vtimovf)/2)-1) void TOF_ISR(void) {
  overflow++;                       // Increment Count
  TFLG2 = TFLG2 | TFLG2_TOF_MASK;   // Clear Flag
}


//
// TCNT max value is 65.536
//
// So 1 cyle is equal to 65.536 / 24mHz 
//
// 1 cyle = 65.536/24.000.000 = 2.73 ms
//

// Timer Counter to 1 sn
// 1s delay implemented by using Timer Overflow Property of Timer Module.

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



//////////////////////////// Main Function //////////////////////////////////////////////

void main(void) 
{
    
    
    //int i;              // Counter to song number
    
    __asm(sei);         //  Disable Interrupts Globally 
    DDRB = 0xFF;        //  PORTB Output
    DDRH = 0x00;        //  PORTH Input
    DDRP = 0xFF;        //  PORTP Output to Seven Segment Display
    PTP  = 0x07;        //  Write number of the song on 7-segment LEDs.
      
  
    TSCR1 = 0x80;       // Timer Enable
    TIOS  = 0x20;       // Select Channel 5 for output compare
    TCTL1 = 0x04;       // Toggle PT5 pin upon match
    TIE   = 0x20;       // Enable Interrupt for Channel 5
    TFLG1 = 0x20;       // Clear C5F
    __asm(cli);          // Enable Interrupts Globally
   

    songPlayStr(str_song);
  
} 


/*
 * playing from the provided string
 * logic is the first 5 bits of each char are for pitch, and last 3 are for duration (s) 
 * 
 *
 *
 *
 */
void songPlayStr(char * string){

    int songStrLen = strlen(string);
    int songLen = 0;

    int i = 0;

    //updating the song time for each of the 
    for (i = 0; i < songStrLen; i++){
        int noteVal = string[i];
        //printf("noteval: %d %c\n",noteVal,noteVal);
        int j;
        for (j = 0; j <= 2; j++){
            int nvMod;
            nvMod = noteVal%2;
            //printf(" nvmod : %d");
            if (nvMod == 1){
                int count = 0;
                int x = 1;
                while (count < j){
                    x = x * 2;
                    count++;
                }
                songLen += x;
            }
            noteVal/=2;
        }
        
    }
    current_song_time = songLen;

    //calculate the note value and playthe note
    for (i = 0; i < songStrLen; i++){
        int noteVal = string[i];
        //printf("noteval: %d %c\n",noteVal,noteVal);
        int j;
        int noteLen;
        for (j = 0; j <= 2; j++){
            int nvMod;
            nvMod = noteVal%2;
            //printf(" nvmod : %d");
            if (nvMod == 1){
                int count = 0;
                int x = 1;
                while (count < j){
                    x = x * 2;
                    count++;
                }
                noteLen += x;
            }
            noteVal/=2;
        }
        noteVal = string[i] - noteLen;
        if (noteVal >= 100){
            noteVal*=100;
        }else{
            noteVal *= 1000;
        }
        //play note
        musicNote = noteVal;
        PORTB = noteVal;
        PORTB = 0x06;
        //shows playing new song
        delay_xsn(noteLen);
        //remove the length from the total song length
        current_song_time-= noteLen;

        
    }
    //set tone to off/rest etc
    musicNote = 100;
    PORTB = 0x5B; //ready for new song


}
