#ifndef TREE_H
#define TREE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct node
{
    uint8_t data;
    struct node *left;
    struct node *right;
} node_t;

void tree_print(node_t *root);
void tree_insert(node_t **root, uint8_t value);

// Search for a value in the tree
// Delete a node if exists
// Edit a node if exists

#endif