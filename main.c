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

Node **LL_min(Node **one, Node **two) {
  if (*one == NULL)
    return two;
  if (*two == NULL)
    return one;
  return (*one)->val < (*two)->val ? one : two;
}

Node *LL_merge(Node **one, Node **two) {
  Node **head = LL_min(one, two);
  Node **prev_min = head;
  (*prev_min) -> next = *head;
  while((*one) -> next != NULL && (*two) -> next != NULL){
    Node **min = LL_min(one, two);
    printf("%d ", (*min) -> val);
    (*prev_min) -> next = *min;
    (*min) = (*min) -> next;
  }
  return *head;
}

int main() {
  Node *first = LL_read();
  Node *second = LL_read();
  Node *merged = LL_merge(&first, &second);
  LL_print(merged);
  return 0;
}
