

#include "nd_core.h"

static ND_INLINE void nd_rbtree_left_rotate(nd_rbtree_node_t **root,
    nd_rbtree_node_t *sentinel, nd_rbtree_node_t *node);
static ND_INLINE void nd_rbtree_right_rotate(nd_rbtree_node_t **root,
    nd_rbtree_node_t *sentinel, nd_rbtree_node_t *node);


void
nd_rbtree_insert(nd_thread_volatile nd_rbtree_t *tree,
    nd_rbtree_node_t *node)
{
    nd_rbtree_node_t  **root, *temp, *sentinel;

    /* a binary tree insert */

    root = (nd_rbtree_node_t **) &tree->root;
    sentinel = tree->sentinel;

	tree->size++;

    if (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        nd_rbt_black(node);
        *root = node;

        return;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    while (node != *root && nd_rbt_is_red(node->parent)) {

        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            if (nd_rbt_is_red(temp)) {
                nd_rbt_black(node->parent);
                nd_rbt_black(temp);
                nd_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    nd_rbtree_left_rotate(root, sentinel, node);
                }

                nd_rbt_black(node->parent);
                nd_rbt_red(node->parent->parent);
                nd_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } else {
            temp = node->parent->parent->left;

            if (nd_rbt_is_red(temp)) {
                nd_rbt_black(node->parent);
                nd_rbt_black(temp);
                nd_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    nd_rbtree_right_rotate(root, sentinel, node);
                }

                nd_rbt_black(node->parent);
                nd_rbt_red(node->parent->parent);
                nd_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    nd_rbt_black(*root);
}


void
nd_rbtree_insert_value(nd_rbtree_node_t *temp, nd_rbtree_node_t *node,
    nd_rbtree_node_t *sentinel)
{
    nd_rbtree_node_t  **p;

    for ( ;; ) {

        p = (node->key < temp->key) ? &temp->left : &temp->right;

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    nd_rbt_red(node);
}


void
nd_rbtree_insert_seq_value(nd_rbtree_node_t *temp, nd_rbtree_node_t *node,
    nd_rbtree_node_t *sentinel)
{
    nd_rbtree_node_t  **p;

    for ( ;; ) {

        p = ((nd_int32_t)(node->key - temp->key) < 0)
            ? &temp->left : &temp->right;

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    nd_rbt_red(node);
}


void
nd_rbtree_delete(nd_thread_volatile nd_rbtree_t *tree,
    nd_rbtree_node_t *node)
{
    nd_uint_t           red;
    nd_rbtree_node_t  **root, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

	tree->size--;

    root = (nd_rbtree_node_t **) &tree->root;
    sentinel = tree->sentinel;

    if (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } else {
        subst = nd_rbtree_min(node->right, sentinel);

        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }

    if (subst == *root) {
        *root = temp;
        nd_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        return;
    }

    red = nd_rbt_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;

    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {

        temp->parent = subst->parent;

    } else {

        if (subst->parent == node) {
            temp->parent = subst;

        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        nd_rbt_copy_color(subst, node);

        if (node == *root) {
            *root = subst;

        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if (red) {
        return;
    }

    /* a delete fixup */

    while (temp != *root && nd_rbt_is_black(temp)) {

        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (nd_rbt_is_red(w)) {
                nd_rbt_black(w);
                nd_rbt_red(temp->parent);
                nd_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            if (nd_rbt_is_black(w->left) && nd_rbt_is_black(w->right)) {
                nd_rbt_red(w);
                temp = temp->parent;

            } else {
                if (nd_rbt_is_black(w->right)) {
                    nd_rbt_black(w->left);
                    nd_rbt_red(w);
                    nd_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                nd_rbt_copy_color(w, temp->parent);
                nd_rbt_black(temp->parent);
                nd_rbt_black(w->right);
                nd_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }

        } else {
            w = temp->parent->left;

            if (nd_rbt_is_red(w)) {
                nd_rbt_black(w);
                nd_rbt_red(temp->parent);
                nd_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            if (nd_rbt_is_black(w->left) && nd_rbt_is_black(w->right)) {
                nd_rbt_red(w);
                temp = temp->parent;

            } else {
                if (nd_rbt_is_black(w->left)) {
                    nd_rbt_black(w->right);
                    nd_rbt_red(w);
                    nd_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                nd_rbt_copy_color(w, temp->parent);
                nd_rbt_black(temp->parent);
                nd_rbt_black(w->left);
                nd_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    nd_rbt_black(temp);
}


static ND_INLINE void
nd_rbtree_left_rotate(nd_rbtree_node_t **root, nd_rbtree_node_t *sentinel,
    nd_rbtree_node_t *node)
{
    nd_rbtree_node_t  *temp;

    temp = node->right;
    node->right = temp->left;

    if (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->left) {
        node->parent->left = temp;

    } else {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}


static ND_INLINE void
nd_rbtree_right_rotate(nd_rbtree_node_t **root, nd_rbtree_node_t *sentinel,
    nd_rbtree_node_t *node)
{
    nd_rbtree_node_t  *temp;

    temp = node->left;
    node->left = temp->right;

    if (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->right) {
        node->parent->right = temp;

    } else {
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
}
