#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MODE_DEBUG
#ifdef MODE_DEBUG
#define ENABLE_LOG
#else
#endif

#ifdef ENABLE_LOG
#define log printf
#else
#define log(...)
#endif

#define BUF_SIZE 50
#define COMMAND_SIZE 3001
#define ADDR_MIN_NEG 6000
#define ADDR_MAX_POS 6000
#define ADDR_SIZE ADDR_MIN_NEG + ADDR_MAX_POS + 1


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
  log("Command{\n\tlabel: '%s'\n\targ1: '%s'\n\targ2: '%s'\n\ttype: '%s'\n}\n", this->label, this->arg1, this->arg2,
      name);
}

typedef struct vm_t {
  Command program[1000];
  int size;
  int memory[ADDR_MIN_NEG + ADDR_MAX_POS];
} VM;


void VM_write(VM *this, int addr, int val) {
  this->memory[addr] = val;
}

int VM_read(VM *this, int addr) {
  if (addr < -ADDR_MIN_NEG || addr > ADDR_MAX_POS)
    return -1 - addr;
  return this->memory[addr];
}

int VM_find_label(VM *this, char *label) {
  for (int i = 0; i < this->size; ++i) {
    if (strcmp(label, this->program[i].label) == 0)
      return i;
  }
  return -1;
}

void VM_run_command_at(VM *this, int command) {
  Command *c = &(this->program[command]);
  int arg1, arg2;
  switch (c->type) {
    case WRITE:
      arg1 = atoi(c->arg1);
      putchar(VM_read(this, VM_read(this, arg1)));
      break;
    case READ:
      arg1 = atoi(c->arg1);
      int in = getchar();
      VM_write(this, VM_read(this, arg1), in == EOF ? -1 : in);
      break;
    case SUBTRACT:
      arg1 = atoi(c->arg1);
      arg2 = atoi(c->arg2);
      int val1 = VM_read(this, VM_read(this, arg1));
      int val2 = VM_read(this, VM_read(this, arg2));
      VM_write(this, VM_read(this, arg1), val1 - val2);
      break;
    case CALL_LABEL:
      VM_run_command_at(this, VM_find_label(this, c->arg1));
      break;
    case CONDITION:
      arg1 = atoi(c->arg1);
      if (VM_read(this, VM_read(this, arg1)) > 0)
        VM_run_command_at(this, VM_find_label(this, c->arg2));
      break;
    case RETURN:
      break;
  }

}


void VM_run_program(VM *this) {
  for (int curr = 0; curr < this->size; ++curr) {
    if (!this->program[curr].is_labeled)
      VM_run_command_at(this, curr);
  }
}

VM *VM_new() {
  VM *vm = malloc(sizeof(VM));
  vm->size = 0;
  for (int i = -ADDR_MIN_NEG; i < ADDR_MAX_POS; ++i) {
    VM_write(vm, i, -1 - i);
  }
  return vm;
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
  while (true) {
    Word first = Word_next_word();
    if (first.is_last && first.length == 0)
      break;
    bool is_labeled = (first.word[0] == ':');
    Command c = {.is_labeled = is_labeled};
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
    this->program[this->size++] = c;
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
