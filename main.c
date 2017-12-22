#include <stdio.h>

void swap(char A[], int i, int j) {
  char tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}


void f(char A[], int from, int to, int mid) {

  if (from >= mid) {
    printf("%s\n", A);
    return;
  }

  for (int i = mid; i >= from; --i) {
    swap(A, mid, i);
    f(A, i + 1, to, mid + 1);
    swap(A, mid, i);
  }

}

int main() {
  int N = 6;
  char A [] = {'0', '0', '0', '1', '1', '1'};
  f(A, 0, N-1, N/2);
  return 0;
}
