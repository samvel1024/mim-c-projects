#include <stdio.h>
#include <stdlib.h>


void print(int len, char str[]) {
  for (int i = 0; i < len; ++i)
    putchar(str[i]);
  putchar('\n');
}

void f(int len, char str[], int start, int end, int covered) {
  if (covered == len) {
    print(len, str);
    return;
  }
  if (start >= end) return;
  str[start] = '(';
  for (int i = start + 1; i <= end; i += 2) {
    str[i] = ')';
    if (covered != len - 2) {
      f(len, str, start + 1, i - 1, covered + 2);
    }
    f(len, str, i + 1, end, covered + 2);
  }

}

//alternative way
void g(char str[], int pos, int n, int open, int close) {
  if (close == n) {
    printf("%s \n", str);
    return;
  }

  if (open > close) {
    str[pos] = '}';
    g(str, pos + 1, n, open, close + 1);
  }

  if (open < n) {
    str[pos] = '{';
    g(str, pos + 1, n, open + 1, close);
  }

}

int main() {
  int N = 4;
  char *str = calloc(N, sizeof(char));
//  f(N, str, 0, N - 1, 0);
  g(str, 0, N / 2, 0, 0);
}
