#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct Node_t {
  struct Node_t *left;
  struct Node_t *right;
  int val;
} Node;


Node *NN(int val, Node *left, Node *right) {
  Node *n = malloc(sizeof(Node));
  n->val = val;
  n->left = left;
  n->right = right;
  return n;
}

Node *LN(int val) {
  return NN(val, NULL, NULL);
}

Node *find(Node *root, int *count) {
  if (!root) return NULL;

  Node *left_search = find(root->left, count);
  if (--(*count) == 0)
    return root;
  Node *right_search = find(root->right, count);

  if (left_search)
    return left_search;
  if (right_search)
    return right_search;
  return NULL;

}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);

  Node *root = NN(5,
                  NN(3,
                     NN(2,
                        LN(1),
                        NULL),
                     LN(4)),
                  NN(8,
                     NN(6,
                        NULL,
                        LN(7)),
                     LN(9)));


  for (int i = 1; i <= 9; ++i) {
    int K = i;
    Node *f = find(root, &K);
    printf("%d %d\n", i, f->val);
  }
  return 0;
}
