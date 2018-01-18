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

int cycle_len(Node *meet_node){
  Node *curr = meet_node->next;
  int len = 0;
  while(curr != meet_node) {
    curr = curr->next;
    ++len;
  }
  return len;
}



int tail_len(Node *meet_node, Node *head){
  int len = 0;
  while(meet_node != head){
    len++;
    meet_node = meet_node -> next;
    head = head -> next;
  }
  return len+1;
}

int cyclic_len(Node *head) {

  Node *slow, *fast;
  slow = fast = head;


  while (slow && fast) {
    slow = slow->next;
    fast = (fast->next) ? fast->next->next : NULL;
    if (slow == fast && slow != NULL) {
      return tail_len(slow, head) + cycle_len(slow);
    }
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

  printf("%d", cyclic_len(cyclic));


  return 0;
}
