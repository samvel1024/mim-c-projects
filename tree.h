#ifndef TREE_H
#define TREE_H


#include <jmorecfg.h>

struct LinkedList;
struct ListNode;
struct Tree;
struct TreeNode;


struct Tree *Tree_new();

struct TreeNode *add_node(struct Tree *self, int parent_id, int id);

boolean remove_node(struct Tree *self, int node);

#endif
