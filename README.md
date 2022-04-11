# G9Assignment2
Github repo for the C labs for MTRX2700  
###################################################  
Acknowledgement:  
Some parts of the code were developed based on the 2022 Tuesday MTRX2700 weekly lecture example code demonstration.  
###################################################  

# Project Details

The aim of this lab is to understand the theoretical concepts from this course. Assembly language will be used to see how the microcontroller system works. It will focus on performing 4 main aspects: Handling of information in memory, interacting with a 7-segment display, using the serial interface of the 68HCS12, and using the hardware timer. Finally, all the knowledge of the 4 aspects will be integrated to perform some more complex functions.  

# Group member:

Daniel Miu         dmiu7957  
Jiah Pang          jpan7871  
Olivia Chen        yche4284  
Shuangning Wan     swan2159  

## Responsibility Allocation
Task 1 and 2: Daniel
Task 3: Jiah 

# Task Descriptions

## Task 1: Serial with interrupts
### Function Description
This task creates a function in which allows for interfacing to the serial port via the interrupt method rather than the polling method as previously tackled in the ASM lab. To put the entire function simply, we use a terminal simulator (puTTy) to send bytes to the serial I/O registers of the 68HCS12, and it will be outputted back onto the terminal emulator as an indication that the data is recieved.

Now for the high level description of the module:
- We first declare our function and interrupt required
    - serialInit is used to initialise the serial port
    - As we are using a small memory model, we can directly define our interrupt using the method: interrupt 21 void serialISR()
- Define the variables needed
    - a buffer where the inputted string is stored into
    - a stringLength variable to keep track of the length of the string inputted
- Now the main function
    - We call the serialInit() function to initialise the serial port, where we set the baud rate of the serial interface to 9600, setting the 8 bits of the control register 1 to be 00000000 and the 8 bits of the control register 2 to be 00101100
    - EnableInterrupts does what it suggests, enabling the interrupt vectors such that we can create this module using an interrupts method rather than a polling method
    - Then the code enters an infinite loop to prevent any unexpected operations if the line pointer is to leave the main function
- If the interrupt is triggered:
    - We check if the data is received via the RDRF flag (using bitwise and checks if bit 5 is set)
    - Then check if the data sent in is a carriage return character
        - If it is, then we will write back to the serial every character, and the "detected" string after each character, that is inputted up until the carriage return character
        - Otherwise, it will be added to the end of the buffer array and the stringLength variable is incremented
    - If the carriage return is detected and after outputting all the characters back to the user, the buffer is resetted by using the memset function from string.h and resetting the stringLength variable to 0
### User Instructions
Enter a desired string, and press enter at the end of the string, you will then see the string outputted back, character by character, with " detected" at the end of each character

## Task 2: Serial command parser
### Function Description
The task creates the function to take user inputed command from external serial terminal program  and send back unique action. There are 2 unique actions:  
By receiving the command "l",  the system will response as:   
"detected:    
                                                             LED"  
By receiving the command "m",  the system will response as:   
"detected:    
                                                             PERIOD"  
Other invalid commands will have the error message shown: "invalidInput"                                                               

Now for the high level description of the module:   
- We first declare our function and interrupt required
    - serialInit is used to initialise the serial port
    - timerInit is used to initialise the timer
    - As we are using a small memory model, we can directly define our interrupt using the method: interrupt 21 void serialISR() and interrupt 13 void speakerISR()
- Define the variables needed
    - a buffer where the string to be outputted is stored into
    - a stringLength variable to keep track of the length of the string inputted
    - a period variable to toggle upon successful output compare
- Now the main function
    - We call the timerInit() function to initialise the timer, where we enable the timer and fast flag clear, setting the prescaler to 8. Enable the output compare for channel 5 and interrupts for timer 5.
    - We call the serialInit() function to initialise the serial port, where we set the baud rate of the serial interface to 9600, setting the 8 bits of the control register 1 to be 00000000 and the 8 bits of the control register 2 to be 00101100
    - EnableInterrupts does what it suggests, enabling the interrupt vectors such that we can create this module using an interrupts method rather than a polling method
    - We set the Data Direction register for port B to 0xFF;
    - Then the code enters an infinite loop to prevent any unexpected operations if the line pointer is to leave the main function (FEED the dog)
- If the interrupt is triggered:
- 
### User Instructions


## Task 3
### Function Description

### User Instructions
