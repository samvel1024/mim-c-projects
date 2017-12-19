#include <stdio.h>
#include <stdbool.h>


void f(int subset[], int subset_size, int super_set_size, int curr_element) {

  if (curr_element == super_set_size) {
    printf("{");
    for (int i =0; i< subset_size ; ++i){
      printf("%d ", subset[i]);
    }
    printf("}\n");
    return;
  }


  f(subset, subset_size, super_set_size, curr_element + 1);
  subset[subset_size] = curr_element + 1;
  f(subset, subset_size + 1, super_set_size, curr_element + 1);


}


int main() {
  int N;
  scanf("%d", &N);
  int subset[N + 1];
  f(subset, 0, N, 0);
}
