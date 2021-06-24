/*
Receives instructions from parser and calls corredponding model functions
Unpacks and formats arguments to correct types
Checks that commands and arguments are valid
*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lib/strfuncs.h"
#include "model.h"
#include "interpreter.h"


/***********************************************
 * Error printout functions
 */


void print_argc_error(char* command, int expected_argc, int got_argc){
  printf("Invalid number of arguments for %s.  Expected %d, got %d\n",
	 command, expected_argc, got_argc);
}


void print_script_error(char* err, int line){
  printf("%s error.  Aborting script on line %d\n", err, line);
}


/***********************************************
 * Parsing functions
 */


#define MAXBUFFER 1000
#define MAXFIELDS 10
static const char* delimiters = " ,\t\n";


static int parse(char* line, struct instruction* next_instruction){
  // Reset instruction argument counter
  next_instruction->argc = 0;
  // Must be at least one token indicating the command
  char* field = strtok(line, delimiters);
  if (field != NULL){
    next_instruction->command = field;
  }
  else{
    printf("Error: No command could be parsed\n");
    return 1;
  }
  // Collect all remaining arguments, if any
  while ((field = strtok(NULL, delimiters)) != NULL){
    if (next_instruction->argc < MAXFIELDS){
      next_instruction->argv[next_instruction->argc++] = field;
    }
    else{
      printf("Error: Too many fields\n");
      return 1;
    }
  }
  return 0;
}


static int whitespace_line(char* line){
  // Determines whether a line is entirely whitespace
  char c;
  int i = 0;
  while ((c = line[i++]) != '\0'){
    if (c != '\n' && c != '\t' && c != ' '){
      return 0;
    }
  }
  return 1;
}


static int comment_line(char* line){
  // Determines whether a line is a comment
  if (line[0] == '!'){
    return 1;
  }
  else{
    return 0;
  }
}


int run_script(struct model* running_model, FILE* script_file){
  // Only one instruction object is created.
  // Each instruction overwrites the last.
  char buffer[MAXBUFFER];
  char* fields[MAXFIELDS];
  struct instruction next_instruction;
  next_instruction.argc = 0;
  next_instruction.argv = fields;

  int exec_status, parse_status;
  int line_number = 0;

  // Attempt to parse and execute each non-whitespace and non-comment line
  while(fgets(buffer, MAXBUFFER, script_file) != NULL){
    line_number++;
    if (!whitespace_line(buffer) && !comment_line(buffer)){
      printf("%s", buffer);
      parse_status = parse(buffer, &next_instruction);
      if (parse_status != 0){
	print_script_error("Parsing", line_number);
      	return 1;
      }
      exec_status = execute(running_model, &next_instruction);
      if (exec_status != 0){
      	print_script_error("Execution", line_number);
      	return 1;
      }
    }
  }
  return 0;
}


/****************************************************
 * Interpreting functions
 */


static int exec_new_node(struct model* running_model,
			  int argc, char* argv[]){
  // X Coor, Y Coor
  double x = atof(argv[0]);
  double y = atof(argv[1]);
  new_model_node(running_model, x, y);
  return 0;
}


static int exec_new_element(struct model* running_model,
			     int argc, char* argv[]){
  int et_id = atoi(argv[0]);
  int* IEN = malloc((argc-1)*sizeof(int));
  int i;
  for (i=1; i<argc; i++)
    IEN[i-1] = atoi(argv[i]);
  new_model_element(running_model, et_id, IEN);
  return 0;
}


static int exec_new_element_type(struct model* running_model,
				 int argc, char* argv[]){
  assert(argc == 2);
  int et_id = atoi(argv[0]);
  strtoupper(argv[1]);
  char* type_name = argv[1]; 
  new_model_element_type(running_model, et_id, type_name);
  return 0;
}


static int exec_set_keyopt(struct model* running_model,
				 int argc, char* argv[]){
  assert(argc == 3);
  int et_id = atoi(argv[0]);
  int key = atoi(argv[1]);
  int option = atoi(argv[2]);
  set_model_et_keyopt(running_model, et_id, key, option);
  return 0;
}


static int exec_set_real_constant(struct model* running_model,
				 int argc, char* argv[]){
  assert(argc == 3);
  int et_id = atoi(argv[0]);
  int const_id = atoi(argv[1]);
  double value = atof(argv[2]);
  set_model_et_real_constant(running_model, et_id, const_id, value);
  return 0;
}


static int exec_set_matprop(struct model* running_model,
			    int argc, char* argv[]){
  assert(argc == 3);
  int et_id = atoi(argv[0]);
  strtoupper(argv[1]);
  char* prop_name = argv[1];
  double value = atof(argv[2]);
  set_model_et_matprop(running_model, et_id, prop_name, value);
  return 0;
}


static int exec_add_essential_bc(struct model* running_model,
				 int argc, char* argv[]){
  assert(argc == 3);
  int node_id = atoi(argv[0]);
  strtoupper(argv[1]);
  char* comp = argv[1];
  double value = atof(argv[2]);
  add_model_essential_bc(running_model, node_id, comp, value);
  return 0;
}


static int exec_add_nodal_force(struct model* running_model,
				int argc, char* argv[]){
  assert(argc == 3);
  int node_id = atoi(argv[0]);
  strtoupper(argv[1]);
  char* comp = argv[1];
  double value = atof(argv[2]);
  add_model_nodal_force(running_model, node_id, comp, value);
  return 0;
}


static int exec_model_solve(struct model* running_model,
			     int argc, char* argv[]){
  assert(argc == 2);
  int p_type = atoi(argv[0]); // Physics type
  int s_type = atoi(argv[1]); // Solver type
  solve_model(running_model, p_type, s_type);
  return 0;
}


static int exec_print_nodal_soln(struct model* running_model,
				 int argc, char* argv[]){
  assert(argc == 1);
  strtoupper(argv[0]);
  char* res_name = argv[0];
  print_model_result(running_model, res_name);
  return 0;
}


static int exec_print_mesh(struct model* running_model){
  print_model_mesh(running_model);
  return 0;
}


static int exec_free_model(struct model* running_model){
  free_model(running_model);
  return 0;
}


int execute(struct model* running_model, struct instruction* next_instruction){
  /* Executes instructions supplied by the parser
   Returns 0 for successful execution
   Returns 1 for unsuccessful execution */
  
  int return_code = 0;
  strtoupper(next_instruction->command);
  char* command_code = next_instruction->command;
  int argc = next_instruction->argc;
  char** argv = next_instruction->argv;

  if (strcmp("N", command_code) == 0)
    return exec_new_node(running_model, argc, argv);
  
  else if (strcmp("E", command_code) == 0)
    return exec_new_element(running_model, argc, argv);
  
  else if (strcmp("ET", command_code) == 0)
    return exec_new_element_type(running_model, argc, argv);
  
  else if (strcmp("KEYOPT", command_code) == 0)
    return exec_set_keyopt(running_model, argc, argv);
  
  else if (strcmp("R", command_code) == 0)
    return exec_set_real_constant(running_model, argc, argv);
  
  else if (strcmp("MP", command_code) == 0)
    return exec_set_matprop(running_model, argc, argv);
  
  else if (strcmp("D", command_code) == 0)
    return exec_add_essential_bc(running_model, argc, argv);
  
  else if (strcmp("F", command_code) == 0)
    return exec_add_nodal_force(running_model, argc, argv);
  
  else if (strcmp("SOLVE", command_code) == 0)
    return exec_model_solve(running_model, argc, argv);
  
  else if (strcmp("PRNSOL", command_code) == 0)
    return exec_print_nodal_soln(running_model, argc, argv);
  
  else if (strcmp("PRMESH", command_code) == 0)
    return exec_print_mesh(running_model);
  
  else if (strcmp("FINISH", command_code) == 0)
    return exec_free_model(running_model);
  
  else{
    printf("Error: Invalid command: %s\n", command_code);
    return 1;
  }
}


