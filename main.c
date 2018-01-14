#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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
  putchar('\n');
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

void group(Node *head) {

  if (!head) return;

  Node *sum = head;
  while (sum->next) {
    Node *next = sum->next;
    if (sum->val * next->val < 0) {
      sum = next;
      next = sum->next;
    }
    if (next) {
      sum->val += next->val;
      sum->next = next->next;
    }

  }

}


int main() {
  Node *head = LL_read();
  group(head);
  LL_print(head);

  return 0;
}
