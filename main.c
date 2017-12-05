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
  printf("Word{\n\tword: '%s'\n\tsize: %d\n\tis_last: %d\n}\n", this->word, this->length, this->is_last);
}

typedef enum {
  WRITE = 100,
  READ,
  SUBTRACT,
  RETURN,
  END_OF_PROGRAM
} CommandType;

typedef struct command_t {
  char label[BUF_SIZE];
  char arg1[BUF_SIZE];
  char arg2[BUF_SIZE];
  bool is_labeled;
  CommandType type;
} Command;

void Command_log(Command * this) {
  printf("Command{\n\tlabel: '%s'\n\targ1: '%s'\n\targ2: '%s'\n\ttype: %d\n}\n", this->label, this->arg1, this->arg2,
         this->type);
}

typedef struct vm_t {
  Command program[1000];
  int size;
} VM;

int to_int(char c[]) {
  return atoi(c);
}

bool is_int(char c[]) {
  int len = strlen(c);
  if (c[0] != '-' || c[0] != '+' || !isdigit(c[0]))
    return false;
  for (int i = 1; i < len; ++i) {
    if (!isdigit(c[i])) return false;
  }
  return true;
}


void VM_read_program(VM * this) {
  while (true) {
    first = Word_next_word();
    if (w.is_last) {
      Command c = {.is_labeled = false, .type = END_OF_PROGRAM};
      this->program[this.size++] = c;
      break;
    }
    char label[BUF_SIZE];
    bool is_labeled = (first.word[0] == ':');
    Command c = {.is_labeled = is_labeled, .label = (is_labeled) ? first.word : ""};
    if (c.is_labeled) {
      first = Word_next_word();
    }
    CommandType type = 0; //Undefined
    if (first.lenght == 1) {
      switch (first.word[0]) {
        case '^':
          type = READ;
          break;
        case ';':
          type = RETURN;
          break;
      }
    }
  }

}


int main() {

  return 0;
}
