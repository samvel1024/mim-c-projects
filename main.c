#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

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

bool is_bst(Node *root, int from, int to) {
  return (root == NULL) ? true :
         root->val >= from && root->val <= to &&
         is_bst(root->left, from, root->val) &&
         is_bst(root->right, root->val, to);
}


int main() {
  Node *root = NN(5,
                  NN(3,
                     LN(2),
                     LN(6)),
                  LN(6));
  printf("%d", is_bst(root, INT_MIN, INT_MAX));
  return 0;
}


