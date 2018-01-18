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


void LL_print(Node *head) {
  if (!head) return;
  while (head) {
    printf("%d ", head->val);
    head = head->next;
  }
  printf("\n");
}


Node *LL_read() {
  int size = 0;
  scanf("%d", &size);
  Node *fake = Node_new(1, NULL);
  Node *curr = fake;
  while (size--) {
    int val;
    scanf("%d", &val);
    curr->next = Node_new(val, NULL);
    curr = curr->next;
  }
  return fake -> next;
}

int main() {
  Node *head = LL_read();
  LL_print(head);
  return 0;
}
