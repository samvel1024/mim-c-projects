#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MODE_DEBUG
#ifdef MODE_DEBUG
#define log printf
#define DECORATE_STDOUT true
#else
#define log fake_printf
#define DECORATE_STDOUT false
#endif

#define UNUSED(x) (void)(x)

#define BUF_SIZE 50
#define COMMAND_SIZE 3001
#define ADDR_MAX 6000
#define STACK_MAX 5000

void fake_printf ( const char * format, ... ){
  UNUSED(format);
}

/**
 * Represents a word in the bytecode without whitespace
 * The property is_last is set to true iff the word is followed by EOF or '&'
 */
typedef struct word_t {
  char word[BUF_SIZE];
  int length;
  bool is_last;
} Word;


void Word_log(Word *this) {
  log("Word{\n\tword: '%s'\n\tsize: %d\n\tis_last: %d\n}\n", this->word, this->length, this->is_last);
}

/**
 * Reads the next word in stdin
 */
Word Word_next_word() {
  char c;
  while (isspace(c = getchar()));
  char first = c;
  if (first == EOF || first == '&') {
    Word w = {.is_last = true, .length = 0, .word = "\0"};
    return w;
  }
  Word w;
  w.length = 1;
  w.word[0] = first;
  w.word[1] = '\0';
  w.is_last = false;
  while (!isspace(c = getchar())) {
    if (c == EOF || c == '&') {
      w.is_last = true;
      break;
    }
    w.word[w.length++] = c;
    w.word[w.length] = '\0';
  }
//  Word_log(&w);
  return w;
}

bool Word_is_int_between(Word *this, int from, int to) {
  for (int i = from + (this->word[0] == '-' || this->word[0] == '+'); i < to; ++i) {
    if (!isdigit(this->word[i])) return false;
  }
  return true;
}

bool Word_is_int(Word *this) {
  return Word_is_int_between(this, 0, this->length);
}

bool Word_is_one_word_command(Word *first) {
  return (first->length == 1 && first->word[0] == ';') || !Word_is_int(first);
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
  log("%s(%s%s%s) %s%s\n", name,
      this->arg1, strlen(this->arg2) == 0 ? "" : ",",
      this->arg2,
      this->is_labeled ? "---> " : "",
      this->is_labeled ? this->label : "");
}

typedef struct vm_t {
  Command program[1000];
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
  int command = VM_peek_stack(self);
  Command *c = &(self->program[command]);
  int arg1, arg2;
  log("_________NEXT_COMMAND_____________\n");
  log("VM: Stack Size %d , Current Command index %d\n", self->stack_size, command);
  Command_log(c);

  switch (c->type) {
    case WRITE:
      arg1 = atoi(c->arg1);
      VM_print_stdout(VM_read(self, VM_read(self, arg1)));
      VM_increment_peek(self);
      break;
    case READ:
      arg1 = atoi(c->arg1);
      int in = getchar();
      int addr = VM_read(self, arg1);
      VM_write(self, addr, in == EOF ? -1 : in);
      VM_increment_peek(self);
      break;
    case SUBTRACT:
      arg1 = atoi(c->arg1);
      arg2 = atoi(c->arg2);
      int val1 = VM_read(self, VM_read(self, arg1));
      int val2 = VM_read(self, VM_read(self, arg2));
      VM_write(self, VM_read(self, arg1), val1 - val2);
      VM_increment_peek(self);
      break;
    case CALL_LABEL:
      log("VM_CALL: %s\n", c->arg1);
      VM_push_stack(self, VM_find_labeled(self, c->arg1));
      break;
    case CONDITION:
      arg1 = atoi(c->arg1);
      if (VM_read(self, VM_read(self, arg1)) > 0) {
        log("VM_CONDITION_TRUE %s\n", c->arg1);
        VM_set_peek(self, VM_find_labeled(self, c->arg2));
      } else {
        log("VM_CONDITION_WRONG\n");
        VM_increment_peek(self);
      }
      break;
    case RETURN:
      VM_pop_stack(self);
      VM_increment_peek(self);
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


void VM_read_one_word_command(Word *first, Command *c) {

  if (first->length == 1 && first->word[0] == ';') {
    c->type = RETURN;
  } else if (first->length > 1 && first->word[0] == '^' && Word_is_int_between(first, 1, first->length)) {
    c->type = READ;
    memcpy(c->arg1, &(first->word[1]), first->length - 1);
    c->arg1[first->length - 1] = '\0';
  } else if (first->length > 1 && first->word[first->length - 1] == '^' &&
             Word_is_int_between(first, 0, first->length - 1)) {
    c->type = WRITE;
    memcpy(c->arg1, &(first->word[0]), first->length - 1);
    c->arg1[first->length - 1] = '\0';
  } else {
    c->type = CALL_LABEL;
    strcpy(c->arg1, first->word);
  }
}

void VM_read_two_word_command(Word *first, Word *second, Command *c) {
  c->type = (Word_is_int(first) && Word_is_int(second)) ? SUBTRACT : CONDITION;
  strcpy(c->arg1, first->word);
  strcpy(c->arg2, second->word);

}

void VM_read_program(VM *this) {
  log("\n\n\n\n *** PARSING SOURCE CODE ***\n");
  while (true) {
    Word first = Word_next_word();
    if (first.is_last && first.length == 0)
      break;
    bool is_labeled = (first.word[0] == ':');
    Command c = {.is_labeled = is_labeled, .arg1 = "", .arg2 = ""};
    if (is_labeled) {
      memcpy(c.label, &(first.word[1]), first.length - 1);
      c.label[first.length - 1] = '\0';
      first = Word_next_word();
    }
    Word *curr = &first;
    if (Word_is_one_word_command(&first)) {
      VM_read_one_word_command(&first, &c);
    } else {
      Word second = Word_next_word();
      VM_read_two_word_command(&first, &second, &c);
      curr = &second;
    }
    Command_log(&c);
    this->program[this->program_len++] = c;
    if (curr->is_last) break;
  }

}


int main() {
  VM *vm = VM_new();
  VM_read_program(vm);
  VM_run_program(vm);
  free(vm);
  return 0;
}
