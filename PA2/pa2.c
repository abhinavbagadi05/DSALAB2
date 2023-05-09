#include "../include/common.h"
#include "pa2.h"


// don't remove these globals
static struct record *bst_root = NULL;
static struct record *avl_root = NULL;
static size_t num_bst_nodes = 0;
static size_t num_avl_nodes = 0;

// don't modify this function
struct record *get_bst_root()
{
  return bst_root;
}

// don't modify this function
struct record *get_avl_root()
{
  return avl_root;
}

// don't modify this function
// compare two uids
// return -1 if uid1 < uid2
// return 0 if uid1 == uid2
// return 1 if uid1 > uid2
static int cmp_uid(char *uid1, char *uid2)
{
  int i;

  for (i = 0; i < MAX_LEN; i++) {
    if (uid1[i] > uid2[i]) {
      return 1;
    }
    else if (uid1[i] < uid2[i]) {
      return -1;
    }
  }
  return 0;
}

// don't modify this function
// compare two records
// we assume that uid is unique and
// there can't be two records with the 
// same uid
// return -1 if record1 < record2
// return 0 if record1 == record2
// return 1 if record1 > record2
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
}

// don't modify this function
static void *allocate_memory(size_t size)
{
  return __mymalloc(size);
}

// don't modify this function
static void free_memory(void *ptr)
{
  __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //

// Return the total number of records in the
// BST rooted at bst_root.
size_t get_num_bst_records()
{
    return num_bst_nodes;
}

// Return the total number of records in the
// AVL tree rooted at avl_root.
size_t get_num_avl_records()
{
    return num_avl_nodes;
}

// Insert record r in the BST rooted at bst_root.
void insert_record_bst(struct record r)
{
    // Create a new node to hold the record
    struct record *new_node = malloc(sizeof(struct record));
    memcpy(new_node, &r, sizeof(struct record));
    new_node->left = NULL;
    new_node->right = NULL;

    // If the tree is empty, set the new node as the root
    if (bst_root == NULL) {
        bst_root = new_node;
        return;
    }

    // Traverse the tree to find the appropriate leaf node to insert the new record
    struct record *current = bst_root;
    while (1) {
        if (strcmp(r.name, current->name) < 0) {
            if (current->left == NULL) {
                current->left = new_node;
                return;
            } else {
                current = current->left;
            }
        } else {
            if (current->right == NULL) {
                current->right = new_node;
                return;
            } else {
                current = current->right;
            }
        }
    }
}



// Insert record r in the AVL tree rooted at avl_root.
int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(struct record *node)
{
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int get_balance(struct record *node)
{
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

struct record *rotate_left(struct record *node)
{
    struct record *new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;

    node->height = 1 + max(height(node->left), height(node->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));

    return new_root;
}

struct record *rotate_right(struct record *node)
{
    struct record *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;

    node->height = 1 + max(height(node->left), height(node->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));

    return new_root;
}

struct record *insert_record_avl_helper(struct record *node, struct record *new_node)
{
    if (node == NULL) {
        return new_node;
    }

    if (strcmp(new_node->name, node->name) < 0) {
        node->left = insert_record_avl_helper(node->left, new_node);
    } else {
        node->right = insert_record_avl_helper(node->right, new_node);
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    // Left Left Case
    if (balance > 1 && strcmp(new_node->name, node->left->name) < 0) {
        return rotate_right(node);
    }

    // Right Right Case
    if (balance < -1 && strcmp(new_node->name, node->right->name) > 0) {
        return rotate_left(node);
    }

    // Left Right Case
    if (balance > 1 && strcmp(new_node->name, node->left->name) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(new_node->name, node->right->name) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void insert_record_avl(struct record r)
{
    // Create a new node to hold the record
    struct record *new_node = malloc(sizeof(struct record));
    memcpy(new_node, &r, sizeof(struct record));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;

    // If the tree is empty, set the new node as the root
    if (avl_root == NULL) {
        avl_root = new_node;
        return;
    }



}

// Search the record corresponding to uid in the BST
// rooted at bst_root.
// If the record is not present, return a dummy record
// with -1 in the status field.
struct record search_record_bst(char uid[MAX_LEN])
{
  // Start at the root
  struct record* curr = bst_root;

  // Traverse the tree until we find the record or reach a leaf node
  while (curr != NULL && strcmp(uid, curr->uid) != 0) {
    if (strcmp(uid, curr->uid) < 0) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }

  // If we found the record, return it; otherwise return a dummy record
  if (curr != NULL) {
    return *curr;
  } else {
    struct record dummy = {"", "", -1, {0,0}, NULL, NULL, -1, NULL, 0, NULL, NULL, NULL};
    return dummy;
  }
}


// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the BST rooted at bst_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_bst(char uid[MAX_LEN])
{
  // Search for the node with matching uid
  struct record user = search_record_bst(uid);

  // If the record is found, return the friends field
  if (user.status != -1) {
    return user.friends;
  } else {
    // Otherwise, return NULL
    return NULL;
  }
}


// Delete the record (say n) corresponding to uid from the BST.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.
struct record delete_record_bst(char uid[MAX_LEN]) {
    // Find the node to be deleted
    struct record *p = bst_root;
    struct record *parent = NULL;
    while (p != NULL) {
        int cmp = strcmp(uid, p->uid);
        if (cmp == 0) {
            // Found the node to be deleted
            break;
        } else {
            parent = p;
            if (cmp < 0) {
                p = p->left;
            } else {
                p = p->right;
            }
        }
    }

    if (p == NULL) {
        // The node is not present
        struct record dummy = {-1};
        return dummy;
    }

    // Remove n from the lists of friends of other records
    struct list_records *q = p->friends;
    while (q != NULL) {
        struct list_records *prev = NULL;
        struct record *f = q->record;
        struct list_records *r = f->friends;
        while (r != NULL) {
            if (r->record == p) {
                if (prev == NULL) {
                    f->friends = r->next;
                } else {
                    prev->next = r->next;
                }
                free(r);
                break;
            } else {
                prev = r;
                r = r->next;
            }
        }
        q = q->next;
    }

    // Release memory for all the nodes in the list of friends of n
    q = p->friends;
    while (q != NULL) {
        struct list_records *r = q;
        q = q->next;
        free(r);
    }

    // Remove the node from the BST
    if (p->left == NULL && p->right == NULL) {
        // The node has no children
        if (parent == NULL) {
            // The node is the root
            bst_root = NULL;
        } else if (parent->left == p) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
    } else if (p->left != NULL && p->right != NULL) {
        // The node has two children
        struct record *q = p->left;
        struct record *r = p;
        while (q->right != NULL) {
            r = q;
            q = q->right;
        }
        memcpy(p, q, sizeof(struct record));
        if (r == p) {
            r->left = q->left;
        } else {
            r->right = q->left;
        }
    } else {
        // The node has one child
        struct record *c = p->left ? p->left : p->right;
        if (parent == NULL) {
            // The node is the root
            bst_root = c;
        } else if (parent->left == p) {
            parent->left = c;
        } else {
            parent->right = c;
        }
    }

    // Release memory for the deleted node
    struct record copy = *p;
    free(p);

    return copy;
}


// Search the record corresponding to uid in the AVL tree
// rooted at avl_root.
// If the record is not present, return a dummy record 
// with -1 in the status field.
struct record search_record_avl(char uid[MAX_LEN])
{
    struct record dummy = {.status = -1}; // create a dummy record with status -1
    if (avl_root == NULL) {
        return dummy; // tree is empty, return dummy record
    }
    struct record *current = avl_root; // start from the root of AVL tree
    while (current != NULL) {
        if (strcmp(uid, current->uid) == 0) {
            return *current; // record with uid found, return it
        } else if (strcmp(uid, current->uid) < 0) {
            current = current->left; // search in the left subtree
        } else {
            current = current->right; // search in the right subtree
        }
    }
    return dummy; // record with uid not found, return dummy record
}


// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the AVL tree rooted at avl_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_avl_helper(struct record *root, char uid[MAX_LEN]) {
    if (root == NULL) {
        // Record not found, return NULL
        return NULL;
    } else if (strcmp(uid, root->uid) < 0) {
        // uid is smaller than the uid of the current node,
        // search in the left subtree
        return get_friends_list_avl_helper(root->left, uid);
    } else if (strcmp(uid, root->uid) > 0) {
        // uid is greater than the uid of the current node,
        // search in the right subtree
        return get_friends_list_avl_helper(root->right, uid);
    } else {
        // Found the record with the matching uid,
        // return the head of the linked list of friends
        return root->friends;
    }
}

struct list_records *get_friends_list_avl(char uid[MAX_LEN])
{
    struct record *root = avl_root;
    return get_friends_list_avl_helper(root, uid);
}


// Delete the record (say n) corresponding to uid from the AVL tree.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.
// Remove the node with the given key from the AVL tree rooted at root.
// Return the new root of the subtree.
struct record* avl_remove(struct record *root, char key[MAX_LEN]) {
    if (root == NULL) {
        // Node not found, return null.
        return NULL;
    } else if (strcmp(root->uid, key) > 0) {
        // Node to be deleted is in the left subtree.
        root->left = avl_remove(root->left, key);
        root = rebalance(root);
    } else if (strcmp(root->uid, key) < 0) {
        // Node to be deleted is in the right subtree.
        root->right = avl_remove(root->right, key);
        root = rebalance(root);
    } else {
        // Node found, remove it.
        if (root->left == NULL && root->right == NULL) {
            // Node has no children.
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            // Node has only a right child.
            struct record *temp = root;
            root = root->right;
            free(temp);
        } else if (root->right == NULL) {
            // Node has only a left child.
            struct record *temp = root;
            root = root->left;
            free(temp);
        } else {
            // Node has two children.
            // Find the predecessor of root in the left subtree.
            struct record *predecessor = root->left;
            while (predecessor->right != NULL) {
                predecessor = predecessor->right;
            }
            // Copy the predecessor's data into root.
            strncpy(root->name, predecessor->name, MAX_LEN);
            strncpy(root->uid, predecessor->uid, MAX_LEN);
            root->age = predecessor->age;
            root->loc = predecessor->loc;
            root->posts = predecessor->posts;
            root->friends = predecessor->friends;
            root->status = predecessor->status;
            root->pred = predecessor->pred;
            // Delete the predecessor.
            root->left = avl_remove(root->left, predecessor->uid);
            root = rebalance(root);
        }
    }
    return root;
}

struct record delete_record_avl(char uid[MAX_LEN])
{
    // Find the node to delete
    struct record deleted_node = search_record_avl(uid);

    // If node not found, return a dummy record with -1 in the status field
    if (deleted_node.status == INVALID_STATUS) {
        struct record dummy_record = { "", "", -1, {0}, NULL, NULL, INVALID_STATUS, NULL, 0, NULL, NULL };
        return dummy_record;
    }

    // Remove node from the friends lists of other records
    struct list_records *friends_list = get_friends_list_avl(uid);
    struct list_records *current = friends_list;
    while (current != NULL) {
        struct record *friend_record = current->record;
        struct list_records *friend_friends_list = friend_record->friends;
        struct list_records *prev = NULL;
        struct list_records *curr = friend_friends_list;
        while (curr != NULL) {
            if (strcmp(curr->record->uid, uid) == 0) {
                if (prev == NULL) {
                    friend_record->friends = curr->next;
                } else {
                    prev->next = curr->next;
                }
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        current = current->next;
    }
    
    free(friends_list);

    // Remove node from the AVL tree
    avl_root = avl_remove(avl_root, deleted_node.uid);

    return deleted_node;
}


// Release memory for all BST nodes and their lists of friends.
// Make bst_root points to an empty tree.
void destroy_bst_helper(struct record* node) {
    if (node == NULL) {
        return;
    }

    destroy_bst_helper(node->left);
    destroy_bst_helper(node->right);

    // Release the memory for the linked list of friends of this node
    struct list_records* cur = node->friends;
    while (cur != NULL) {
        struct list_records* next = cur->next;
        free(cur);
        cur = next;
    }

    // Release the memory for this node
    free(node);
}


void destroy_bst()
{
    destroy_bst_helper(bst_root);
    bst_root = NULL;
}


// Release memory for all AVL nodes and their lists of friends.
// Make avl_root points to an empty tree.
// Free memory for AVL node and its friends list recursively
void destroy_avl_helper(struct record* node) {
    if (node == NULL) {
        return;
    }
    destroy_avl_helper(node->left);
    destroy_avl_helper(node->right);
    // Free the friends list
    struct list_records* curr = node->friends;
    while (curr != NULL) {
        struct list_records* temp = curr;
        curr = curr->next;
        free(temp);
    }
    // Free the node
    free(node);
}

void destroy_avl() {
    destroy_avl_helper(avl_root);
    avl_root = NULL;
}


// Make users with uids uid1 and uid2 in the BST rooted at bst_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int make_friends_bst(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    // Find the records with uid1 and uid2 in the BST
    struct record *record1 = search_bst(uid1);
    struct record *record2 = search_bst(uid2);

    // Check if uid1 and uid2 are already friends
    struct list_records *curr = record1->friends;
    while (curr != NULL) {
        if (strcmp(curr->record->uid, uid2) == 0) {
            return 1;
        }
        curr = curr->next;
    }

    // Add uid2 to uid1's list of friends
    struct list_records *new_rec1 = malloc(sizeof(struct list_records));
    new_rec1->record = record2;
    new_rec1->next = record1->friends;
    record1->friends = new_rec1;

    // Add uid1 to uid2's list of friends
    struct list_records *new_rec2 = malloc(sizeof(struct list_records));
    new_rec2->record = record1;
    new_rec2->next = record2->friends;
    record2->friends = new_rec2;

    return 0;
}


// Make users with uids uid1 and uid2 in the AVL tree rooted at avl_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int make_friends_avl(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    // Search for the nodes with uid1 and uid2 in the AVL tree.
    struct record *node1 = search_avl(avl_root, uid1);
    struct record *node2 = search_avl(avl_root, uid2);

    // If either node is not found, return 1 to indicate failure.
    if (!node1 || !node2) {
        return 1;
    }

    // Check if node1 and node2 are already friends.
    struct list_records *friend = node1->friends;
    while (friend) {
        if (strcmp(friend->record->uid, uid2) == 0) {
            // node1 and node2 are already friends, so return 1 to indicate success.
            return 1;
        }
        friend = friend->next;
    }

    // Add node1 to node2's list of friends.
    struct list_records *new_friend = (struct list_records*)malloc(sizeof(struct list_records));
    new_friend->record = node1;
    new_friend->next = node2->friends;
    node2->friends = new_friend;

    // Add node2 to node1's list of friends.
    new_friend = (struct list_records*)malloc(sizeof(struct list_records));
    new_friend->record = node2;
    new_friend->next = node1->friends;
    node1->friends = new_friend;

    // Return 0 to indicate success.
    return 0;
}

