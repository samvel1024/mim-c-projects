#include <stdio.h>
#include <stdlib.h>


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


Node *remove_negative(Node *l) {
  Node *fake = Node_new(1, l);
  Node *curr = fake;
  while (curr) {
    if (curr->next && curr->next->val < 0)
      curr->next = curr->next->next;
    else curr = curr->next;
  }
  free(fake);
  return fake -> next;
}

int main() {
  Node *head = LL_read();
  Node *l = remove_negative(head);
  while (l != NULL) {
    printf("%d ", l->val);
    l = l->next;
  }
  return 0;
}
