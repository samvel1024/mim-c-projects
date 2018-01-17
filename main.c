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


Node *extract_min(Node **first, Node **second) {
  Node ***min_ptr =
    *first == NULL ? &second : *second == NULL ? &first : (*first)->val <= (*second)->val ? &first : &second;
  Node *ans = **min_ptr;
  (**min_ptr) = (**min_ptr)->next;
  return ans;
}

Node *merge(Node *first, Node *second) {
  Node *head = (first->val <= second->val) ? first : second;
  Node *curr = head;
  while (first || second) {
    Node *next_min = extract_min(&first, &second);
    if (curr->val != next_min->val) {
      curr -> next = next_min;
      curr = curr->next;
    }
  }

  while(curr){
    if (curr -> val == curr -> next -> val)
      curr -> next = NULL;
    curr = curr -> next;
  }

  return head;
}

int main() {
  Node *first = LL_read();
  Node *second = LL_read();
  Node *merged = merge(first, second);
  LL_print(merged);
  return 0;
}
