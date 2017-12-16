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

#define BUF_SIZE 100
#define PROGRAM_LEN_MAX 3001
#define ADDR_MAX 6000
#define STACK_MAX 5000

void fake_printf(const char *format, ...) {
  UNUSED(format);
}

typedef enum {
  WRITE,
  READ,
  SUBTRACT,
  CALL_LABEL,
  CONDITION,
  RETURN,
  LABEL
} CommandType;

const char *COMMAND_NAMES[] = {"WRITE", "READ", "SUBTRACT", "CALL_LABEL", "CONDITION", "RETURN", "LABEL"};

typedef struct command_t {
  char arg1[BUF_SIZE];
  char arg2[BUF_SIZE];
  CommandType type;
} Command;


void Command_log(Command *this) {
  const char *name = COMMAND_NAMES[this->type];
  log("%s(%s%s%s) %s %s\n", name,
      this->arg1, strlen(this->arg2) == 0 ? "" : ",",
      this->arg2);
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
  return  !isspace(c) && c!= ';';
}

bool Parser_is_number(char c) {
  return isdigit(c) || c == '-' || c == '+';
}


void Parser_skip(Parser *self) {
  while (self->has_next && Parser_is_whitespace(self->buff) && Parser_read(self));
}

Parser *Parser_new() {
  Parser *self = malloc(sizeof(Parser));
  self->has_next = true;
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
  Parser_read(self);
  Parser_skip(self);
  Parser_read_word(self, c->arg1, Parser_not_whitespace);
  Parser_skip(self);
}

Command Parser_next_command(Parser *self) {
  Command c = {.arg1 = "", .arg2 = "", .type = -1};
  if (self -> buff == ':'){
    Parser_read_label(self, &c);
    c.type = LABEL;
  }
  else if (self->buff == ';') {
    c.type = RETURN;
    Parser_read(self);
  } else if (self->buff == '^') {
    Parser_read(self);
    Parser_skip(self);
    Parser_read_word(self, c.arg1, Parser_is_number);
    c.type = READ;
  } else if (Parser_is_number(self->buff)) {
    Parser_read_word(self, c.arg1, Parser_is_number);
    Parser_skip(self);
    if (self->buff == '^') {
      Parser_read(self);
      c.type = WRITE;
    } else if (Parser_is_number(self->buff)) {
      Parser_read_word(self, c.arg2, Parser_is_number);
      c.type = SUBTRACT;
    } else {
      Parser_read_word(self, c.arg2, Parser_not_whitespace);
      c.type = CONDITION;
    }
  } else {
    Parser_read_word(self, c.arg1, Parser_not_whitespace);
    c.type = CALL_LABEL;
  }
  Parser_skip(self);
  return c;

}


typedef struct vm_t {
  Command program[PROGRAM_LEN_MAX];
  int program_len;
  int heap[2 * ADDR_MAX];
  int stack[STACK_MAX];
  int stack_size;
} VM;

VM *VM_new() {
  VM *vm = malloc(sizeof(VM));
  vm->program_len = 0;
  vm->stack_size = 0;
  for (int i = -ADDR_MAX; i < ADDR_MAX; ++i) {
    vm->heap[i + ADDR_MAX] = -1 - i;
  }
  return vm;
}

void VM_mem_write(VM *self, int addr, int val) {
  self->heap[addr + ADDR_MAX] = val;
  log("VM: Write addr %d val %d\n", addr, self->heap[addr + ADDR_MAX]);
}

int VM_mem_read(VM *self, int addr) {
  if (addr < -ADDR_MAX || addr > ADDR_MAX)
    return -1 - addr;
  log("VM: Read  addr %d val %d\n", addr, self->heap[addr + ADDR_MAX]);
  return self->heap[addr + ADDR_MAX];
}

int VM_find_labeled(VM *self, char *label) {
  for (int i = 0; i < self->program_len; ++i) {
    if (self->program[i].type == LABEL && strcmp(label, self->program[i].arg1) == 0) {
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

  self->stack[self->stack_size++] = ptr;
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


void VM_exec_write(VM *self, Command *comm) {
  int arg1 = atoi(comm->arg1);
  int val = VM_mem_read(self, VM_mem_read(self, arg1));
  if (DECORATE_STDOUT)
    log("VM_STDOUT: %c\n", (char) val);
  else
    putchar((char) val);
  VM_increment_peek(self);
}

void VM_exec_read(VM *self, Command *comm) {
  int arg1 = atoi(comm->arg1);
  int in = getchar();
  int addr = VM_mem_read(self, arg1);
  VM_mem_write(self, addr, in == EOF ? -1 : in);
  VM_increment_peek(self);
}

void VM_exec_subtract(VM *self, Command *comm) {
  int arg1 = atoi(comm->arg1);
  int arg2 = atoi(comm->arg2);
  int val1 = VM_mem_read(self, VM_mem_read(self, arg1));
  int val2 = VM_mem_read(self, VM_mem_read(self, arg2));
  VM_mem_write(self, VM_mem_read(self, arg1), val1 - val2);
  VM_increment_peek(self);
}

void VM_exec_call_label(VM *self, Command *comm) {
  log("VM_CALL: %s\n", comm->arg1);
  VM_increment_peek(self);
  if (strlen(comm -> arg1) > 0)
    VM_push_stack(self, VM_find_labeled(self, comm->arg1));
}

void VM_exec_condition(VM *self, Command *comm) {
  int arg1 = atoi(comm->arg1);
  if (VM_mem_read(self, VM_mem_read(self, arg1)) > 0) {
    log("VM_CONDITION_TRUE %s\n", comm->arg1);
    VM_set_peek(self, VM_find_labeled(self, comm->arg2));
  } else {
    log("VM_CONDITION_WRONG\n");
    VM_increment_peek(self);
  }
}

void VM_exec_return(VM *self, Command *comm) {
  UNUSED(comm);
  VM_pop_stack(self);
}

void VM_exec_label(VM *self, Command *comm){
  UNUSED(comm);
  VM_increment_peek(self);
}

void (*VM_COMMAND_EXECUTORS[])(VM*, Command*) = {VM_exec_write, VM_exec_read, VM_exec_subtract, VM_exec_call_label, VM_exec_condition, VM_exec_return, VM_exec_label};

void VM_run_stack_peek(VM *self) {
  int comm_index = VM_peek_stack(self);
  Command *comm = &(self->program[comm_index]);
  log("_________NEXT_COMMAND_____________\n");
  log("VM: Stack Size %d , Current Command index %d\n", self->stack_size, comm_index);
  Command_log(comm);
  VM_COMMAND_EXECUTORS[comm->type](self, comm);
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
  while (parser->has_next) {
    Command command = Parser_next_command(parser);
    log("%d\t", self->program_len);
    Command_log(&command);
    self->program[self->program_len++] = command;
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
