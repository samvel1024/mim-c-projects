#include <stdio.h>
#include <stdlib.h>


typedef struct Node_t {
  struct Node_t *left;
  struct Node_t *right;
  int val;
} Node;


Node *NN(int val, Node *left, Node *right){
  Node *n = malloc(sizeof(Node));
  n -> val = val;
  n -> left = left;
  n -> right = right;
  return n;
}

Node *LN(int val){
  return NN(val, NULL, NULL);
}

int main() {
  int i;
  scanf("%d", &i);
  printf("%d\n", 2 * i);
  return 0;
}
