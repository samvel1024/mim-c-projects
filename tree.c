#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"


/********************* Doubly Linked List *******************************/

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

void ListNode_add_after(ListNode *self, ListNode *new_head, ListNode *new_tail) {
	ListNode *next = self->next;
	next->prev = new_tail;
	new_tail->next = next;
	new_head->prev = self;
	self->next = new_head;
}

void ListNode_remove(ListNode *self) {
	self->prev->next = self->next;
	self->next->prev = self->prev;
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

void LinkedList_shallow_free(LinkedList *self) {
	free(self->head);
	free(self->tail);
	free(self);
}

void LinkedList_deep_free(LinkedList *self, void (*destruct)(void *)) {
	ListNode *curr = self->head;
	while (curr) {
		ListNode *next = curr->next;
		if (destruct)
			destruct(curr->val);
		else
			free(curr->val);
		free(curr);
		curr = next;
	}
	free(self);
}

void LinkedList_remove_node(LinkedList *self, ListNode *node, void (*destruct)(void *)) {
	if (node == self->head || node == self->tail) {
		printf("Error: attempt to remove sentinel nodes");
		exit(1);
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	if (destruct)
		destruct(node->val);
	else
		free(node->val);
	free(node);
}

bool LinkedList_is_empty(LinkedList *self) {
	return self->head->next == self->tail;
}


ListNode *LinkedList_push_tail(LinkedList *self, void *val) {
	ListNode *new_node = ListNode_new(val);
	ListNode_add_after(self->tail->prev, new_node, new_node);
	return new_node;
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

	ListNode_add_after(curr, new_node, new_node);
	return new_node;
}

void LinkedList_concat(LinkedList *self, LinkedList *merged) {
	self->tail->prev->next = merged->head->next;
	merged->head->next->prev = self->tail->prev;
	free(self->tail);
	free(merged->head);
}

bool LinkedList_equal(LinkedList *self, int arr[]) {
	assert(self->head->val == NULL);
	assert(self->tail->val == NULL);
	int index = 0;
	ListNode *n = self->head->next;
	while (n != self->tail) {
		assert(ListNode_as_int(n) == arr[index++]);
		n = n->next;
	}
	return true;
}


void TEST_linkedListImpl() {
	LinkedList *ll = LinkedList_new();
	LinkedList_insert_sorted_desc(ll, 0);
	LinkedList_insert_sorted_desc(ll, 2);
	LinkedList_insert_sorted_desc(ll, 1);
	int order[] = {2, 1, 0};
	assert(LinkedList_equal(ll, order));
	LinkedList_deep_free(ll, NULL);
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
	return t;
};

void TreeNode_add_child(TreeNode *parent, TreeNode *child) {
	ListNode *ref = LinkedList_push_tail(parent->children, child);
	child->in_parent = ref;
}


typedef struct Tree {
	TreeNode *root;
	TreeNode **node_lookup;
} Tree;


Tree *Tree_new() {
	Tree *t = malloc(sizeof(Tree));
	TreeNode *root = TreeNode_new(0);
	t->root = root;
	t->node_lookup = calloc(NODE_LOOKUP_SIZE, sizeof(TreeNode *));
	t->node_lookup[0] = root;
	return t;
}

bool Tree_valid_index(int i) {
	return (i >= 0 && i <= NODE_LOOKUP_SIZE);
}

bool Tree_exists_node(Tree *self, int id) {
	return Tree_valid_index(id) && self->node_lookup[id] != NULL;
}

TreeNode *Tree_get(Tree *self, int id) {
	return self->node_lookup[id];
}

TreeNode *Tree_add_node(struct Tree *self, int parent_id, int id) {
	if (!Tree_exists_node(self, parent_id) || Tree_exists_node(self, id)) return false;

	TreeNode *parent = Tree_get(self, parent_id);
	TreeNode *child = TreeNode_new(id);
	TreeNode_add_child(parent, child);
	self->node_lookup[id] = child;

	return child;
}

bool Tree_remove_node(struct Tree *self, int rem_id) {
	if (!Tree_exists_node(self, rem_id)) return false;
	if (rem_id == 0) return false;
	TreeNode *node = Tree_get(self, rem_id);
	if (node == NULL) return false;

	if (!LinkedList_is_empty(node->children))
		ListNode_add_after(node->in_parent, node->children->head->next, node->children->tail->prev);

	ListNode_remove(node->in_parent);
	free(node->in_parent);
	LinkedList_shallow_free(node->children);
	LinkedList_deep_free(node->items, NULL);
	self->node_lookup[rem_id] = NULL;
	free(node);
	return true;
}


void TEST_treeRemoveAndInsert() {
	Tree *t = Tree_new();
	assert(Tree_add_node(t, 0, 1));
	assert(Tree_add_node(t, 0, 2));
	assert(Tree_add_node(t, 0, 3));
	assert(Tree_add_node(t, 0, 10));
	assert(Tree_add_node(t, 0, 4));
	assert(Tree_add_node(t, 1, 5));
	assert(Tree_add_node(t, 2, 6));
	assert(Tree_add_node(t, 3, 7));
	assert(Tree_add_node(t, 4, 8));
	assert(Tree_add_node(t, 4, 9));

	assert(Tree_remove_node(t, 1));
	assert(Tree_remove_node(t, 2));
	assert(Tree_remove_node(t, 3));
	assert(Tree_remove_node(t, 4));
	assert(Tree_remove_node(t, 5));
	assert(Tree_remove_node(t, 6));
	assert(Tree_remove_node(t, 7));
	assert(Tree_remove_node(t, 8));
	assert(Tree_remove_node(t, 9));
	assert(Tree_remove_node(t, 10));

	assert(LinkedList_is_empty(t->root->children));
	LinkedList_shallow_free(t->root->children);
	LinkedList_shallow_free(t->root->items);
	free(t->root);
	free(t->node_lookup);
	free(t);
}

int main() {
	//TEST_linkedListImpl();
	TEST_treeRemoveAndInsert();
	return 0;
}

