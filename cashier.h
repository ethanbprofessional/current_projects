#include <stdio.h>
#include <stdlib.h>
struct nodeLinkedList {
    int processTime;
    struct nodeLinkedList *next;
};
struct nodeCashier {
    int number, linesize, currentsize;
    struct nodeCashier *next;
    struct nodeLinkedList *line;
};
// Points to head of Linked List
struct nodeLinkedList *headLinkedList = NULL;
struct nodeCashier *headCashier = NULL;
// Prints out values of Linked List inputs
int printCashier() {
    struct nodeCashier *pointerCashier = headCashier;
    struct nodeLinkedList *pointerList = headCashier->line;
    while (pointerCashier != NULL)
    {
        printf("\nCashier #%d Linesize %d Current Size %d: ", pointerCashier->number, pointerCashier->linesize, pointerCashier->currentsize);
        printf("[");
        while (pointerList != NULL)
        {
            printf(" %d ", pointerList->processTime);
            pointerList = pointerList->next;
        }
        printf("]");
        if (pointerCashier->next == NULL) {return 0;}
        pointerCashier = pointerCashier->next;
        pointerList = pointerCashier->line;
    }
    return 0;
}
// Inserts new node at the end of Linked List
int insertAtLineEnd(int lineNumber, int processTime) {
    struct nodeLinkedList *link = (struct nodeLinkedList*) malloc(sizeof(struct nodeLinkedList));
    link->processTime = processTime;
    struct nodeCashier *pointerCashier = headCashier;
    // Moves to correct cashier
    while (lineNumber != pointerCashier->number) {
        if (pointerCashier == NULL) {
            printf("\nThere is no cashier of that number: #%d.", pointerCashier->number);
            return 0;
        }
        pointerCashier = pointerCashier->next;
    }
    if (pointerCashier->line == NULL) {
        link->next = NULL;
        pointerCashier->line = link;
        pointerCashier->currentsize++;
        return 0;
    } 
    // Moves to end of line
    headLinkedList = pointerCashier->line;
    while (headLinkedList->next != NULL) {
        headLinkedList = headLinkedList->next;
    }
    pointerCashier->currentsize++;
    if (pointerCashier->currentsize > pointerCashier->linesize) {pointerCashier->currentsize--;return 1;}    
    headLinkedList->next = link;
    link->next = NULL;
    return 0;
}
// Creates Cashiers
int insertCashier(int number) {
    struct nodeCashier *link = (struct nodeCashier*) malloc(sizeof(struct nodeCashier));
    link->number = number;
    struct nodeCashier *pointerCashier = headCashier;
    if (pointerCashier == NULL) {
        link->next = headCashier;
        link->line = NULL;
        headCashier = link;
        return 0;
    }
    while (pointerCashier->next != NULL) {
        if (pointerCashier->number == number) {
            printf("\nThis Cashier already exists: %d.", pointerCashier->number);
            return 0;
        }
        pointerCashier = pointerCashier->next;
    }
    pointerCashier->next = link;
    link->next = NULL;
    link->line = NULL;
    return 0;
}
int deleteatbeginning(int lineNumber) {
    struct nodeCashier *pointerCashier = headCashier;
    while (lineNumber != pointerCashier->number) {
        if (pointerCashier->next == NULL) {
            printf("There is no cashier of number %d.", lineNumber);
            return 0;
        }
        pointerCashier = pointerCashier->next;
    }
    struct nodeLinkedList *storage = pointerCashier->line;
    pointerCashier->line = pointerCashier->line->next;
    pointerCashier->currentsize--;
    free(storage);
}