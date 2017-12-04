#include <stdio.h>

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

#define ADDR_MIN_NEG 1000
#define ADDR_MAX_POS 1000
#define ADDR_SIZE ADDR_MIN_NEG + ADDR_MAX_POS + 1


#define PRINT 1
#define SUBTRACT 2


typedef struct command_t {
  char *arg1;
  char *arg2;
  int type;
} Command;

int mem[ADDR_SIZE];



void init_mem() {
  for (int i = -ADDR_MIN_NEG; i < ADDR_MAX_POS; ++i) {
    mem[i] = -1 - i;
  }
}

void vm_start() {
  init_mem();
}


int main() {

  vm_start();
}
