#ifndef PUZZLE_H
#define PUZZLE_H

typedef struct nodeLinkedList node_l;
int deleteatbeginning();
int insertatend(int currentPosition, int width);
int findinlist(int data);
int sortList();
int solvePuzzle(char * puzzleFrame, int width, int height);
int findSolution(char * puzzleFrame, int width, int height);
int generateSolution(char * puzzleFrame, int width, int height);
int generateBranches(char * puzzleFrame, int width, int height);
char * generateBranchesHelper(char * puzzleFrame, int width, int height, int * quadrantPosition);
char * removeWideSpace(char * puzzleFrame, int width, int height);
char * removeWideSpaceHelper(char * puzzleFrame, int quadrantPosition, int width);
char * generateStartEnd(char puzzleFrame[], int width, int height, char point);
void generateMap();
void printList();
void filterEdge(char puzzleFrame[], int width, int height, int startingIndex, char point);
void savePosition(char point, int position, int width);
void printPuzzleClear(char puzzleFrame[], int width, int height);

#endif