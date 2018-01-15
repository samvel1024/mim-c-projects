#include <stdio.h>
#include <stdbool.h>
#include <tkDecls.h>


typedef struct Node_t {
  struct Node_t *left;
  struct Node_t *right;
  int val;
} Node;


bool is_bst(Node *root, int *min, int *max) {

  if (root->right == NULL && root->left == NULL) {
    *min = root->val;
    *max = root->val;
    return true;
  }

  int lmax, lmin, rmax, rmin;

  bool left_bst = root->left == NULL || is_bst(root->left, &lmin, &lmax);
  bool right_bst = root->right == NULL || is_bst(root->right, &rmin, &rmax);

  if (!left_bst || !right_bst) return false;

  if (root->left != NULL) {
    if (root->val < lmax) {
      return false;
    }
    *min = (*min < lmin) ? *min : lmin;
    *max = (*max > lmax) ? *max : lmax;
  }

  if (root->right != NULL) {
    if (root->val > rmin) {
      return false;
    }
    *min = (*min < rmin) ? *min : rmin;
    *max = (*max > rmax) ? *max : rmax;
  }

  return true;

}


int main() {
  int i;
  scanf("%d", &i);
  printf("%d\n", 2 * i);
  return 0;
}
