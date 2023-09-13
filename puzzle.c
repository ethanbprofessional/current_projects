#include "puzzle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
int currentPosition, endPosition, distanceFromEnd;
char *storePuzzle;
time_t toGenerate; // $ = wall, % = temporary wall, * = empty space, # = beginning point, & = end point and - = correct path
struct nodeLinkedList {
    int position, y_distance, x_distance;
    struct nodeLinkedList *next;
};
node_l *head = NULL;
int main(int argc, char const *argv[]) {
    srand((unsigned) time(&toGenerate));
    generateMap();
    return 0;
}
void printList() {
    node_l *pointer = head;
    printf("\n[");
    while (pointer != NULL) {
        printf(" (%d,%d) ", pointer->x_distance, pointer->y_distance);
        pointer = pointer->next;
    }
    printf("]");
}
int deleteatbeginning() {
    if (head == NULL) return -1;
    int tempData = head->position;
    node_l *pointer = head;
    head = head->next;
    free(pointer);
    return tempData;
}
int findinlist(int data) {
    node_l *tempPoint = head;
    if (head == NULL) return 0;
    while (tempPoint->position != data) {
        if (tempPoint->next == NULL) break;
        tempPoint = tempPoint->next;
    }
    if (tempPoint->position != data) return 0;
    return 1;
}
int insertatend(int position, int width) {
    node_l *link = (node_l*) malloc(sizeof(node_l));
    link->position = position;
    int count = 0;
    int target;
    if (position > endPosition) {
        distanceFromEnd = endPosition;
        target = position;
    } else {
        distanceFromEnd = position;
        target = endPosition;
    } 
    while (distanceFromEnd / width < target / width) {
        distanceFromEnd += width;
        count++;
    }
    link->y_distance = count;
    printf("Current Position: %d, End Position: %d, Target: %d Count: %d", position, endPosition, target, count);
    if (distanceFromEnd > target) link->x_distance = (distanceFromEnd - target + 1);
    if (distanceFromEnd < target) link->x_distance = (target - distanceFromEnd + 1);
    if (distanceFromEnd == target) link->x_distance = 0;
    Sleep(10);
    node_l *tempPoint = head;
    if (tempPoint == NULL) {
        link->next = head;
        head = link;
        return 0;
    } 
    while (tempPoint->next != NULL)
        tempPoint = tempPoint->next;
    tempPoint->next = link;
    link->next = NULL;
    return 0;
}
int sortList() {
    int sum1, sum2;
    node_l *tempPoint = head;
    if (head == NULL) return 0;
    while (tempPoint->next != NULL) {
        sum1 = tempPoint->x_distance + tempPoint->y_distance;
        sum2 = tempPoint->next->x_distance + tempPoint->next->y_distance;
        if (sum2 < sum1) {
            int tempData = tempPoint->next->y_distance;
            tempPoint->next->y_distance = tempPoint->y_distance;
            tempPoint->y_distance = tempData;
            tempData = tempPoint->next->x_distance;
            tempPoint->next->x_distance = tempPoint->x_distance;
            tempPoint->x_distance = tempData;
            tempData = tempPoint->next->position;
            tempPoint->next->position = tempPoint->position;
            tempPoint->position = tempData;
            tempPoint = head;
            continue;
        }
        tempPoint = tempPoint->next;
    }
}
void printPuzzleClear(char puzzleFrame[], int width, int height) {
    char map[width + 4];
    memset(map, '\0', sizeof(map));
    for (int i = 0; i < (width - 1); i++) strcat(map, " ");
    strcat(map, "Map:");
    printf("%s\n", map);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) printf(" %c", puzzleFrame[i * width + j]);
        printf("\n");  
    }
}
void generateMap() {
    int width, height;
    char storeWidth[4], storeHeight[4], *puzzleFrame;
    printf("\nPlease input a width for the board greater or equal to 18: ");
    fgets(storeWidth, 4, stdin);
    width = atoi(storeWidth);
    while (width <= 17) {
        printf("\nThe width is too small, or you entered an invalid character. The paramaters must both be 18 or greater: ");
        fgets(storeWidth, 4, stdin);
        width = atoi(storeWidth);
    }
    printf("\nPlease input a height for the board greater or equal to 18: ");
    fgets(storeHeight, 4, stdin);
    height = atoi(storeHeight);
    while (height <= 17) {
        printf("\nThe height is too small, or you entered an invalid character. The paramaters must both be 18 or greater: ");
        fgets(storeHeight, 4, stdin);
        height = atoi(storeHeight);
    }
    width += 2;
    height += 2;
    puzzleFrame = (char*) malloc(width * height * sizeof(char));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) 
            puzzleFrame[i * width + j] = '*';
    printPuzzleClear(puzzleFrame, width, height);
    filterEdge(puzzleFrame, width, height, 0, '-');
    printPuzzleClear(puzzleFrame, width, height);
    puzzleFrame = generateStartEnd(puzzleFrame, width, height, '#');
    printPuzzleClear(puzzleFrame, width, height); 
    puzzleFrame = generateStartEnd(puzzleFrame, width, height, '&');
    printPuzzleClear(puzzleFrame, width, height);
    filterEdge(puzzleFrame, width, height, 1, '%');
    printPuzzleClear(puzzleFrame, width, height);
    generateBranches(puzzleFrame, width, height);
    printPuzzleClear(puzzleFrame, width, height);
    puzzleFrame = removeWideSpace(puzzleFrame, width, height);
    printPuzzleClear(puzzleFrame, width, height); 
    free(puzzleFrame);
    free(storePuzzle);
}
char * generateStartEnd(char puzzleFrame[], int width, int height, char point) {
    int saveRandom; // 0 = Top Row, 1 = Left Collumn, 2 = Bottom Row, 3 = Right Collumn
    SAMEAREA:
    switch ((rand() % 4)) {
    
    case 0:
        saveRandom = (rand() % width);
        while ((saveRandom <= 1) || (saveRandom >= (width - 2))) saveRandom = (rand() % width);
        if (puzzleFrame[saveRandom] == '#') goto SAMEAREA;
        puzzleFrame[saveRandom] = point;
        saveRandom += width;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    case 1:
        saveRandom = (rand() % height);
        while ((saveRandom <= 1) || (saveRandom >= (height - 2))) saveRandom = (rand() % height);
        saveRandom *= width;
        if (puzzleFrame[saveRandom] == '#') goto SAMEAREA;
        puzzleFrame[saveRandom] = point;
        saveRandom ++;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    case 2:
        saveRandom = (rand() % width);
        while ((saveRandom <= 1) || (saveRandom >= (width - 2))) saveRandom = (rand() % width);
        saveRandom = (width * (height - 1)) + saveRandom;
        if (puzzleFrame[saveRandom] == '#') goto SAMEAREA;
        puzzleFrame[saveRandom] = point;
        saveRandom -= width;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    case 3:
        saveRandom = (rand() % height);
        while ((saveRandom <= 1) || (saveRandom >= (height - 2))) saveRandom = (rand() % height);
        saveRandom = (width - 1) + (saveRandom * width);
        if (puzzleFrame[saveRandom] == '#') goto SAMEAREA;
        puzzleFrame[saveRandom] = point;
        saveRandom--;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    }
}
void savePosition(char point, int position, int width) {
    switch (point) {
    
    case '#':
        currentPosition = position;
        break;
    case '&':
        endPosition = position;
        break;
    }
    printf("\ncurrentPosition: %d \nendPosition: %d\n", currentPosition, endPosition);
}
void filterEdge(char puzzleFrame[], int width, int height, int startingIndex, char point) {
    for (int leftColumn = startingIndex; leftColumn < (height - startingIndex); leftColumn++) {
        if (puzzleFrame[leftColumn * width + startingIndex] == '-') continue; 
        puzzleFrame[leftColumn * width + startingIndex] = point;
    } 
    for (int bottomRow = startingIndex; bottomRow < (width - startingIndex); bottomRow++) {
        if (puzzleFrame[width * (height - startingIndex - 1) + bottomRow] == '-') continue; 
        puzzleFrame[(width * (height - startingIndex - 1)) + bottomRow] = point;
    }
    for (int topRow = startingIndex; topRow < (width - startingIndex); topRow++) {
        if (puzzleFrame[topRow + (width * startingIndex)] == '-') continue; 
        puzzleFrame[topRow + (width * startingIndex)] = point;
    }
    for (int rightCollumn = startingIndex; rightCollumn < (height - startingIndex); rightCollumn++) {
        if (puzzleFrame[width - startingIndex - 1 + (rightCollumn * width)] == '-') continue;
        puzzleFrame[width - startingIndex - 1 + (rightCollumn * width)] = point;
    }
} 
int generateSolution( char * puzzleFrame, int width, int height) {
    int choices[4];
    printf("YA START: %d", findSolution(puzzleFrame, width, height));
    while (findSolution(puzzleFrame, width, height) == 0) {
        int tempData = deleteatbeginning();
        for (int i = -1; i < 2; i += 2) {
            if (puzzleFrame[tempData + i] == '$') {
                puzzleFrame[tempData + i] = '*';
                break;
            }
            if (puzzleFrame[tempData + (width * i)] == '$') {
                puzzleFrame[tempData + (width * i)] = '*';
                break;
            }
        }
        printList();
        printPuzzleClear(puzzleFrame, width, height);
    }
    printf("YA DUN");
    return 0;
}
int findSolution(char * puzzleFrame, int width, int height) { // Checks for spot in this order: Left, Up, Right, Down
    int count = 0;
    if (currentPosition == endPosition) {
        storePuzzle = (char*) malloc(sizeof(puzzleFrame));
        memset(storePuzzle, '\0', sizeof(storePuzzle));
        strcpy(storePuzzle, puzzleFrame);
        printList();
        printPuzzleClear(storePuzzle, width, height);
        return 1;
    }
    for (int i = -1; i < 2; i += 2) {
        int position = currentPosition + i;
        if ((puzzleFrame[position] == '*') || (puzzleFrame[position] == '&') || position == endPosition) {
            if (puzzleFrame[position] != '&') puzzleFrame[position] = '-';
            currentPosition += i;
            findSolution(puzzleFrame, width, height);
            currentPosition -= i;
            if (puzzleFrame[position] != '&') puzzleFrame[position] = '*';
        }
        position = currentPosition + (width * i);
        if ((puzzleFrame[position] == '*') || (puzzleFrame[position] == '&') || position == endPosition) {
            if (puzzleFrame[position] != '&') puzzleFrame[position] = '-';
            currentPosition += width * i;
            findSolution(puzzleFrame, width, height);
            currentPosition -= width * i;
            if (puzzleFrame[position] != '&') puzzleFrame[position] = '*';
        }
    }
    for (int i = -1; i < 2; i += 2) {
        if (puzzleFrame[currentPosition + i] == '*') count++;
        if (puzzleFrame[currentPosition + (width * i)] == '*') count++;
    }
    printList();
    if (count == 0 && findinlist(currentPosition) == 0) {
        insertatend(currentPosition, width);
        sortList();
    }
    return 0;
}
int generateBranches(char * puzzleFrame, int width, int height) { 
    int widthQuadrantAmount = (width - 4) / 2;
    int heightQuadrantAmount = (height - 4) / 2;
    int quadrantPosition = 2 * width;
    for (int i = 0; i < heightQuadrantAmount; i++) {
        for (int i = 0; i < widthQuadrantAmount; i++) puzzleFrame = generateBranchesHelper(puzzleFrame, width, height, &quadrantPosition);
        quadrantPosition += width + (width % 2) + 4;
    }
}
char * generateBranchesHelper(char * puzzleFrame, int width, int height, int * quadrantPosition) {
    int pickRowOrColumn = rand() % 2;
    int saveRandomRow = rand() % 2;
    int saveRandomColumn = rand() % 2;
    *quadrantPosition += 2;
    switch (pickRowOrColumn) { // Row = 0, Column = 1
    
    case 0:
        if (saveRandomRow == 0) {
            for (int i = 0; i < 2; i++) puzzleFrame[*quadrantPosition + i] = '$';
            break;
        }
        for (int i = 0; i < 2; i++) puzzleFrame[*quadrantPosition + width + i] = '$';
        break;
    
    case 1:
        if(saveRandomColumn == 0) {
            for (int i = 0; i < 2; i++) puzzleFrame[*quadrantPosition + (width * i)] = '$';
            break;
        }
        for (int i = 0; i < 2; i++) puzzleFrame[*quadrantPosition + 1 + (width * i)] = '$';
        break;
    }
    return puzzleFrame;
}
char * removeWideSpace(char * puzzleFrame, int width, int height) {
    int widthQuadrantAmount = width - 4;
    int heightQuadrantAmount = height - 4;
    int quadrantPosition = 2 * width + 1;
    for (int i = 1; i < heightQuadrantAmount; i++) {
        for (int j = 1; j < widthQuadrantAmount; j++) {
            quadrantPosition += 1;
            if (puzzleFrame[quadrantPosition] != '*' || puzzleFrame[quadrantPosition + 1] != '*') continue;
            if (puzzleFrame[quadrantPosition + width] != '*' || puzzleFrame[quadrantPosition + width + 1] != '*') continue;
            puzzleFrame = removeWideSpaceHelper(puzzleFrame, quadrantPosition, width);
        }
        quadrantPosition += 5;
    }
    return puzzleFrame;
}
char * removeWideSpaceHelper(char * puzzleFrame, int quadrantPosition, int width) {
    switch (rand() % 4) { // 0 = Up Left, 1 = Up Right, 2 = Down Left, 3 = Down Right

    case 0:
        puzzleFrame[quadrantPosition] = '$';
        break;
    case 1:
        puzzleFrame[quadrantPosition + 1] = '$';
        break;
    case 2:
        puzzleFrame[quadrantPosition + width] = '$';
        break;
    case 3:
        puzzleFrame[quadrantPosition + width + 1] = '$';
        break;        
    }
    return puzzleFrame;
}
// Checks for spot in this order: Up, Left, Down, Right
int solvePuzzle(char * puzzleFrame, int width, int height) {
    if (puzzleFrame[0] == '&') {printPuzzleClear(puzzleFrame, width, height); return 0;}
    for (int i = -1; i < 2; i += 2) {
        if ((puzzleFrame[0 + i] == '-') || (puzzleFrame[0] == '&')) {

        }
        if ((puzzleFrame[0] == '-') || (puzzleFrame[0] == '&')){
            
        }
    }
    return 0;
}