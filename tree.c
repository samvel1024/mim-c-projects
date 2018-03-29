#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tree.h"

/********************* Doubly Linked List *******************************/
void LinkedList_remove_node(struct LinkedList *self, struct ListNode *node);

struct ListNode *LinkedList_push_tail(struct LinkedList *self, void *val);

struct ListNode *LinkedList_push_tail_int(struct LinkedList *self, int val);

struct ListNode *LinkedList_concat(struct LinkedList *self, struct LinkedList *merged);


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

void *LinkedList_deep_free(LinkedList *self){
	ListNode *curr = self->head;
	while(curr){
		ListNode *next = curr->next;
		free(curr -> val);
		free(curr);
		curr = next;
	}
	free(self);
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

	free(node->val);
	free(node);
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
	if (id < 0 || id > NODE_LOOKUP_SIZE)
		return NULL;

	TreeNode *parent = self->node_lookup[parent_id];
	TreeNode *child = TreeNode_new(id);
	TreeNode_add_child(parent, child);

	return child;
}


