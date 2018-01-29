#include <stdio.h>


int A[] = {1, 3, 1, 5, 2, 1, 6, 3, 7, 3, 4, 5, 9, 120, 3, 121};
int len = 16;

int max(int a, int b) { return a > b ? a : b; }


int lcs(int prev, int curr) {

  if (curr == -1)
    return 0;


  if (prev == len || A[prev] > A[curr]) {
    return max(1 + lcs(curr, curr - 1), lcs(prev, curr - 1));
  } else {
    return lcs(prev, curr - 1);
  }

}


int main() {
  printf("%d", lcs(len, len-1));

  return 0;
}
