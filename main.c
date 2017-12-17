#include <stdio.h>
#include <stdbool.h>


int A[1000];
bool visited[1000];
int N;

typedef struct P {
  int left;
  int right;
} Pair;

int max(int a, int b) {
  return a < b ? b : a;
}

Pair first() {
  int right = 1;
  while (A[right] != A[0] && right < N) ++right;
  Pair p = {.right = right, .left = 0};
  if (p.right < N)
    visited[0] = visited[right] = true;
  return p;
}

Pair next(Pair curr) {
  int left = curr.left + 1;
  while (left < N && visited[left]) ++left;
  visited[left] = true;

  int right = max(left + 1, curr.right + 1);
  while (right < N && A[left] != A[right]) ++right;
  visited[right] = true;

  Pair p = {.right = right, .left = left};
  return p;

}


int main() {
  scanf("%d", &N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &A[i]);
  }
  int covered = 0;
  Pair curr = first();
  while (curr.right < N) {
    covered += 2;
    curr = next(curr);
  }
  printf("%d\n", covered == N);
  return 0;
}
