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

bool is_cyclic(Node *head) {

  Node *slow, *fast;
  slow = fast = head;


  while (slow && fast) {
    slow = slow->next;
    fast = (fast->next) ? fast->next->next : NULL;

    if (slow == fast && slow != NULL)
      return true;
    if (slow == NULL || fast == NULL)
      return false;
  }

  return false;

}


int main() {
  Node *flat = Node_new(1, Node_new(2, Node_new(3, Node_new(4, NULL))));

  Node *cycle_head = Node_new(4, NULL);
  Node *cyclic = Node_new(1, Node_new(2, Node_new(3, cycle_head)));
  Node *cycle = Node_new(5, Node_new(6, Node_new(7, Node_new(8, cycle_head))));
  cycle_head -> next = cycle;

  printf("%d %d", is_cyclic(flat), is_cyclic(cyclic));


  return 0;
}
