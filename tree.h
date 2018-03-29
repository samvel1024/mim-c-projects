#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

struct LinkedList;
struct ListNode;
struct Tree;
struct TreeNode;


struct Tree *Tree_new();

struct TreeNode *Tree_add_node(struct Tree *self, int parent_id, int id);

bool Tree_remove_node(struct Tree *self, int node);

#endif