#include <stdio.h>
#include <stdbool.h>



int cache[10000];

int f(char str[], int from) {
  if (from == 0)
    return 1;

  bool two = from > 1 ? (str[from-1]-'0' + (str[from - 2]-'0') * 10 <= 26) ? true : false : false;

  return f(str, from - 1) + (two ? f(str, from - 2) : 0);

}


int main() {
  char str[] = "1111111111";
  printf("%d", f(str, 10));
  return 0;
}
