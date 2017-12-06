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
  int curr_command;
} VM;


int VM_to_int(char *c) {
  return atoi(c);
}

bool VM_is_int_between(Word *w, int from, int to) {
  for (int i = from + (w->word[0] == '-' || w->word[0] == '+'); i < to; ++i) {
    if (!isdigit(w->word[i])) return false;
  }
  return true;
}

bool VM_is_int(Word *w) {
  return VM_is_int_between(w, 0, w->length);
}


bool VM_is_one_word_command(Word *first) {
  return (first->length == 1 && first->word[0] == ';') || !VM_is_int(first);
}

void VM_run_command_at(VM *this, int command) {
  Command *c = &(this->program[command]);
  switch (c->type) {
    case WRITE:
      break;
    case READ:
      break;
    case SUBTRACT:
      break;
    case CALL_LABEL:
      break;
    case CONDITION:
      break;
    case RETURN:
      break;
  }

}

void VM_run_program(VM *this) {
  this->curr_command = 0;
  for (this->curr_command = 0; this->curr_command < this->size; ++this->curr_command) {

  }
}

void VM_read_one_word_command(Word *first, Command *c) {

  if (first->length == 1 && first->word[0] == ';') {
    c->type = RETURN;
  } else if (first->length > 1 && first->word[0] == '^' && VM_is_int_between(first, 1, first->length)) {
    c->type = READ;
    memcpy(c->arg1, &(first->word[1]), first->length - 1);
    c->arg1[first->length - 1] = '\0';
  } else if (first->length > 1 && first->word[first->length - 1] == '^' &&
             VM_is_int_between(first, 0, first->length - 1)) {
    c->type = WRITE;
    memcpy(c->arg1, &(first->word[0]), first->length - 1);
    c->arg1[first->length - 1] = '\0';
  } else {
    c->type = CALL_LABEL;
    strcpy(c->arg1, first->word);
  }
}

void VM_read_two_word_command(Word *first, Word *second, Command *c) {
  c->type = (VM_is_int(first) && VM_is_int(second)) ? SUBTRACT : CONDITION;
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
    if (VM_is_one_word_command(&first)) {
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
//  VM vm = {.size = 0};
//  VM_read_program(&vm);
  printf("%d", (int) sizeof(VM));

  return 0;
}
