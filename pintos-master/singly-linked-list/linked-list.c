#include <stdio.h>
#include <stdlib.h>
#include "linked-list.h"


void append( struct list_item *first, int x){
    struct list_item *next_item = malloc(sizeof *next_item + sizeof(int));
    next_item->value = x;
    next_item->next = NULL;
    
    if(first->next == NULL){
        first->next = next_item;
        return;
    }
    
    struct list_item *iter = first->next;
    while(iter->next != NULL){
        iter = iter->next;
    };
    iter->next = next_item;
};

void prepend(struct list_item *first, int x){
    struct list_item *next_item = malloc(sizeof *next_item + sizeof(int));
    next_item->value = x;
    next_item->next = first->next;
    first->next = next_item;
};

void print(struct list_item *first){
    if(first->next == NULL){
        printf("This list is empty\n");
        return;
    }
    struct list_item *iter = first->next;
    while(iter->next != NULL){
        printf("%d\n", iter->value);
        iter = iter->next;
    };
    printf("%d\n", iter->value);
};


void input_sorted(struct list_item *first, int x){
    struct list_item *iter = first;
    while(iter->next != NULL){
        if(iter->next->value > x){
            prepend(iter, x);
            return;
        };
        iter = iter->next;
    };
};

void clear(struct list_item *first){
    struct list_item *iter = first->next;
    while(iter->next != NULL){
        struct list_item *temp = iter;
        iter = iter->next;
        free(temp);
    }
    first->next = NULL;
    free(iter);
};

