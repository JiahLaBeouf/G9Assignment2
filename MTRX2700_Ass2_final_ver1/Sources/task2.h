#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <string.h>
#include <stdlib.h>

void task2main();
void serialInit(void);
void timerInit(void);
void invalidInput();
interrupt 21 void serialISR();
interrupt 13 void speakerISR();