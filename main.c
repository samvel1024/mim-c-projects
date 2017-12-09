#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//#define MODE_DEBUG
#ifdef MODE_DEBUG
#define log printf
#define DECORATE_STDOUT true
#else
#define log fake_printf
#define DECORATE_STDOUT false
#endif

#define UNUSED(x) (void)(x)

#define BUF_SIZE 50
#define PROGRAM_LEN_MAX 3001
#define ADDR_MAX 6000
#define STACK_MAX 5000

void fake_printf(const char *format, ...) {
  UNUSED(format);
}

typedef enum {
  WRITE = 100,
  READ,
  SUBTRACT,
  CALL_LABEL,
  CONDITION,
  RETURN,
} CommandType;

const char *COMMAND_NAMES[] = {"WRITE", "READ", "SUBTRACT", "CALL_LABEL", "CONDITION", "RETURN"};

typedef struct command_t {
  char label[BUF_SIZE];
  char arg1[BUF_SIZE];
  char arg2[BUF_SIZE];
  bool is_labeled;
  CommandType type;
} Command;


void Command_log(Command *this) {
  const char *name = COMMAND_NAMES[this->type - WRITE];
  log("%s(%s%s%s) %s %s\n", name,
      this->arg1, strlen(this->arg2) == 0 ? "" : ",",
      this->arg2,
      this->is_labeled ? "DEFINE" : "",
      this->is_labeled ? this->label : "");
}

typedef struct parser_t {
  char buff;
  bool has_next;
} Parser;

bool Parser_read(Parser *self) {
  if (!self->has_next) return false;
  char c = (char) getchar();
  self->buff = c;
  self->has_next = c != EOF && c != '&';
  return self->has_next;
}


bool Parser_is_whitespace(char c) {
  return isspace(c) || c == '|';
}

bool Parser_not_whitespace(char c) {
  return !isspace(c);
}

bool Parser_is_number(char c) {
  return isdigit(c) || c == '-' || c == '+';
}


void Parser_skip(Parser *self) {
  while (self->has_next && Parser_is_whitespace(self->buff) && Parser_read(self));
}

Parser* Parser_new(){
  Parser *self = malloc(sizeof(Parser));
  self -> has_next = true;
  Parser_read(self);
  Parser_skip(self);
  return self;
}


void Parser_read_word(Parser *self, char *buff, bool (*filter)(char)) {
  int i = 0;
  while (filter(self->buff) && self->has_next) {
    buff[i++] = self->buff;
    Parser_read(self);
  }
}

void Parser_read_label(Parser *self, Command *c) {
  if (self->buff != ':')
    return;
  Parser_read(self);
  Parser_skip(self);
  Parser_read_word(self, c->label, Parser_not_whitespace);
  Parser_skip(self);
  c->is_labeled = true;
}

Command Parser_next_command(Parser *self) {
  Command c = {.is_labeled = false, .arg1 = "", .arg2 = "", .type = 0, .label = ""};
  Parser_read_label(self, &c);
  if (self->buff == ';') {
    c.type = RETURN;
    Parser_read(self);
  }
  else if (self->buff == '^') {
    Parser_read(self);
    Parser_skip(self);
    Parser_read_word(self, c.arg1, Parser_is_number);
    c.type = READ;
  } else if (Parser_is_number(self->buff)) {
    Parser_read_word(self, c.arg1, Parser_is_number);
    Parser_skip(self);
    if (self -> buff == '^'){
      Parser_read(self);
      c.type = WRITE;
    }else if (Parser_is_number(self -> buff)){
      Parser_read_word(self, c.arg2, Parser_is_number);
      c.type = SUBTRACT;
    }
    else {
      Parser_read_word(self, c.arg2, Parser_not_whitespace);
      c.type = CONDITION;
    }
  }else {
    Parser_read_word(self, c.arg1, Parser_not_whitespace);
    c.type = CALL_LABEL;
  }
  Parser_skip(self);
  return c;

}


typedef struct vm_t {
  Command program[PROGRAM_LEN_MAX];
  int program_len;
  int memory[2 * ADDR_MAX];
  int stack[STACK_MAX];
  int stack_size;
} VM;

VM *VM_new() {
  VM *vm = malloc(sizeof(VM));
  vm->program_len = 0;
  vm->stack_size = 0;
  for (int i = -ADDR_MAX; i < ADDR_MAX; ++i) {
    vm->memory[i + ADDR_MAX] = -1 - i;
  }
  return vm;
}

void VM_write(VM *this, int addr, int val) {
  this->memory[addr + ADDR_MAX] = val;
  log("VM: Write addr %d val %d\n", addr, this->memory[addr + ADDR_MAX]);
}

int VM_read(VM *this, int addr) {
  if (addr < -ADDR_MAX || addr > ADDR_MAX)
    return -1 - addr;
  log("VM: Read  addr %d val %d\n", addr, this->memory[addr + ADDR_MAX]);
  return this->memory[addr + ADDR_MAX];
}

int VM_find_labeled(VM *this, char *label) {
  for (int i = 0; i < this->program_len; ++i) {
    if (strcmp(label, this->program[i].label) == 0) {
      return i;
    }
  }
  return -1;
}

void VM_pop_stack(VM *self) {
  (self->stack_size)--;
}

int VM_peek_stack(VM *self) {
  return self->stack[self->stack_size - 1];
}

void VM_push_stack(VM *self, int ptr) {

  self->stack[self->stack_size] = ptr;
  self->stack_size = self->stack_size + 1;
  if (self->stack_size > STACK_MAX) {
    log("VM_ERR: STACKOVERFLOW\n");
    exit(1);
  }
}

void VM_set_peek(VM *self, int val) {
  self->stack[self->stack_size - 1] = val;
}

void VM_increment_peek(VM *self) {
  (self->stack[self->stack_size - 1])++;
}

void VM_print_stdout(int val) {
  if (DECORATE_STDOUT)
    log("VM_STDOUT: %c\n", (char) val);
  else
    putchar((char) val);
}

void VM_run_stack_peek(VM *self) {
  int comm_index = VM_peek_stack(self);
  Command *comm = &(self->program[comm_index]);
  int arg1, arg2;
  log("_________NEXT_COMMAND_____________\n");
  log("VM: Stack Size %d , Current Command index %d\n", self->stack_size, comm_index);
  Command_log(comm);

  switch (comm->type) {
    case WRITE:
      arg1 = atoi(comm->arg1);
      VM_print_stdout(VM_read(self, VM_read(self, arg1)));
      VM_increment_peek(self);
      break;
    case READ:
      arg1 = atoi(comm->arg1);
      int in = getchar();
      int addr = VM_read(self, arg1);
      VM_write(self, addr, in == EOF ? -1 : in);
      VM_increment_peek(self);
      break;
    case SUBTRACT:
      arg1 = atoi(comm->arg1);
      arg2 = atoi(comm->arg2);
      int val1 = VM_read(self, VM_read(self, arg1));
      int val2 = VM_read(self, VM_read(self, arg2));
      VM_write(self, VM_read(self, arg1), val1 - val2);
      VM_increment_peek(self);
      break;
    case CALL_LABEL:
      log("VM_CALL: %s\n", comm->arg1);
      VM_increment_peek(self);
      VM_push_stack(self, VM_find_labeled(self, comm->arg1));
      break;
    case CONDITION:
      arg1 = atoi(comm->arg1);
      if (VM_read(self, VM_read(self, arg1)) > 0) {
        log("VM_CONDITION_TRUE %s\n", comm->arg1);
        VM_set_peek(self, VM_find_labeled(self, comm->arg2));
      } else {
        log("VM_CONDITION_WRONG\n");
        VM_increment_peek(self);
      }
      break;
    case RETURN:
      VM_pop_stack(self);
      break;
  }

}


void VM_run_program(VM *self) {
  log("\n\n\n\n***START***\n");
  if (self->program_len > 0)
    VM_push_stack(self, 0);
  while (self->stack_size > 0 && VM_peek_stack(self) < self->program_len) {
    VM_run_stack_peek(self);
  }
}


void VM_read_program(VM *self) {
  log("*** PARSING SOURCE CODE ***\n");
  Parser *parser = Parser_new();
  while (parser -> has_next) {
    Command command = Parser_next_command(parser);
    log("%d\t", self -> program_len);
    Command_log(&command);
    self->program[self -> program_len ++] = command;
  }
  free(parser);
}


int main() {
  VM *vm = VM_new();
  VM_read_program(vm);
  VM_run_program(vm);
  free(vm);
  return 0;
}
