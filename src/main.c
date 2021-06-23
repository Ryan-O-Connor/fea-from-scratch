/*
 * FEA program:
 * 1. New running model created 
 * 2. Model created and run using script in a text file
 * 3. Main opens file and runs the interpreter
 * 4. When script is finished, file is closed, and program exits
*/


#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "interpreter.h"


int main(int argc, char* argv[]){
  if (argc != 2){
    printf("Input one script file\n");
    exit(1);
  }
  struct model* running_model = new_model();
  FILE* script_file = fopen(argv[1], "r");
  run_script(running_model, script_file);
  fclose(script_file);
  return 0;
}
