#include <stdio.h>
#include <stdlib.h>

void swap(char A[], int i, int j) {
  char tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}


void f(char A[], int from, int to, int mid) {

  if (from >= mid - to / 2) {
    printf("%s\n", A);
    return;
  }

  if (mid > to)
    return;

  for (int i = mid; i >= from; --i) {
    swap(A, mid, i);
    f(A, i + 1, to, mid + 1);
    swap(A, mid, i);
  }

}

int main() {

  int N;
  scanf("%d", &N);
  char *A = calloc(2 * N + 1, sizeof(char));
  for (int i = 0; i < 2 * N; ++i)
    A[i] = ((i < N) ? '0' : '1');

  f(A, 0, 2 * N - 1, N);
  return 0;
}
