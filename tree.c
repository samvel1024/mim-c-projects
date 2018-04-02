#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <memory.h>
#include "tree.h"

#define NODE_LOOKUP_SIZE 65536


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

ListNode *ListNode_new_int(int val) {
	int *ptr = malloc(sizeof(int));
	*ptr = val;
	return ListNode_new(ptr);
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
	ListNode *curr = self->head->next;
	while (curr != self->tail) {
		ListNode *next = curr->next;
		destruct(curr->val);
		free(curr);
		curr = next;
	}
	free(self->head);
	free(self->tail);
	free(self);
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
	ListNode *curr = self->head;

	while (curr->next != self->tail) {
		int curr_val = ListNode_as_int(curr->next);
		if (curr_val < val)
			break;
		else if (curr_val == val)
			return NULL;
		curr = curr->next;
	}

	ListNode *new_node = ListNode_new_int(val);
	ListNode_add_after(curr, new_node, new_node);
	return new_node;
}


bool LinkedList_equal(LinkedList *self, int arr[]) {
	assert(self->head->val == NULL);
	assert(self->tail->val == NULL);
	int index = 0;
	ListNode *n = self->head->next;
	while (n != self->tail) {
		assert(ListNode_as_int(n) == arr[index]);
		++index;
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
	LinkedList_deep_free(ll, free);
}


/********************* Tree *******************************/



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

void TreeNode_free_adapt_void(void *ptr);

void TreeNode_free(TreeNode *self) {
	LinkedList_deep_free(self->items, free);
	LinkedList_deep_free(self->children, TreeNode_free_adapt_void);
	free(self);
}

void TreeNode_free_adapt_void(void *ptr) {
	TreeNode_free((TreeNode *) ptr);
}


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
	t->node_lookup = malloc(NODE_LOOKUP_SIZE * sizeof(void*));
	for (int i = 0; i < NODE_LOOKUP_SIZE; ++i)
		t->node_lookup[i] = NULL;
	t->node_lookup[0] = root;
	return t;
}

void Tree_free(Tree *self) {
	free(self->node_lookup);
	TreeNode_free(self->root);
	free(self);
}

bool Tree_valid_id(int i) {
	return (i >= 0 && i <= NODE_LOOKUP_SIZE - 1);
}

TreeNode *Tree_get(Tree *self, int id) {
	return self->node_lookup[id];
}

bool Tree_exists_node(Tree *self, int id) {
	return Tree_valid_id(id) && Tree_get(self, id) != NULL;
}

bool Tree_add_node(struct Tree *self, int parent_id, int id) {
	if (!Tree_exists_node(self, parent_id) || Tree_exists_node(self, id)) return false;

	TreeNode *parent = Tree_get(self, parent_id);
	TreeNode *child = TreeNode_new(id);
	TreeNode_add_child(parent, child);
	self->node_lookup[id] = child;

	return true;
}

bool Tree_remove_node(struct Tree *self, int rem_id) {
	if (rem_id == 0 || !Tree_exists_node(self, rem_id)) return false;
	TreeNode *node = Tree_get(self, rem_id);

	if (!LinkedList_is_empty(node->children))
		ListNode_add_after(node->in_parent, node->children->head->next, node->children->tail->prev);

	ListNode_remove(node->in_parent);
	free(node->in_parent);
	LinkedList_shallow_free(node->children);
	LinkedList_deep_free(node->items, free);
	free(node);
	self->node_lookup[rem_id] = NULL;
	return true;
}

bool Tree_add_item(Tree *self, int node_id, int item) {
	if (!Tree_exists_node(self, node_id))
		return false;
	TreeNode *t = Tree_get(self, node_id);
	ListNode *added = LinkedList_insert_sorted_desc(t->items, item);
	return added != NULL;
}

bool Tree_remove_item(Tree *self, int node_id, int item) {
	if (!Tree_exists_node(self, node_id))
		return false;
	TreeNode *node = Tree_get(self, node_id);
	ListNode *rem = node->items->head->next;
	bool found = false;
	while (!found && rem != node->items->tail) {
		int val = ListNode_as_int(rem);
		if (val == item) {
			ListNode_remove(rem);
			free(rem->val);
			free(rem);
			found = true;
		} else rem = rem->next;
	}
	return found;
}


void TreeNode_collect_items(TreeNode *node, int items[], int limit, int min) {
	int count = 0;
	ListNode *curr = node->items->head->next;
	while (curr != node->items->tail && ListNode_as_int(curr) > min && count < limit) {
		items[count++] = ListNode_as_int(curr);
		curr = curr->next;
	}
	while (count < limit)
		items[count++] = EMPTY_ITEM;
}

void merge_sorted(int dest[], const int a[], const int b[], int len) {
	int ai = 0, bi = 0, di = 0;
	while (ai < len && bi < len && di < len) {
		if (a[ai] > b[bi])
			dest[di++] = a[ai++];
		else if (b[bi] > a[ai])
			dest[di++] = b[bi++];
		else {
			int val = b[bi];
			while (bi < len && b[bi] == val) ++bi;
			while (ai < len && a[ai] == val) ++ai;
			dest[di++] = val;
		}
	}
	while (ai < len && di < len)
		dest[di++] = a[ai++];
	while (bi < len && di < len)
		dest[di++] = b[bi++];
}


void TreeNode_collect_recursive(TreeNode *curr, int parent_max, int limit, int ans[]) {

	TreeNode_collect_items(curr, ans, limit, parent_max);


	int my_max = LinkedList_is_empty(curr->items) ? parent_max : ListNode_as_int(curr->items->head->next);
	my_max = parent_max > my_max ? parent_max : my_max;

	int *buff1 = malloc(sizeof(int) * limit);
	int *buff2 = malloc(sizeof(int) * limit);

	ListNode *child = curr->children->head->next;
	while (child != curr->children->tail) {
		memcpy(buff1, ans, sizeof(int) * limit);
		TreeNode_collect_recursive((TreeNode *) (child->val), my_max, limit, buff2);
		merge_sorted(ans, buff1, buff2, limit);
		child = child->next;
	}

	free(buff1);
	free(buff2);

}


bool Tree_extract_max(Tree *self, int node_id, int limit, int ans[]) {
	if (!Tree_exists_node(self, node_id))
		return false;
	if (limit >= 0)
		TreeNode_collect_recursive(Tree_get(self, node_id), -1, limit, ans);
	else
		ans[0] = EMPTY_ITEM;
	return true;
}


void TEST_TreeExtractMax() {
	Tree *t = Tree_new();
	Tree_add_node(t, 0, 2);
	Tree_add_node(t, 2, 1);

	Tree_add_item(t, 0, 1337);
	Tree_add_item(t, 0, 1410);
	Tree_add_item(t, 2, 1683);
	Tree_add_item(t, 1, 2018);

	int ans[] = {1, 2, 3};
	Tree_extract_max(t, 0, 3, ans);
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

	assert(Tree_add_item(t, 0, 3));
	assert(Tree_add_item(t, 0, 1));
	assert(Tree_add_item(t, 0, 2));
	assert(Tree_add_item(t, 1, 6));
	assert(Tree_add_item(t, 1, 7));
	assert(Tree_add_item(t, 1, 9));
	assert(Tree_add_item(t, 1, 10));
	assert(Tree_add_item(t, 10, 11));
	assert(Tree_add_item(t, 10, 12));

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

void TEST_merge() {
	int a[] = {1, 2, 3, 4};
	int b[] = {4, 3, 0, -1};
	int c[] = {5, 4, -1, -1};
	merge_sorted(a, b, c, 4);

}

//int main() {
//	TEST_merge();
////	TEST_TreeExtractMax();
//	return 0;
//}

