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


Node *reverse_rec(Node *node, Node **new_head) {
  if (!node->next) {
    *new_head = node;
    return node;
  }
  Node *rev = reverse_rec(node->next, new_head);
  rev->next = node;
  return node;
}

Node *reverse(Node *node) {
  if (!node) return NULL;
  Node *nhead = NULL;
  reverse_rec(node, &nhead);
  node->next = NULL;
  return nhead;
}


int main() {
  Node *head = LL_read();
  LL_print(reverse(head));
  return 0;
}
