#include <stdio.h>


int A[1000];
int N;


int max_len = 1;
int range_min = 0;
int range_max = 0;
int from = 0;
int to = 0;


void move_head() {
  ++to;
  range_max = max(to, range_max);
  range_min = min(range_min, to);
}

void cut_tail(int from){

}


int main() {
  scanf("%d", &N);
  for (int i = 0; i < N; ++i)
    scanf("%d", &A[i]);

  range_min = A[0];
  range_max = A[0];

  for (int i = 1; i < N; ++i) {

  }
  printf("%d", max_len);

  return 0;
}
