#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Creates a node for Linked List
struct heapnode {
    int arrivalTime, processTime;
    struct heapnode *left, *right;
};
struct storenode {
    struct heapnode *data;
    struct storenode *next;
};
typedef struct storenode node_s;
typedef struct heapnode node_t;
node_t *root = NULL;
node_s *headpointer = NULL;
int sortMinHeap(node_t *pointer) {
    bool leftNULL = (pointer->left == NULL);
    bool rightNULL = (pointer->right == NULL);
    if(leftNULL && rightNULL) {return 0;}
    if (!leftNULL){sortMinHeap(pointer->left);}
    if (!rightNULL) {sortMinHeap(pointer->right);}
    if (!leftNULL && (pointer->left->arrivalTime < pointer->arrivalTime)) {
        int temparrivalTime = pointer->left->arrivalTime;
        int tempProcessTime = pointer->left->processTime;
        pointer->left->arrivalTime = pointer->arrivalTime;
        pointer->left->processTime = pointer->processTime;
        pointer->arrivalTime = temparrivalTime;
        pointer->processTime = tempProcessTime;
        sortMinHeap(pointer->left);
    }
    if (!rightNULL && (pointer->right->arrivalTime < pointer->arrivalTime)) {
        int tempArrivalTime = pointer->right->arrivalTime;
        int tempProcessTime = pointer->right->processTime;
        pointer->right->arrivalTime = pointer->arrivalTime;
        pointer->right->processTime = pointer->processTime;
        pointer->arrivalTime = tempArrivalTime;
        pointer->processTime = tempProcessTime;
        sortMinHeap(pointer->right);
    }
    return 0;
}
// Inserts new pointer towards specific node to the end of Linked List
node_s * insertpointer(node_t *pointer, node_s *headpointer) {
    node_s *link = (node_s*) malloc(sizeof(node_s));
    link->data = pointer;
    node_s *tempPoint = headpointer;
    if (tempPoint == NULL) {
        link->next = headpointer;
        headpointer = link;
        return headpointer;
    } 
    while (tempPoint->next != NULL)
        tempPoint = tempPoint->next;
    tempPoint->next = link;
    link->next = NULL;
    return headpointer;
}
node_s * deletepointer(node_s *headpointer) {
    headpointer = headpointer->next;
    return headpointer;
}
int helperDeleteFromMinHeap(node_t *pointer, int arrivalTime) {
    if (pointer->left != NULL) {headpointer = insertpointer(pointer->left, headpointer);}
    if (pointer->right != NULL) {headpointer = insertpointer(pointer->right, headpointer);}
    if (root->left->left == NULL) {
        if ((root->right != NULL) && (root->right->left == NULL)) {
           pointer = root;
           arrivalTime = pointer->arrivalTime;
           free(root->right);
           root->right = NULL;
           return arrivalTime;
        }   
        pointer = root;
        arrivalTime = pointer->arrivalTime;
        free(root->left);
        root->left = NULL;    
        return arrivalTime;
    }
    if (headpointer != NULL) {
        pointer = headpointer->data;
        headpointer = deletepointer(headpointer);
    }
    if (headpointer == NULL) {
        pointer->left = root->left;
        pointer->right = root->right;
        arrivalTime = pointer->arrivalTime;
        root->left = root->right = NULL;
        free(root);
        root = pointer;
        return arrivalTime;
    }
    arrivalTime = helperDeleteFromMinHeap(pointer, arrivalTime);
    if ((pointer->left != NULL) && (pointer->left->arrivalTime == arrivalTime)) {pointer->left = NULL;}
    if ((pointer->right != NULL) && (pointer->right->arrivalTime == arrivalTime)) {pointer->right = NULL;}
    return arrivalTime;
}
int deleteFromMinHeap() {
    if (root == NULL) {return 0;}
    if ((root->left == NULL) && (root->right == NULL)) {
        int arrivalTime =  root->arrivalTime;
        free(root);
        root = NULL;
        return arrivalTime;
    }
    int arrivalTime = helperDeleteFromMinHeap(root, root->arrivalTime);
    sortMinHeap(root);
    return arrivalTime;
}
// Stores pointers to nodes when traversing heap FIFO
int makeMinHeap(node_t *pointer, node_t *link) {
    if (pointer->left == NULL) {
        pointer->left = link;
        sortMinHeap(root);
        return 0;
    }
    headpointer = insertpointer(pointer->left, headpointer);
    if (pointer->right == NULL) {
        pointer->right = link;
        sortMinHeap(root);
        return 0;
    }
    headpointer = insertpointer(pointer->right, headpointer);
    pointer = headpointer->data;
    headpointer = deletepointer(headpointer);
    makeMinHeap(pointer, link);
    return 0;
}
int minHeap(int arrivalTime, int processTime) {
    node_t *link = (node_t*) malloc(sizeof(node_t));
    node_t *pointer = root;
    link->arrivalTime = arrivalTime;
    link->processTime = processTime;
    link->left = link->right = NULL;
    if (root == NULL) {root = link;return 0;}
    headpointer = NULL;
    makeMinHeap(pointer, link);
    return 0;
}
void helperPrintHeap(node_t *pointer) {
    if (pointer == root) {printf("[ %d ", pointer->arrivalTime);}
    if (pointer->left != NULL) {
        printf(" %d ", pointer->left->arrivalTime);
        headpointer = insertpointer(pointer->left, headpointer);
    }
    if (pointer->right != NULL) {
        printf(" %d ", pointer->right->arrivalTime);
        headpointer = insertpointer(pointer->right, headpointer);
    }
    if (headpointer != NULL) {
        pointer = headpointer->data;
        headpointer = deletepointer(headpointer);
        helperPrintHeap(pointer);
    }
}
int printHeap() {
    if (root == NULL) {printf("\nThere are no nodes in heap."); return 1;}
    node_t *pointer = root;
    headpointer = NULL;
    helperPrintHeap(pointer);  
    printf("]");
    return 0;
}