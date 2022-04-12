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
Task 1, 2 + Documentation for task 1, 2: Daniel  
Task 3 + Documentation for task 3: Jiah   
Documentation Layout and Minutes: Olivia   
# Task Descriptions

## Task 1: Serial with interrupts
### Function Description
This task creates a function in which allows for interfacing to the serial port via the interrupt method rather than the polling method as previously tackled in the ASM lab. To put the entire function simply, we use a terminal simulator (puTTy) to send bytes to the serial I/O registers of the 68HCS12, and it will be outputted back onto the terminal emulator as an indication that the data is recieved.

### High level description
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
This task utilises the serial port to interface to the microcontroller, designing a method such that the inputted string from the simulated terminal can be interpretted by the microcontroller as a command. This command, in turn will trigger a certain action on the dragon board, and concurrently print out a message in accordance to the command or an error message if it is an invalid command.

### High level description
- First discuss about the included header files in addition to hidef.h and derivative.h 
    - string.h :
        - memset(): which allows us to clear the buffer of the previous command
        - strlen(char* s): Gives us the length of the string s
        - strtok : allows us to split the string by the space to get the command and the parameter entered
    - stdlib.h :
        - atoi() : to convert the string into a number such that it could be directly outputted by the microcontroller

- Then declare the functions and interrupts we need:
    - serialInit is used to initialise the serial port
    - timerInit is used to initialise the hardware timer
    - invalidInput is used to print out the error message when an invalid command is detected
    - As we are using a small memory model, we can directly define our interrupts using the method: 
        - interrupt 21 void serialISR() for the serial interrupt
        - interrupt 13 void speakerISR() for the PT5 interrupt 
- Defining the variables needed:
    - a buffer where the inputted string is stored into
    - a stringLength variable to keep track of the length of the string inputted
    - a period that is used for the timer period to determine the note played
- Now the main function
    - asm("sei") disables the interrupt to prevent any  of the initialisation process to accidentally trigger an interrupt
    - We call the timerInit() function to initialise the timer in order to use to speaker. Note we only set output compare for channel 5 as only channel 5 is connected to the speaker
    - We call the serialInit() function to initialise the serial port (same initialisation as task 1)
    - EnableInterrupts to reenable the interrupt after it is disabled
    - DDRB sets port B (LEDs and 7 seg) to be output
    - Then the code enters an infinite loop to prevent any unexpected operations if the line pointer is to leave the main function
- Looking at the interrupt 21 void serialISR() function:
    - we first define all the variables we need: char* token for later use in strtok, char* string is to create a pointer to the buffer which stores the input command
    - Similar to task 1, we check if the data is received via the RDRF flag (using bitwise and checks if bit 5 is set)
    - Then check if the data sent in is a carriage return character
        - If it is, then set the first character to lowercase and check it against predefined characters
            - If it is a l, this indicates the LED function. Split the string by the space by using the strtok function from string.h. If the token is null, then there is no parameter entered and hence the error message is outputted. If the parameter isn't a number, the atoi function will set the output to 0 thus all LEDS will be turned off. Otherwise, if the parameter is an integer at most 255, then it'll be outputted to the LEDS in binary form.
            - If it is a m, this indicates the music function. Again split the string by the space by using the strtok function from string.h. If the token is null, then there is no parameter entered and hence the error message is outputted. If the parameter isn't a number, the atoi function will set the output to 0 thus a note will very high frequency is played. Otherwise, any integer value entered will be outputted as a note with the period as the inputted value.
            - If it is neither of the two above functions, then an error message is printed. 
        - Otherwise, it will be added to the end of the buffer array and the stringLength variable is incremented
    - If the carriage return is detected and after outputting all the characters back to the user, the buffer is resetted by using the memset function from string.h and resetting the stringLength variable to 0


### Discussion points
- In the serial interrupt handle, we check the first character for the inputted command. This might cause some issues if we further implement other functions as we run out of characters used to implement, or quite frankly any words starting with the command letter can trigger the command. An improvement to this would be to use the strcmp function from the string.h, thus taking string commands rather using characters to identify the command used
- This can be easily extended to multiple function by adding other else if statements to check for the command 
### User Instructions
- For the LED, enter a 'l' , followed by a space key (' '), then a integer value under 255 and press enter to send through the command. You should see the corresponding binary value show on the port B LEDs.
- For the note player function, enter a 'm', followed by a space key (' '), then an integer value and press enter to send through the command. You should hear a note played from the dragon board.

## Task 3
### Function Description

#### Included headers
- string.h included for use of strlen in the use of the parsed string  
#### Functions and interrupts
- as mentioned in previous parts, the serial and timer interrupts are set up and additionally the timer interrupts have been implemented for frequency generation.
- we also have to initialise the buzzer which is attached to PT5 and requires interrupts as well. This is changed through the use of TC5
- delay_xsn()
    - uses the timer overflow property of the timer module to perform a delay for a calculated duration. seeing that the TCNT max val is 65, 1 cycle can be calculated and then multiplied to create a variable delay function to play notes over that duration etc. 
- songPlayStr()
    - this function intends to play a song based on a string passed to it. the function breaks down the string into individual characters, and then calculates the value of the first 3 bits for duration and the last five for pitch/note. it then updates the overall time to play value and then plays the notes until it reaches the end of the string. this is where the string.h header is used.
#### variables
- str_song[]
    - this variable in the code is like a global string which is used for testing the module. when integrated, this variable would not be used as you could pass in the string from serial into the songPlayStr function. 
- current_song_time
    - this integer tracks the current song time of the music module. The songPlayStr function updates this time as it goes, and then decreases this count after the duration of each note. this is also essentially the ready to play next song flag, as unless this value is zero, there can be no next song played. 

### Discussion Points
- The first major discussion point is the integration. Due to some circumstances out of our control, we were unable to actually integrate the code with the part 2 code. this ment that the serial parsing and functionality around receiving commands for part 3 were not fully completed, but had been addressed more in part 2. The flags and other things (timer count, interrupts) along with the function have been written in mind ready for integration, but unfortunately did not come along
- What happens if a new tune is requested before the previous one finishes?
    - as mentioned above, the commands are not being processed by this code itself, but the logic behind it was to use the flag (current_song_time) variable, and if the time is greater than zero, return the time to the serial output and then continue playing the song. the delay using timer overflow should ensure the song kept playing. This count for song time is changed only by the songPlayStr function, and updates throughout the song to show the remaining time of the song.
- What happens if there are unexpected characters or parsing errors from the tune string.
    - currently there is not a lot of unexpected characters that could not be handled, as the function processes all ascii characters given they are valid ascii characters. Given more time, a type check would be handy to ensure characters are within the given ascii range and not unicode range, however due to it being serial input this shold not be an issue. 

### User Instructions
- To play a song, edit the str_song string at the top of the code, and then run to see the song played on the dragon board. 
