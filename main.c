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


Node *LL_read(int *s) {
  int size = 0;
  scanf("%d", &size);
  *s = size;
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


void link(Node *curr, Node **head) {
  if (curr == NULL) {
    return;
  }
  link(curr->next, head);
  Node *hn = (*head)->next;
  (*head)->next = curr;
  (*head) = hn;
}


int main() {
  int size;
  Node *head = LL_read(&size);
  size = (size + 1)/2;
  Node *arr[size];
  Node *curr = head;
  for (int i = 0; i < size; ++i) {
    arr[i] = curr;
    curr = curr->next;
  }
  link(head, &head);
  for (int i = 0; i < size; ++i) {
    printf("%d %d\n", arr[i]->val, arr[i]->next->val);
  }

  return 0;
}
