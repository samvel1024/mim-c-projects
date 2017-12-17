#include <stdio.h>


int A[1000];
int N;

typedef struct P {
  int left;
  int right;
} Pair;

int max(int a, int b) {
  return a < b ? b : a;
}

Pair next(Pair curr) {
  int left = curr.left + 1 == curr.right ? curr.right + 1 : curr.left + 1;
  int right = max(curr.right + 1, left + 1);
  while (right < N && A[left] != A[right]) {
    right++;
  }
  Pair p = {.left = left, .right = right};
  return p;
}


int main() {
  scanf("%d", &N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &A[i]);
  }
  int covered = -2;
  Pair curr = {-1, -1};
  while (curr.right < N) {
    covered += 2;
    curr = next(curr);
  }
  printf("%d\n", covered == N);
  return 0;
}
