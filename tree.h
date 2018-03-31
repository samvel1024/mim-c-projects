#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

struct LinkedList;
struct ListNode;
struct Tree;
struct TreeNode;


struct Tree *Tree_new();

bool Tree_add_node(struct Tree *self, int parent_id, int id);

bool Tree_remove_node(struct Tree *self, int node);

bool Tree_add_item(struct Tree *self, int node_id, int item);

bool Tree_remove_item(struct Tree *self, int node_id, int item);

#endif
