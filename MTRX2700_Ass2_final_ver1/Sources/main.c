#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "task1.h"
#include "task2.h"



void main(void) {
  int taskSelector = 1;
  
  if(taskSelector == 1){
      task1main();
  } else if(taskSelector == 2){
      task2main();
  }
}
