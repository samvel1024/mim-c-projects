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

int len(Node *a) {
  int len = 0;
  while (a) {
    ++len;
    a = a->next;
  }
  return len;
}


Node *first_common(Node *a, Node *b) {
  int len1 = len(a);
  int len2 = len(b);

  while (a && b) {
    if (a == b)
      return a;

    if (len1 > len2) {
      a = a->next;
      len1--;
    }
    else if (len1 < len2) {
      b = b->next;
      len2--;
    }else {
      a = a->next;
      b = b->next;
    }
  }

  return NULL;

}


