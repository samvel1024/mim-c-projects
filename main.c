#include <stdlib.h>
#include <assert.h>


typedef struct ListNode {
  void *val;
  struct ListNode *prev;
  struct ListNode *next;
} ListNode;

typedef struct LinkedList {
  ListNode *head;
  ListNode *tail;
} LinkedList;

LinkedList *LinkedList_new() {
  LinkedList *ll = malloc(sizeof(LinkedList));
  ll->head = NULL;
  ll->tail = NULL;
  return ll;
}

void LinkedList_remove_node(LinkedList *self, ListNode *node) {
  if (node == self->head)
    self->head = node->next;
  if (node == self->tail)
    self->tail = node->prev;

  if (node->next)
    node->next->prev = node->prev;
  if (node->prev)
    node->prev->next = node->next;

  free(node);
  free(node -> val);
}


ListNode *LinkedList_push_tail(LinkedList *self, void *val) {
  ListNode *new_node = malloc(sizeof(ListNode));
  new_node->val = val;
  new_node->next = NULL;

  if (self->tail == NULL) {
    self->head = new_node;
    self->tail = new_node;
    new_node->prev = NULL;
  } else {
    new_node->prev = self->tail;
    self->tail->next = new_node;
  }
  return new_node;
}

ListNode *LinkedList_push_tail_int(LinkedList *self, int val) {
  int *ptr = malloc(sizeof(int));
  *ptr = val;
  return LinkedList_push_tail(self, ptr);
}

ListNode *LinkedList_concat(LinkedList *self, LinkedList *merged) {
  self->tail->next = merged->head;
  self->tail = merged->tail;
}


void TEST_linkedListImpl() {
  LinkedList *ll = LinkedList_new();
  assert(ll->head == NULL);
  assert(ll->tail == NULL);
  ListNode *node = LinkedList_push_tail_int(ll, 1);
  assert(ll->head == ll->tail);
  assert((*(int*)(ll->head->val)) == 1);
  assert(ll -> head != NULL);
  LinkedList_remove_node(ll, node);
  assert(ll->head == NULL);
  assert(ll->tail == NULL);
}

int main() {
  TEST_linkedListImpl();
  return 0;
}
