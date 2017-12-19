#include <stdio.h>


void swap(int *x, int *y) {
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}


void permute(int perm[], int left, int right, int len) {

  if (left == right) {
    for (int i = 0; i < len; ++i)
      printf("%d ", perm[i]);
    printf("\n");
  }
  for (int i = left; i <= right; i++) {
    swap((perm + left), (perm + i));
    permute(perm, left + 1, right, len);
    swap((perm + left), (perm + i)); //backtrack
  }

}


int main() {
//  f(N, used, perm, 0);
  int N = 3;
  int nums[N];
  for (int i=0; i<N ; ++i) nums[i] = i+1;
  permute(nums, 0, N-1, N);
  return 0;
}
