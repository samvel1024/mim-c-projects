#include <stdio.h>
#include <stdbool.h>


int N = 3;
bool used[100];
int perm[100];


void f(int n, bool used[], int perm[], int used_size) {

  if (used_size == n) {
    for (int i = 0; i < n; ++i)
      printf("%d ", perm[i]);
    printf("\n");
    return;
  }

  for (int i = 1; i <= n; ++i) {
    if (!used[i - 1]) {
      perm[used_size] = i;
      used[i - 1] = true;
      f(n, used, perm, used_size + 1);
      used[i - 1] = false;
    }
  }

}


void swap(int *x, int *y) {
  char temp;
  temp = *x;
  *x = *y;
  *y = temp;
}


void permute(int *a, int l, int r) {

  if (l == r) {
    for (int i = 0; i < N; ++i)
      printf("%d ", a[i]);
    printf("\n");
  }
  for (int i = l; i <= r; i++) {
    swap((a + l), (a + i));
    permute(a, l + 1, r);
    swap((a + l), (a + i)); //backtrack
  }

}


int main() {
//  f(N, used, perm, 0);
  int nums[] = {1, 2, 3};
  permute(nums, 0, 2);
  return 0;
}
