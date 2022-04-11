#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <string.h>
#include <stdlib.h>

void task1main();
void serialInit1(void);
interrupt 21 void serialISR1();