#ifndef PUZZLE_H
#define PUZZLE_H

int generateBranches(char * puzzleFrame, int width, int height);
char * generateBranchesHelper(char * puzzleFrame, int width, int height, int * quadrantPosition);
char * generateSolution(char * puzzleFrame, int width, int height);
char * findSolution(char * puzzleFrame, int width, int height, int position);
char * removeWideSpace(char * puzzleFrame, int width, int height);
char * removeWideSpaceHelper(char * puzzleFrame, int quadrantPosition, int width);
char * smartRemoveWideSpace(char * puzzleFrame, int width, int height);
char * smartRemoveWideSpaceHelper(char * puzzleFrame, int position, int width);
char * generateStartEnd(char * puzzleFrame, int width, int height, char point);
char * removeAtSign(char * puzzleFrame, int width, int height, int position);
char * freeExits(char * puzzleFrame, int width, int height, int position);
char * removeDiagonalDollar(char * puzzleFrame, int width, int height, int position);
void generateMap();
void filterEdge(char * puzzleFrame, int width, int height, int startingIndex, char point);
void savePosition(char point, int position, int width);
void printPuzzleClear(char * puzzleFrame, int width, int height);
void printPuzzleSave(char * puzzleFrame, int width, int height);

#endif