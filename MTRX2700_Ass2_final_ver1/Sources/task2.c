#include "task2.h"

char* buffer;
int stringLength;

int period = 0;

void task2main() 
{
  asm("sei");
  
  
  timerInit();
  serialInit();     // Initialising serial port
  EnableInterrupts; // Enabling Interrupts
  
  DDRB = 0xFF;
  buffer = malloc(500*sizeof(char));
  
  for(;;){
    _FEED_COP(); 
  }
}

void serialInit(void) 
{
  // Set baud rate to 9600
  SCI1BDL = 0x9C;
  SCI1BDH = 0;
  
  // No fancy stuff needed
  SCI1CR1 = 0;
  
  // 2C = 0010110, Enable receive interrupt, transmit, receive
  SCI1CR2 = 0x2C;
}


void timerInit(void){
  // Enable timer and fast flag clear
	TSCR1 = 0x90; 
	
	// Set prescaler to 8
	TSCR2 = 0x03;
	
	// Choose output compare for channel 5
	TIOS = 0x20;
	
	// Toggle upon successful output compare 
	TCTL1 = 0x04; 
	
	period = 100;
	
	// Enable interrupts for timer 5
	TIE = 0x20;  
}

void invalidInput(){
    
    int i;
    char output[16] = " invalidInput\n\r";
    
     for (i = 0; i < 15; i++) 
        {
          while(!(SCI1SR1 & 0x80));
          SCI1DRL = output[i];
        } 
     
       
}




interrupt 21 void serialISR() 
{
  int i;
  char* token;
  char* string;
  // Check if data is received, ie The RDRF flag
  if (SCI1SR1 & 0x20) 
  {
    // Look for a carriage return
    if (SCI1DRL == 0x0D) 
    {   
        
        // Don't do anything unless you are ready to send data. The TDRE flag
        while(!(SCI1SR1 & 0x80));
        buffer[0] = tolower(buffer[0]);   
        if( buffer[0] == 'l'){
           
           string = buffer;
           token = strtok(string, " ");
           token = strtok(NULL, " ");
           token[strlen(token)] = ':';
           token[strlen(token)] = 'L';
           token[strlen(token)] = 'E';
           token[strlen(token)] = 'D';
            
           token[strlen(token)] = '\n';
           token[strlen(token)] = '\r';
            
            if(token == NULL){
               invalidInput();
            } else{                                     
                for(i = 0; i!= strlen(token);i++){
                  while(!(SCI1SR1 & 0x80));
                  SCI1DRL = token[i];
                }
                
                PORTB = atoi(token);
            }
        }else if( buffer[0] == 'm'){
           string = buffer;
           token = strtok( string, " ");
           token = strtok(NULL, " ");
           token[strlen(token)] = ':';
           token[strlen(token)] = 'P';
           token[strlen(token)] = 'E';
           token[strlen(token)] = 'R';
           token[strlen(token)] = 'I';
           token[strlen(token)] = 'O';
           token[strlen(token)] = 'D';
           token[strlen(token)] = '\n';
           token[strlen(token)] = '\r';
           
           
           
           if(token == NULL){
               invalidInput();
            } else{
                for(i = 0; i!= strlen(token);i++){
                  while(!(SCI1SR1 & 0x80));
                  SCI1DRL = token[i];
                }                     
               period = atoi(token);
            }
        }
        else{
            invalidInput();
        }
        // Reset buffer
        memset(buffer, '\0' , sizeof(buffer));
        stringLength = 0;

     }
    
    // Store each character of sentence in buffer
    else
    {
      buffer[stringLength] = SCI1DRL;
      stringLength = stringLength + 1;
    }
   
  }
}

interrupt 13 void speakerISR() 
{
  TC5 = TC5 + period;
}