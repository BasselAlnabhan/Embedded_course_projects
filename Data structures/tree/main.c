#include "tree.h"
#include <stdio.h>

int main(void)
{
    node_t *tree = NULL;

    tree_insert(&tree, 10);
    tree_insert(&tree, 20);
    tree_insert(&tree, 5);
    tree_insert(&tree, 10);
    tree_insert(&tree, 30);
    tree_insert(&tree, 1);

    tree_print(tree);
    printf("\n\n");

    return EXIT_SUCCESS;
}
