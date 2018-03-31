#ifndef TREE_H
#define TREE_H
#define EMPTY_ITEM (-1)

#include <stdbool.h>

struct LinkedList;
struct ListNode;
struct Tree;
struct TreeNode;


struct Tree *Tree_new();

void Tree_free(struct Tree *self);

bool Tree_add_node(struct Tree *self, int parent_id, int id);

bool Tree_remove_node(struct Tree *self, int node);

bool Tree_add_item(struct Tree *self, int node_id, int item);

bool Tree_remove_item(struct Tree *self, int node_id, int item);

bool Tree_extract_max(struct Tree *self, int node_id, int limit, int ans[]);



#endif
