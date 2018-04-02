#ifndef TREE_H
#define TREE_H
#define EMPTY_ITEM (-1)

#include <stdbool.h>

struct LinkedList;
struct ListNode;
struct Tree;
struct TreeNode;

/**
 * @return
 * Instance of the Tree having a root element with id 0
 */
struct Tree *Tree_new();

/**
 * Destructor
 * @param self
 */
void Tree_free(struct Tree *self);
/**
 * @param self
 * @param parent_id
 * Id of the parent node
 * @param id
 * Child node id
 * @return
 * True if args were correct
 */
bool Tree_add_node(struct Tree *self, int parent_id, int id);
/**
 * @param self
 * @param node_id
 * The id of the node to be removed
 * @return
 * True if args were correct
 */
bool Tree_remove_node(struct Tree *self, int node_id);
/**
 *
 * @param self
 * @param node_id
 * The if of the node which will contain the item
 * @param item
 * The value of the item
 * @return
 * True if args were correct
 */
bool Tree_add_item(struct Tree *self, int node_id, int item);
/**
 * @param self
 * @param node_id
 * The id of the node which contains the item
 * @param item
 * The value of the item to be removed
 * @return
 * True if args were correct
 */
bool Tree_remove_item(struct Tree *self, int node_id, int item);
/**
 * @param self
 * @param node_id
 * The root node id from which the query will be executed
 * @param limit
 * The maximum number of items to select
 * @param ans
 * A buffer array which will be filled with the query result
 * @return
 * True if args were correct
 */
bool Tree_extract_max(struct Tree *self, int node_id, int limit, int ans[]);


#endif
