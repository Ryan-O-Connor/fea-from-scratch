/*
Publically available parser and interpreter functions
*/

struct instruction{
  char* command;
  int argc;
  char** argv;
};

int run_script(struct model* running_model, FILE* script_file);
int execute(struct model* running_model, struct instruction* next_instruction);
