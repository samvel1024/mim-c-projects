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
  if (head == NULL) return;
  do {
    printf("%d ", head->val);
    head = head->next;
  } while (head != NULL);
  printf("\n");
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

Node *LL_reverse(Node *head) {
  if (head == NULL) return NULL;
  Node *prev = head;
  Node *curr = head->next;
  while (curr != NULL) {
    Node *temp_next = curr -> next;
    curr->next = prev;
    prev = curr;
    curr = temp_next;
  }
  head->next = NULL;
  return prev;
}


int main() {
  Node *head = LL_read();
  head = LL_reverse(head);
  LL_print(head);
  return 0;
}
