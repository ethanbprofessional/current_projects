#include "puzzle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/**
 * Maze Generator
 * Created By: Ethan Bown
 * Last Edited: 10/12/23
 * Purpose: To randomly create a maze to the best possible degree based on the user's desired size
 * Should always create a maze that is solvable, but the difficulty of those mazes is random
 * Saves the maze generated to 'savePuzzles.txt' file
 * 
*/
int currentPosition, endPosition, distanceFromEnd;
time_t toGenerate; // $ = wall, % = temporary wall, * = empty space, # = beginning point, & = end point and - = correct path
int main(int argc, char const *argv[]) {
    srand((unsigned) time(&toGenerate));
    generateMap();
    return 0;
}
void printPuzzleClear(char * puzzleFrame, int width, int height) { //Prints puzzle to terminal
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
void printPuzzleSave(char * puzzleFrame, int width, int height) { // Saves puzzle in a text file
    FILE *fp;
    fp = fopen("savePuzzles.txt", "w");
    char map[width + 4];
    memset(map, '\0', sizeof(map));
    for (int i = 0; i < (width - 1); i++) strcat(map, " ");
    strcat(map, "Map:");
    fprintf(fp, "%s\n", map);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) fprintf(fp ," %c", puzzleFrame[i * width + j]);
        fputs("\n", fp);  
    }
    fclose(fp);
}
void generateMap() { // Invokes functions to generate map
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
    for (int i = 0; i < (height * width); i++) puzzleFrame[i] = '*';
    filterEdge(puzzleFrame, width, height, 0, '-');
    generateStartEnd(puzzleFrame, width, height, '#');
    generateStartEnd(puzzleFrame, width, height, '&');
    filterEdge(puzzleFrame, width, height, 1, '%');
    generateBranches(puzzleFrame, width, height);
    removeWideSpace(puzzleFrame, width, height);
    generateSolution(puzzleFrame, width, height);
    smartRemoveWideSpace(puzzleFrame, width, height);
    freeExits(puzzleFrame, width, height, currentPosition);
    freeExits(puzzleFrame, width, height, endPosition);
    printPuzzleSave(puzzleFrame, width, height);
    free(puzzleFrame);
}
char * generateStartEnd(char * puzzleFrame, int width, int height, char point) { // Adds a start and end position to maze
    int saveRandom; // 0 = Top Row, 1 = Left Collumn, 2 = Bottom Row, 3 = Right Collumn
    switch ((rand() % 4)) {
    
    case 0:
        saveRandom = (rand() % width);
        while ((saveRandom <= 1) || (saveRandom >= (width - 2))) saveRandom = (rand() % width);
        if (puzzleFrame[saveRandom] == '#') generateStartEnd(puzzleFrame, width, height, point);
        puzzleFrame[saveRandom] = point;
        saveRandom += width;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    case 1:
        saveRandom = (rand() % height);
        while ((saveRandom <= 1) || (saveRandom >= (height - 2))) saveRandom = (rand() % height);
        saveRandom *= width;
        if (puzzleFrame[saveRandom] == '#') generateStartEnd(puzzleFrame, width, height, point);
        puzzleFrame[saveRandom] = point;
        saveRandom ++;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    case 2:
        saveRandom = (rand() % width);
        while ((saveRandom <= 1) || (saveRandom >= (width - 2))) saveRandom = (rand() % width);
        saveRandom = (width * (height - 1)) + saveRandom;
        if (puzzleFrame[saveRandom] == '#') generateStartEnd(puzzleFrame, width, height, point);
        puzzleFrame[saveRandom] = point;
        saveRandom -= width;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    case 3:
        saveRandom = (rand() % height);
        while ((saveRandom <= 1) || (saveRandom >= (height - 2))) saveRandom = (rand() % height);
        saveRandom = (width - 1) + (saveRandom * width);
        if (puzzleFrame[saveRandom] == '#') generateStartEnd(puzzleFrame, width, height, point);
        puzzleFrame[saveRandom] = point;
        saveRandom--;
        puzzleFrame[saveRandom] = '-';
        savePosition(point, saveRandom, width);
        return puzzleFrame;
    }
}
void savePosition(char point, int position, int width) { // Changes global variables to store current and end position
    switch (point) {
    
    case '#':
        currentPosition = position;
        break;
    case '&':
        endPosition = position;
        break;
    }
}
void filterEdge(char * puzzleFrame, int width, int height, int startingIndex, char point) { // Changes edge of puzzle to desired characters
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
char * generateSolution( char * puzzleFrame, int width, int height) { // Begins process of creating a solution to the puzzle
    int quadrantPosition = 2 * width;
    for (int i = 0; i < ((height - 4) / 2); i++) {
        for (int j = 0; j < ((width - 4) / 2); j++) {
            quadrantPosition += 2;
            puzzleFrame = findSolution(puzzleFrame, width, height, quadrantPosition);
        }
        quadrantPosition += width + 4;
    }
    quadrantPosition = 2 * width + 2;
    for (int i = 0; i < (height - 4); i++) {
        for (int j = 0; j < (width - 4); j++) {
            puzzleFrame = removeAtSign(puzzleFrame, width, height, quadrantPosition);
            quadrantPosition++;
        }
        quadrantPosition += 4;
    }
    quadrantPosition = 2 * width + 2;
    for (int i = 0; i < (height - 4); i++) {
        for (int j = 0; j < (width - 4); j++) {
            puzzleFrame = removeDiagonalDollar(puzzleFrame, width, height, quadrantPosition);
            quadrantPosition++;
        }
        quadrantPosition += 4;
    }
    return puzzleFrame;
}
char * removeAtSign(char * puzzleFrame, int width, int height, int position) { // Removes @ characters, which represent potential walls to make traversable
    // Looks right, left, down, and then up centered at position to check for @
    if (puzzleFrame[position] != '@') return puzzleFrame;
    if (puzzleFrame[position + 1] == '*' && puzzleFrame[position - width] == '*' && puzzleFrame[position - width + 1] == '*') {puzzleFrame[position] = '$'; return puzzleFrame;}
    if (puzzleFrame[position - 1] == '*' && puzzleFrame[position - width] == '*' && puzzleFrame[position - width - 1] == '*') {puzzleFrame[position] = '$'; return puzzleFrame;}
    if (puzzleFrame[position + 1] == '*' && puzzleFrame[position + width] == '*' && puzzleFrame[position + width + 1] == '*') {puzzleFrame[position] = '$'; return puzzleFrame;}
    if (puzzleFrame[position - 1] == '*' && puzzleFrame[position + width] == '*' && puzzleFrame[position + width - 1] == '*') {puzzleFrame[position] = '$'; return puzzleFrame;}
    puzzleFrame[position] = '*';
    return puzzleFrame;
}
char * removeDiagonalDollar(char * puzzleFrame, int width, int height, int position) { // Removes diagonal walls to make each area accessible
    if (puzzleFrame[position] == '$' && puzzleFrame[position + width - 1] == '$') {
        if (puzzleFrame[position - 1] == '*' && puzzleFrame[position + width] == '*') {
            puzzleFrame[position] = '*';
            return puzzleFrame;
        }
    }
    if (puzzleFrame[position] == '$' && puzzleFrame[position + width + 1] == '$') {
        if (puzzleFrame[position + 1] == '*' && puzzleFrame[position - width] == '*') {
            puzzleFrame[position] = '*';
            return puzzleFrame;
        }
    }
    return puzzleFrame;
}
char * freeExits(char * puzzleFrame, int width, int height, int position) { // Makes sure the beginning and end are free of walls
    if (puzzleFrame[position - 1] == '*' || puzzleFrame[position + 1] == '*' || puzzleFrame[position - width] == '*' || puzzleFrame[position + width] == '*') {
        puzzleFrame[position] = '*';
        return puzzleFrame;
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) if (puzzleFrame[position - width - 1 + j + (width * i)] == '$') puzzleFrame[position - width - 1 + j + (width * i)] = '*';
    puzzleFrame[position] = '*';
    return puzzleFrame;
}
char * findSolution(char * puzzleFrame, int width, int height, int position) { // Checks for spot in this order: Left, Up, Right, Down
    if (puzzleFrame[position] == '*' && puzzleFrame[position - 2] == '*' && puzzleFrame[position - 1] == '$') {puzzleFrame[position - 1] = '@'; return puzzleFrame;}
    if (puzzleFrame[position + width] == '*' && puzzleFrame[position + width - 2] == '*' && puzzleFrame[position + width - 1] == '$') {puzzleFrame[position + width - 1] = '@'; return puzzleFrame;}
    if (puzzleFrame[position + width] == '*' && puzzleFrame[position + 3 * width] == '*' && puzzleFrame[position + 2 * width] == '$') {puzzleFrame[position + 2 * width] = '@'; return puzzleFrame;}
    if (puzzleFrame[position + width + 1] == '*' && puzzleFrame[position + 1 + 3 * width] == '*' && puzzleFrame[position + 1 + 2 * width] == '$') {puzzleFrame[position + 1 + 2 * width] = '@'; return puzzleFrame;}
    if (puzzleFrame[position + width + 1] == '*' && puzzleFrame[position + width + 3] == '*' && puzzleFrame[position + width + 2] == '$') {puzzleFrame[position + width + 2] = '@'; return puzzleFrame;}
    if (puzzleFrame[position + 1] == '*' && puzzleFrame[position + 3] == '*' && puzzleFrame[position + 2] == '$') {puzzleFrame[position + 2] = '@'; return puzzleFrame;}
    if (puzzleFrame[position + 1] == '*' && puzzleFrame[position + 1 - 2 * width] == '*' && puzzleFrame[position - width + 2] == '$') {puzzleFrame[position - width + 2] = '@'; return puzzleFrame;}
    if (puzzleFrame[position] == '*' && puzzleFrame[position - 2 * width] == '*' && puzzleFrame[position - width] == '$') {puzzleFrame[position - width] = '@';  return puzzleFrame;}
    return puzzleFrame;
}
int generateBranches(char * puzzleFrame, int width, int height) { // Generates randomly the walls in the maze
    int widthQuadrantAmount = (width - 4) / 2;
    int heightQuadrantAmount = (height - 4) / 2;
    int quadrantPosition = 2 * width;
    for (int i = 0; i < heightQuadrantAmount; i++) {
        for (int i = 0; i < widthQuadrantAmount; i++) puzzleFrame = generateBranchesHelper(puzzleFrame, width, height, &quadrantPosition);
        quadrantPosition += width + (width % 2) + 4;
    }
}
char * generateBranchesHelper(char * puzzleFrame, int width, int height, int * quadrantPosition) { // Chooses how to lay out the maze walls
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
char * removeWideSpace(char * puzzleFrame, int width, int height) { // Removes some 2 x 2 squares of open space without a greater reason
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
char * removeWideSpaceHelper(char * puzzleFrame, int quadrantPosition, int width) { // Chooses what space to make a wall
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
char * smartRemoveWideSpace(char * puzzleFrame, int width, int height) { // Adds wall to some 2 x 2 open spaces depending on if it will prevent access to an area or not
    int quadrantPosition = 2 * width + 1;
    for (int i = 1; i <= (height - 4); i++) {
        for (int j = 1; j <= (width - 4); j++) {
            quadrantPosition += 1;
            if (puzzleFrame[quadrantPosition] != '*' || puzzleFrame[quadrantPosition + 1] != '*') continue;
            if (puzzleFrame[quadrantPosition + width] != '*' || puzzleFrame[quadrantPosition + width + 1] != '*') continue;
            for (int i = 0; i < 2; i++) puzzleFrame = smartRemoveWideSpaceHelper(puzzleFrame, (quadrantPosition + i), width);
            for (int i = 0; i < 2; i++) puzzleFrame = smartRemoveWideSpaceHelper(puzzleFrame, (quadrantPosition + width + i), width);
        }
        quadrantPosition += 4;
    }
    return puzzleFrame;
}
char * smartRemoveWideSpaceHelper(char * puzzleFrame, int position, int width) { // Makes sure that wall will not block off other part of maze
    int count = 0;
    int storePosition = position;
    position -= width;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++) if (puzzleFrame[position + (width * i) + j] == '$') count++;
    if (count >= 2) return puzzleFrame;
    count = 0;
    position += (width - 1);
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++) if (puzzleFrame[position + (width * i) + j] == '$') count++;
    if (count >= 2) return puzzleFrame;
    count = 0;
    position -= width;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++) if (puzzleFrame[position + (width * i) + j] == '$') count++;
    count = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++) if (puzzleFrame[position + (width * i) + j] == '$') count++;
    if (count >= 2) return puzzleFrame;
    puzzleFrame[storePosition] = '$';
    return puzzleFrame;
}