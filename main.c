#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


bool f(int set[], int set_size, int sums[], int partitions, int curr_index) {
  if (curr_index == set_size) {
    for (int i = 1; i < partitions; ++i) {
      if (sums[i] != sums[i - 1])
        return false;
    }
    return true;
  }

  for (int i = 0; i < partitions; ++i) {
    sums[i] += set[curr_index];
    if ((f(set, set_size, sums, partitions, curr_index + 1)))
      return true;
    sums[i] -= set[curr_index];
  }

  return false;
}

int main() {


  int partitions;
  int set_size;
  scanf("%d", &set_size);
  scanf("%d", &partitions);
  int set[set_size];
  int *sums = calloc(partitions, sizeof(int));
  for (int i = 0; i < set_size; ++i)
    scanf("%d", &set[i]);
  bool possible = f(set, 5, sums, partitions, 0);
  printf("%d", possible);
  return 0;
}
