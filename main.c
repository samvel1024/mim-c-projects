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
#define ADDR_MIN_NEG 1000
#define ADDR_MAX_POS 1000
#define ADDR_SIZE ADDR_MIN_NEG + ADDR_MAX_POS + 1

/**
 * Represents an input word of the program
 */
typedef struct word_t {
  char word[BUF_SIZE];
  int length;
  bool is_last;
} Word;

/**
 * Reads the next word in stdin, terminates reading after first & and eof
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
  while (!isspace(c = getchar())) {
    if (c == EOF || c == '&') {
      w.is_last = true;
      break;
    }
    w.word[w.length++] = c;
    w.word[w.length] = '\0';
  }
  return w;
}

void Word_log(Word * this) {
  log("Word{\n\tword: '%s'\n\tsize: %d\n\tis_last: %d\n}\n", this->word, this->length, this->is_last);
}

typedef enum {
  WRITE = 100,
  READ,
  SUBTRACT,
  CALL_LABEL,
  CONDITION,
  RETURN,
} CommandType;

typedef struct command_t {
  char label[BUF_SIZE];
  char arg1[BUF_SIZE];
  char arg2[BUF_SIZE];
  bool is_labeled;
  CommandType type;
} Command;

void Command_log(Command * this) {
  log("Command{\n\tlabel: '%s'\n\targ1: '%s'\n\targ2: '%s'\n\ttype: %d\n}\n", this->label, this->arg1, this->arg2,
      this->type);
}

typedef struct vm_t {
  Command program[1000];
  int size;
} VM;

int to_int(char c[]) {
  return atoi(c);
}

bool is_int(Word *w) {
  for (int i = (w->word[0] == '-' || w->word[0] == '+'); i < w->length; ++i) {
    if (!isdigit(w->word[i])) return false;
  }
  return true;
}


bool VM_is_one_word_command(Word *first) {
  return (first->length == 1 && first->word[0] == ';') || !is_int(first);
}

void VM_read_one_word_command(Word *first, Command *c) {
  if (first->length == 1 && first->word[0] == ';')
    c->type = RETURN;
  else {
    c->type = CALL_LABEL;
    strcpy(c->arg1, first->word);
  }
}

void VM_read_two_word_command(Word *first, Word *second, Command *c) {
  if (is_int(first) && second->length == 1 && second->word[0] == '^') {
    c->type = WRITE;
    strcpy(c->arg1, first->word);
  } else if (first->length == 1 && first->word[0] == '^' && is_int(second)) {
    c->type = READ;
    strcpy(c->arg1, second->word);
  } else {
    c->type = (is_int(first) && is_int(second)) ? SUBTRACT : CONDITION;
    strcpy(c->arg1, first->word);
    strcpy(c->arg2, second->word);
  }
}

void VM_read_program(VM * this) {
  while (true) {
    Word first = Word_next_word();
    bool is_labeled = (first.word[0] == ':');
    Command c = {.is_labeled = is_labeled};
    if (is_labeled) strcpy(c.label, first.word);
    if (c.is_labeled) {
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
  VM vm = {.size = 0};
  VM_read_program(&vm);

  return 0;
}
