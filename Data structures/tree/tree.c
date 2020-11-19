#include "tree.h"
#include <stdio.h>

void tree_insert(node_t **root, uint8_t value)
{
    node_t *current = *root;

    if (current == NULL)
    {
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        new_node->data = value;
        new_node->right = NULL;
        new_node->left = NULL;
        *root = new_node;
        return;
    }

    if (current->data >= value)
    {
        tree_insert(&current->right, value);
    }
    else
    {
        tree_insert(&current->left, value);
    }
}

void tree_print(node_t *root)
{
    if (root != NULL)
    {
        tree_print(root->right);
        printf("%d ", root->data);
        tree_print(root->left);
    }
}
