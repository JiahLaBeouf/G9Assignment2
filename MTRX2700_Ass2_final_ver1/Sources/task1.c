#include "task1.h"

char* buffer1;
int stringLength1;


void task1main() 
{
  buffer1 = malloc( 500* sizeof(char));
  serialInit1();     // Initialising serial port
  EnableInterrupts; // Enabling Interrupts
  
  for(;;){
    _FEED_COP(); 
  }
}

void serialInit1(void) 
{
  
  // Set baud rate to 9600
  SCI1BDL = 0x9C;
  SCI1BDH = 0;
  
  // No fancy stuff needed
  SCI1CR1 = 0;
  
  // 2C = 00101100, Enable receive interrupt, transmit, receive
  SCI1CR2 = 0x2C;
}

interrupt 21 void serialISR1() 
{
  int i;
  int k = 0;
  char outputString[12] = " detected\r\n";
  
  // Check if data is received. The RDRF flag
  if (SCI1SR1 & 0x20) 
  {
    // End of sentence? Look for a carriage return
    if (SCI1DRL == 0x0D) 
    {
      // Don't do anything unless you are ready to send data. The TDRE flag
      // May not need this line since I do it again below
      while(!(SCI1SR1 & 0x80));
      
      // Go through all characters in buffer
      for (k = 0; k < stringLength1; k++) 
      {
        // Wait for data to be ready
        while(!(SCI1SR1 & 0x80));
        
        // Write to serial
        SCI1DRL = buffer1[k];
        
        // Write the rest of the appended string
        for (i = 0; i < 12; i++) 
        {
          while(!(SCI1SR1 & 0x80));
          SCI1DRL = outputString[i];
        }
      }
      
      // Reset buffer
      memset(buffer1, '\0' , sizeof(buffer1));
      stringLength1 = 0;
    } 
    
    // Store each character of sentence in buffer
    else
    {
      buffer1[stringLength1] = SCI1DRL;
      stringLength1 = stringLength1 + 1;
    }
  }
}