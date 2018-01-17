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

void split(Node *head, int len, Node *heads[]) {
  Node *end = head;
  Node *start = head;

  for (int i = 0; i < len && end; ++i) {
    heads[i] = end;
    end = end->next;
  }

  while (end) {
    Node *start_next = start->next;
    start->next = end;
    end = end->next;
    start = start_next;
  }

  while(start){
    Node *sn = start -> next;
    start -> next = NULL;
    start = sn;
  }

}


int main() {
  int list_len, partitions;
  scanf("%d %d", &list_len, &partitions);
  Node *list = Node_new(list_len, NULL);
  for (int i=1 ;i<list_len; ++i){
    list = Node_new(list -> val-1, list);
  }

  Node **parts = calloc((size_t) partitions, sizeof(Node*));
  split(list, partitions, parts);

  for (int i=0; i<partitions; ++i)
    LL_print(parts[i]);
  return 0;
}
