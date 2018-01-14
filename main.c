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


void solve(Node **nodes, int len) {



  for (int i = 0; i < len; ++i) {
    Node *curr = nodes[i];
    while (curr->next && curr->next != curr) {
      curr = curr->next;
    }

    if (curr->next == curr) {
      curr->val++;
    } else {
      curr->next = Node_new(1, NULL);
      curr->next->next = curr->next;
    }

  }

  for (int i = 0; i < len; ++i) {
    Node *curr = nodes[i];
    while(curr -> next != curr){
      curr = curr -> next;
    }
    printf("%d ", curr -> val);
  }



}

int main() {
  Node *l1 = Node_new(1, NULL);
  Node *l2 = Node_new(1, NULL);
  Node *l3 = Node_new(1, NULL);
  Node *l4 = Node_new(1, NULL);
  Node *l5 = Node_new(1, NULL);

  Node *h3 = Node_new(1, l1);
  Node *h5 = Node_new(1, l4);
  Node *h1 = Node_new(1, h3);
  Node *h2 = Node_new(1, l2);
  Node *h4 = Node_new(1, h5);


  Node *r1 = Node_new(1, h1);
  Node *r2 = Node_new(1, h2);
  Node *r3 = Node_new(1, l2);
  Node *r4 = Node_new(1, h3);
  Node *r5 = Node_new(1, h2);
  Node *r7 = Node_new(1, l3);
  Node *r6 = Node_new(1, r7);
  Node *r8 = Node_new(1, h4);
  Node *r9 = Node_new(1, h4);
  Node *r10 = Node_new(1, l5);

  Node *nodes[] = {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10};
  solve(nodes, 10);

  return 0;
}
