#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tree.h"

/********************* Doubly Linked List *******************************/
void LinkedList_remove_node(struct LinkedList *self, struct ListNode *node);

struct ListNode *LinkedList_push_tail(struct LinkedList *self, void *val);

struct ListNode *LinkedList_insert_sorted(struct LinkedList *self, int val);

struct ListNode *LinkedList_concat(struct LinkedList *self, struct LinkedList *merged);


typedef struct ListNode {
	void *val;
	struct ListNode *prev;
	struct ListNode *next;
} ListNode;

ListNode *ListNode_new(void *val) {
	ListNode *l = malloc(sizeof(ListNode));
	l->val = val;
	l->prev = l->next = NULL;
	return l;
}

ListNode *ListNode_add_after(ListNode *self, ListNode *new_node) {
	ListNode *next = self->next;
	next->prev = new_node;
	new_node->next = next;
	new_node->prev = self;
	self->next = new_node;
	return new_node;
}

int ListNode_as_int(ListNode *self) {
	return *((int *) self->val);
}


typedef struct LinkedList {
	ListNode *head;
	ListNode *tail;
} LinkedList;

LinkedList *LinkedList_new() {
	LinkedList *ll = malloc(sizeof(LinkedList));
	ll->head = ListNode_new(NULL);
	ll->tail = ListNode_new(NULL);
	ll->head->next = ll->tail;
	ll->tail->prev = ll->head;
	return ll;
}

void *LinkedList_deep_free(LinkedList *self) {
	ListNode *curr = self->head;
	while (curr) {
		ListNode *next = curr->next;
		free(curr->val);
		free(curr);
		curr = next;
	}
	free(self);
}

void LinkedList_remove_node(LinkedList *self, ListNode *node) {
	if (node == self->head || node == self->tail) {
		printf("Error: attempt to remove sentinel nodes");
		exit(1);
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node->val);
	free(node);
}

boolean LinkedList_is_empty(LinkedList *self) {
	return self->head->next == self->tail;
}


ListNode *LinkedList_push_tail(LinkedList *self, void *val) {
	ListNode *new_node = ListNode_new(val);
	return ListNode_add_after(self->tail->prev, new_node);
}

ListNode *LinkedList_insert_sorted_desc(LinkedList *self, int val) {
	int *ptr = malloc(sizeof(int));
	*ptr = val;
	ListNode *new_node = ListNode_new(ptr);
	ListNode *curr = self->head;

	while (curr->next != self->tail) {
		int curr_val = ListNode_as_int(curr->next);
		if (curr_val < val)
			break;
		curr = curr->next;
	}

	return ListNode_add_after(curr, new_node);


}

ListNode *LinkedList_concat(LinkedList *self, LinkedList *merged) {
	self->tail->prev->next = merged->head->next;
	merged->head->next->prev = self->tail->prev;
	free(self->tail);
	free(merged->head);
}


void TEST_linkedListImpl() {
	LinkedList *ll = LinkedList_new();
	assert(ll->head == NULL);
	assert(ll->tail == NULL);
	ListNode *node = LinkedList_insert_sorted(ll, 1);
	assert(ll->head == ll->tail);
	assert((*(int *) (ll->head->val)) == 1);
	assert(ll->head != NULL);
	LinkedList_remove_node(ll, node);
	assert(ll->head == NULL);
	assert(ll->tail == NULL);
	free(ll);
}


/********************* Tree *******************************/


#define NODE_LOOKUP_SIZE 1000

typedef struct TreeNode {
	LinkedList *items;
	LinkedList *children;
	ListNode *in_parent;
	int id;
} TreeNode;

TreeNode *TreeNode_new(int id) {
	TreeNode *t = malloc(sizeof(TreeNode));
	t->children = LinkedList_new();
	t->items = LinkedList_new();
	t->in_parent = NULL;
	t->id = id;
};

void TreeNode_add_child(TreeNode *parent, TreeNode *child) {
	ListNode *ref = LinkedList_push_tail(parent->children, child);
	child->in_parent = ref;
}


typedef struct Tree {
	TreeNode *root;
	TreeNode *node_lookup[NODE_LOOKUP_SIZE];
} Tree;


Tree *Tree_new() {
	Tree *t = malloc(sizeof(Tree));
	TreeNode *root = TreeNode_new(0);
	t->root = root;
	//TODO init lookup array
	return t;
}

TreeNode *add_node(struct Tree *self, int parent_id, int id) {
	if (parent_id < 0 || parent_id > NODE_LOOKUP_SIZE || self->node_lookup[parent_id] == NULL)
		return NULL;
	if (id < 0 || id > NODE_LOOKUP_SIZE || self->node_lookup[id] != NULL)
		return NULL;

	TreeNode *parent = self->node_lookup[parent_id];
	TreeNode *child = TreeNode_new(id);
	TreeNode_add_child(parent, child);

	return child;
}


