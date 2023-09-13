#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cashier.h"
#include "heaps.h"
void supermarketinit();
int supermarketsim();
bool checkIfSomeOpen();
bool checkIfAllEmpty();
void selectLine();
void iterateLine();
int arrival_time[20] = {1,2,5,6,7,8,10,13,14,15,
                        16,19,22,24,25,26,27,28,30,32};
int process_time[20] = {1,3,4,5,6,7,9,10,12,13,
                        15,16,18,19,21,22,24,25,27,30};
int time = 1;
int main() {
    supermarketinit();
    return 0;
}
void supermarketinit() {
    // Creates Cahsiers and adds arrival time to minheap
    for (int i = 1; i < 6; i++) {insertCashier(i);}
    struct nodeCashier *pointerCashier = headCashier;
    for (int i = 1; i < 6; i++) {pointerCashier->linesize = 1;pointerCashier->currentsize = 0;pointerCashier = pointerCashier->next;}
    for (int i = 6; i < 9; i++) {insertCashier(i);}
    pointerCashier = headCashier;
    for (int i = 1; i < 6; i++) {pointerCashier = pointerCashier->next;}
    for (int i = 6; i < 9; i++) {pointerCashier->linesize = 2;pointerCashier->currentsize = 0;pointerCashier = pointerCashier->next;}
    for (int i = 9; i < 11; i++) {insertCashier(i);}
    pointerCashier = headCashier;
    for (int i = 1; i <  9; i++) {pointerCashier = pointerCashier->next;}
    for (int i = 9; i < 11; i++) {pointerCashier->linesize = 3;pointerCashier->currentsize = 0;pointerCashier = pointerCashier->next;}
    for (int i = 0; i < 20; i++) {minHeap(arrival_time[i],process_time[i]);}
    supermarketsim();
}
int supermarketsim() {
    while ((root != NULL) || !checkIfAllEmpty()) {
        if (checkIfSomeOpen() && (root != NULL)) {selectLine();}
        if (!checkIfAllEmpty()) {iterateLine();}
        time++;
    }
    printHeap();
    printCashier();
    printf("\nFinal Time: %d",time);    
    return 0;
}
bool checkIfAllEmpty() {
    struct nodeCashier *pointer = headCashier;
    bool isEmpty = true;
    for (int i = 0; i < 10; i++) {
        if (pointer->currentsize == 0) {pointer = pointer->next; continue;}
        isEmpty = false;
        break;
    }
    return isEmpty;
}
bool checkIfSomeOpen() {
    struct nodeCashier *pointer = headCashier;
    bool isEmpty = false;
    for (int i = 0; i < 10; i++) {
        if (pointer->currentsize >= pointer->linesize) {pointer = pointer->next; continue;}
        isEmpty = true;
        break;
    }
    return isEmpty;
}
void selectLine() {
    struct nodeCashier *pointer = headCashier;
    while(checkIfSomeOpen() && (root != NULL)) {
        if ((root->arrivalTime >= time) && (pointer->currentsize < pointer->linesize)) {insertAtLineEnd(pointer->number, root->processTime);deleteFromMinHeap();}
        if ((root != NULL) && (time < root->arrivalTime)) {break;}
        pointer = pointer->next;
        if (pointer->next == NULL) {pointer = headCashier;}   
    }
}
void iterateLine() {
    struct nodeCashier *pointer = headCashier;
    for (int i = 0; i < 10; i++) {
        if (pointer->line == NULL) {pointer = pointer->next;continue;}
        pointer->line->processTime--;
        if (pointer->line->processTime == 0) {deleteatbeginning(i);}
        pointer = pointer->next;
    }
}