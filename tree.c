#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <memory.h>
#include "tree.h"

#define NODE_LOOKUP_SIZE 65536


/********************* ListNode definitions *******************************/

/**
 *
 * @param val
 * Pointer to the item to be stored in the list
 * @return
 * Not linked ListNode instance
 */
struct ListNode *ListNode_new(void *val);

/**
 * Convenience method for adding integers
 * @param val
 * int value to be stored
 * @return
 */
struct ListNode *ListNode_new_int(int val);

/**
 * Links a chain of ListNodes with head and tail after self assuming that all
 * nodes from new_head to new_tail are linked
 * @param self
 * @param new_head
 * @param new_tail
 */
void ListNode_add_after(struct ListNode *self, struct ListNode *new_head, struct ListNode *new_tail);

/**
 * Unlinks self from neighbour nodes
 * @param self
 */
void ListNode_unlink(struct ListNode *self);

/**
 * Convenience method to get the value of the node as int
 * @param self
 * @return
 */
int ListNode_as_int(struct ListNode *self);


/********************* ListNode implementation *******************************/
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

void ListNode_unlink(ListNode *self) {
	self->prev->next = self->next;
	self->next->prev = self->prev;
}

int ListNode_as_int(ListNode *self) {
	return *((int *) self->val);
}

/********************* LinkedList definitions *******************************/

/**
 * The implementation below stores two sentinel nodes
 * for head and tail of the doubly linked list
 */

/**
 * @return
 * Initialized LinkedList with sentinel nodes linked together
 */
struct LinkedList *LinkedList_new();

/**
 * @param self
 * @param destruct
 * Destructor function for each ListNode value for performing deep free.
 * A shallow free will be performed once NULL is passed
 */
void LinkedList_free(struct LinkedList *self, void (*destruct)(void *));
/**
 * @param self
 * @return
 * True if self has no elements
 */
bool LinkedList_is_empty(struct LinkedList *self);

/**
 * @param self
 * @param val
 * @return
 * Newly created ListNode which is linked to the end of the list
 */
ListNode *LinkedList_push_tail(struct LinkedList *self, void *val);

/**
 * @param self
 * @param val
 * @return
 * Inserts integer into the list preserving the descending sorted order
 */
ListNode *LinkedList_insert_sorted_desc(struct LinkedList *self, int val);


/********************* LinkedList implementation *******************************/

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


void LinkedList_free(LinkedList *self, void (*destruct)(void *)) {
	ListNode *curr = self->head->next;
	while (destruct != NULL && curr != self->tail) {
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


/********************* Tree Node implementation ************************/

typedef struct TreeNode {
	LinkedList *items;
	LinkedList *children;
	ListNode *in_parent;
} TreeNode;

/**
 * Constructor
 */
TreeNode *TreeNode_new() {
	TreeNode *t = malloc(sizeof(TreeNode));
	t->children = LinkedList_new();
	t->items = LinkedList_new();
	t->in_parent = NULL;
	return t;
};

/**
 * Used as a destruction strategy for deep freeing LinkedList of TreeNodes
 */
void TreeNode_free_adapt_void(void *ptr);

/**
 * Deep free the node (including all nested fields)
 * @param self
 */
void TreeNode_free(TreeNode *self) {
	LinkedList_free(self->items, free);
	LinkedList_free(self->children, TreeNode_free_adapt_void);
	free(self);
}

void TreeNode_free_adapt_void(void *ptr) {
	TreeNode_free((TreeNode *) ptr);
}

/**
 * Links child node to the parent
 */
void TreeNode_add_child(TreeNode *parent, TreeNode *child) {
	ListNode *ref = LinkedList_push_tail(parent->children, child);
	child->in_parent = ref;
}


/********************* Tree implementation*******************************/

typedef struct Tree {
	TreeNode *root;
	TreeNode **node_lookup;
} Tree;


Tree *Tree_new() {
	Tree *t = malloc(sizeof(Tree));
	TreeNode *root = TreeNode_new();
	t->root = root;
	t->node_lookup = malloc(NODE_LOOKUP_SIZE * sizeof(void *));
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

bool Tree_is_valid_id(int i) {
	return (i >= 0 && i <= NODE_LOOKUP_SIZE - 1);
}

TreeNode *Tree_get(Tree *self, int id) {
	return self->node_lookup[id];
}

bool Tree_exists_node(Tree *self, int id) {
	return Tree_is_valid_id(id) && Tree_get(self, id) != NULL;
}

bool Tree_add_node(struct Tree *self, int parent_id, int id) {
	if (!Tree_exists_node(self, parent_id) || Tree_exists_node(self, id)) return false;

	TreeNode *parent = Tree_get(self, parent_id);
	TreeNode *child = TreeNode_new();
	TreeNode_add_child(parent, child);
	self->node_lookup[id] = child;

	return true;
}

bool Tree_remove_node(struct Tree *self, int node_id) {
	if (node_id == 0 || !Tree_exists_node(self, node_id)) return false;
	TreeNode *node = Tree_get(self, node_id);

	if (!LinkedList_is_empty(node->children))
		ListNode_add_after(node->in_parent, node->children->head->next, node->children->tail->prev);

	ListNode_unlink(node->in_parent);
	free(node->in_parent);
	LinkedList_free(node->children, NULL);
	LinkedList_free(node->items, free);
	free(node);
	self->node_lookup[node_id] = NULL;
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
			ListNode_unlink(rem);
			free(rem->val);
			free(rem);
			found = true;
		} else rem = rem->next;
	}
	return found;
}

/**
 * Writes elements stored in node into items preserving sorted order up to limit
 */
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

/**
 * Merges arrays a and b into dest preserving sorted order
 */
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
	while (di < len)
		dest[di++] = EMPTY_ITEM;
}

/**
 * Recursive helper function
 * Writes the result of the query into ans
 */
void TreeNode_collect_recursive(TreeNode *curr, int parent_max, int limit,
                                int ans[], int aux[]) {

	int my_max = LinkedList_is_empty(curr->items) ? parent_max : ListNode_as_int(curr->items->head->next);
	int max = parent_max > my_max ? parent_max : my_max;

	int *my_buff = malloc(sizeof(int) * limit);
	TreeNode_collect_items(curr, my_buff, limit, parent_max);

	ListNode *child = curr->children->head->next;
	while (child != curr->children->tail) {
		TreeNode_collect_recursive((TreeNode *) child->val, max, limit, ans, aux);
		child = child->next;
	}

	merge_sorted(aux, ans, my_buff, limit);
	memcpy(ans, aux, sizeof(int) * limit);
	free(my_buff);
}


bool Tree_extract_max(Tree *self, int node_id, int limit, int ans[]) {
	if (!Tree_exists_node(self, node_id))
		return false;
	for (int i = 0; i < limit; ++i)
		ans[i] = EMPTY_ITEM;
	int *aux = malloc(sizeof(int) * limit);
	TreeNode_collect_recursive(Tree_get(self, node_id), -1, limit, ans, aux);
	free(aux);
	return true;
}
