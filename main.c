#include <stdio.h>
#include <stdlib.h>


typedef struct b_node {
  int val;
  struct b_node *left;
  struct b_node *right;
} Vert;


Vert *Vert_new(int val, Vert *left, Vert *right) {
  Vert *v = malloc(sizeof(Vert));
  v -> val = val;
  v -> left = left;
  v -> right = right;
  return v;
}

int main() {
  int i;
  scanf("%d", &i);
  printf("%d\n", 2 * i);
  return 0;
}
