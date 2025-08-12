#include <stdio.h>
#include "linked-list.h"

int main (int argc, char ** argv)
{
    struct list_item root;
    root.value = -1;
    root.next = NULL;

    append(&root, 10);
    print(&root);
    printf("Trying append: \n");
    append(&root, 100);
    append(&root, 1000);
    append(&root, 10000);
    append(&root, 100000);
    print(&root);
    printf("Trying prepend: \n");
    prepend(&root, 100);
    prepend(&root, 1000);
    prepend(&root, 10000);
    prepend(&root, 100000);
    print(&root);
    printf("Trying clear: \n");
    clear(&root);
    print(&root);
    printf("Trying input_sorted(setup): \n");
    append(&root, 10);
    append(&root, 5);
    append(&root, 1);
    append(&root, 15);
    print(&root);
    printf("Trying input_sorted: \n");
    input_sorted(&root, 4);
    input_sorted(&root, 13);
    input_sorted(&root, 12);
    print(&root);
    clear(&root);

    return 0;
}
