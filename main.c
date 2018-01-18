#include <stdio.h>
#include <stdlib.h>


typedef struct node_t {
  struct node_t *next;
  int val;
} Node;

typedef struct ll_node {
  Node *ll;
  struct ll_node *next;
} LLNode;


LLNode *LLN(Node *val, LLNode *next) {
  LLNode *n = malloc(sizeof(Node));
  n->ll = val;
  n->next = next;
  return n;
}

Node *N(int val, Node *next) {
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
  Node *fake = N(1, NULL);
  Node *curr = fake;
  while (size--) {
    int val;
    scanf("%d", &val);
    curr->next = N(val, NULL);
    curr = curr->next;
  }
  return fake->next;
}

Node *extract_min(Node **first, Node **second) {
  Node ***min_ptr = (*first == NULL) ? &second : (*second == NULL ) ? &first :
                                         (*first)->val < (*second)->val ? &first : &second;
  Node *ret_val = **min_ptr;
  (**min_ptr) = (**min_ptr)->next;
  return ret_val;
}


Node *merge(LLNode *ll) {
  if (!ll) return NULL;

  LLNode *firstLL = ll;
  LLNode *secondLL = ll->next;

  while (secondLL) {

    Node *first = firstLL->ll;
    Node *second = secondLL->ll;
    Node *curr = (first->val < second->val) ? first : second;
    while (first || second) {
      Node *min = extract_min(&first, &second);
      curr->next = min;
      curr = curr->next;
    }

    secondLL = secondLL->next;

  }


  return firstLL->ll;

}


int main() {
  Node *n1 = N(1, N(4, N(6, N(10, NULL))));
  Node *n2 = N(20, N(123, N(123, N(123, NULL))));
  Node *n3 = N(-5, N(9, N(-2, NULL)));
  Node *n4 = N(50, N(92, N(45, N(6, N(89, N(12, NULL))))));
  LLNode *ln = LLN(n1, LLN(n2, LLN(n3, LLN(n4, NULL))));
  Node *merged = merge(ln);
  LL_print(merged);
  return 0;
}
