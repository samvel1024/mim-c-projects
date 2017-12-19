#include <stdio.h>


void swap(int A[], int x, int y) {
  int temp = A[x];
  A[x] = A[y];
  A[y] = temp;
}


void permute(int perm[], int left, int right, int len) {

  if (left == right) {
    for (int i = 0; i < len; ++i)
      printf("%d ", perm[i]);
    printf("\n");
  }
  for (int i = left; i <= right; i++) {
    swap(perm, left,i);
    permute(perm, left + 1, right, len);
    swap(perm , left, i);
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
