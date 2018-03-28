#ifndef TREE_H
#define TREE_H


struct LinkedList;
struct ListNode;
struct Tree;
struct TreeNode;


struct Tree *Tree_new();
struct TreeNode *add_node(struct Tree *self, int parent_id, int id);

#endif
