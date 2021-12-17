#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

#define GRID_SIZE 11

using namespace std;

// Functions
void drawGame(char grid[][GRID_SIZE]);

// Variables
string divider("---------------------------------");

int main( int argc, char * argv[] ){

    /*
    Create game board
    Snake is an array of grid pairs (x, y) that indicate where to draw the snake
    Every turn, add 1 snake pair to the front, and remove 1 snake pair from the back
    If the snake eats a food, then dont remove 1 snake pair from the back

    */
    
    char grid[GRID_SIZE][GRID_SIZE];

    printf("%s\n", divider.c_str());
    for(int x=0;x<GRID_SIZE;x++){
       for(int y=0;y<GRID_SIZE;y++){
           grid[x][y] = '0';
           
       }
       
    }
    drawGame(grid);
    

    while(!_kbhit()){
        Sleep(100);
    }
    printf("\nKey was struck: %c\n",getch());

    for(int x=0;x<GRID_SIZE;x++){
       for(int y=0;y<GRID_SIZE;y++){
           
       }
    }

    return 0;
}

void drawGame(char grid[][GRID_SIZE]){
    for(int x=0;x<GRID_SIZE;x++){
       for(int y=0;y<GRID_SIZE;y++){
           printf(" %c ",grid[x][y]);
       }
       printf("\n");
    }
    printf("%s\n", divider.c_str());
    return;

}