#include <stdio.h>
#include <stdlib.h>

/**
 * Given singly linked list perform following operations
 *
 * Remove item from list if value is negative
 * Given value of item is N replace the node with N ones
 * Do nothing if value is 0
 */

typedef struct node_t {
  struct node_t *next;
  int val;
} Node;

Node *Node_new(int val, Node *next) {
  Node *n = malloc(sizeof(Node));
  n->val = val;
  n->next = next;
  return n;
}


Node *LL_read() {
  int size = 0;
  scanf("%d", &size);
  Node *head = NULL;
  Node **curr = &head;
  while (size--) {
    int val;
    scanf("%d", &val);
    *curr = Node_new(val, NULL);
    curr = &((*curr)->next);
  }
  return head;
}

void LL_print(Node *head){
  while(head){
    printf("%d ", head -> val);
    head = head -> next;
  }
  printf("\n");

}


Node *link_ones_to(Node *next, int count) {
  Node *one = Node_new(1, next);
  for (int i = 1; i < count; ++i) {
    one = Node_new(1, one);
  }
  return one;
}


Node *replace_add(Node *head) {

  Node *fake_head = Node_new(0, head);
  Node *prev = fake_head;
  Node *curr = head;

  while (curr) {

    if (curr->val > 1) {
      Node *added = link_ones_to(curr->next, curr->val);
      prev->next = added;
      curr = added;
    } else if (curr->val < 0) {
      prev->next = curr->next;
      curr = curr->next;
    } else  {
      prev = curr;
      curr = curr->next;
    }

  }

  return fake_head->next;
}

int main() {
  Node *head = LL_read();
  head = replace_add(head);
  LL_print(head);

  return 0;
}
