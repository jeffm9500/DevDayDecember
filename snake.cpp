#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 11
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESCAPE 27
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define GAMEOVER 123456789

using namespace std;

class Node {
    public:
    int x, y;
    Node *next = NULL;
    // This code is contributed by rathbhupendra

    Node(int x, int y){
        this->x = x % GRID_SIZE;
        this->y = y % GRID_SIZE;
        this->next = NULL;
    }
    Node(){
        this->x = 0;
        this->y = 0;
        this->next = NULL;
    }
    
};

enum direction{
    up,
    down,
    l,
    r
};

// Functions
int playGame(int hiscore);
void drawGame(char grid[][GRID_SIZE], int colour);
void push(Node** head, int x, int y);
void append(Node** head, int x, int y);
void deleteNode(Node** head, int position);
void deleteList(Node** head);
int* updateGrid(char grid[GRID_SIZE][GRID_SIZE], Node** head, bool ate, int* prevFood, int input);
int getInput(direction dir);
string setColour(int colour);


// Global Variables
string divider(" ------------------------------------- ");
direction dir = up; //keep track of snake direction so you cant do a 180; initialize facing up
int score = 0;
int prevScore = 0;
int hiscore = 0;
bool enableColour = false;


/*
    Create game board
    Snake is linked list of pairs (x, y) that represent the game tiles it occupies
    Every turn, add 1 snake pair to the front, and remove 1 snake pair from the back
    If the snake eats a food, then push the food (x, y) to the snake

    TO DO:


    - more efficient food placement for late game
    - make exportable .exe for coworkers to try
    - remove colours for default command prompt
    Stretch goal:
    - implement automatic movement (move 1x every second or something)
        - arrow keys buffer input for next tick


    Useful escape sequences:
    Code	Effect
    "\033[2J"	Clear the screen.
    "\033[H"	Move the cursor to the upper-left corner of the screen.
    "\033[r;cH"	Move the cursor to row r, column c. Note that both the rows and columns are indexed starting at 1.
    "\033[?25l"	Hide the cursor.
    "\033[K"	Delete everything from the cursor to the end of the line.

    "\033[0m"	Reset special formatting (such as colour).
    "\033[30m"	Black text.
    "\033[31m"	Red text.
    "\033[32m"	Green text.
    "\033[33m"	Yellow text.
    "\033[34m"	Blue text.
    "\033[35m"	Magenta text.
    "\033[36m"	Cyan text.
    "\033[37m"	White text.

    */

int main( int argc, char * argv[] ){
    srand(time(NULL));

    bool quit = false;

    printf("Welcome to the Dev Day Snake Game created by Jeff!\nUse the arrow keys to move, and the escape key to quit.\n");

    // Toggle for colours (since they arent supported with every terminal)
    printf("Enable colours? [y/n] (disable them if using the default command prompt)\n");
    while(!_kbhit()){
        Sleep(10);
    }
    int c = getch();
    switch(c) {
        case 'n':
            enableColour = false;
            printf("turning OFF colour\n");
            break;
        case 'N':
            enableColour = false;
            break;
        case 'y':
            enableColour = true;
            printf("turning ON colour\n");
            break;
        case 'Y':
            enableColour = true;
            break;
        case KEY_ESCAPE:
            quit = true;
            break;
        case 'q':
            quit = true;
            break;
        default:
            break;
    }

    
    
    do {
        hiscore = playGame(hiscore);
        printf("Play again? [y/n]\n");
        while(!_kbhit()){
            Sleep(10);
        }
        int c = getch();
        switch(c) {
            case 'n':
                quit = true;
                break;
            case 'N':
                quit = true;
                break;
            case 'y':
                quit = false;
                break;
            case 'Y':
                quit = false;
                break;
            case KEY_ESCAPE:
                quit = true;
                break;
            case 'q':
                quit = true;
                break;
            default:
                continue;
        } 

    } while (!quit);
    
}



//================================================================================================================================

int playGame(int hiscore){
    char grid[GRID_SIZE][GRID_SIZE];
    Node* snake = new Node(int(GRID_SIZE/2), int(GRID_SIZE/2)); //inital snake location, middle of the board
    //printf("Snake starting at: %d, %d\n",int(GRID_SIZE/2), int(GRID_SIZE/2));
    //printf("Use the arrow keys to move the snake\n");
    append(&snake, int(GRID_SIZE/2) + DOWN, int(GRID_SIZE/2));
    append(&snake, int(GRID_SIZE/2) + DOWN + DOWN, int(GRID_SIZE/2));
    int snakeLength = 3;
    int *food = nullptr;
    bool ate = false;
    int head_x, head_y;
    
    int input = KEY_UP; //intialize snake head facing up
    dir = up;
    int TIMER = 600; //Speed (ms) between ticks. Will decrease as difficulty increases


    if(enableColour) setColour(0); //reset colours
    cout << "~~~ NEW GAME ~~~" << endl;
    bool newGame = true;
    
    
    

    while(input != GAMEOVER){
        
        
        Sleep(TIMER);
        food = updateGrid(grid, &snake, ate, food, input);


        drawGame(grid, 5);
        ate = false;

        //wait for input on startup
        if (newGame) {
            getch();
            while(!_kbhit()){
                Sleep(10);
            }
        }
        newGame = false;

        input = getInput(dir);
        
        if(input == KEY_ESCAPE){
            cout << "escape key pressed" << endl;
            break; //exit game
        } else if(input == GAMEOVER){
            break;
        }

        head_x = snake->x;
        head_y = snake->y;

        //printf("Input: %d\n",input);
        switch(input){
                case KEY_UP:
                    dir = up;
                    if (head_x == 0){
                        // user has hit the top wall
                        printf("Hit the top wall!\n");
                        input = GAMEOVER;
                        break;
                    } else if(grid[head_x-1][head_y] == '0') {
                        // snake head has hit its body
                        printf("Hit yourself!\n");
                        input = GAMEOVER;
                        break;
                    } else {
                        push(&snake, head_x-1, head_y);
                        if(food[0] == head_x-1 && food[1] == head_y){
                            // snake head has reached a food
                            snakeLength++;
                            score++;
                            ate = true;
                        } else {
                            //snake did not eat, so delete last bit of tail (simulate movement)
                            deleteNode(&snake, snakeLength);
                        }
                    }
                    
                    break;
                case KEY_DOWN:
                    dir = down;
                    if (head_x == GRID_SIZE-1){
                        // user has hit the bottom wall
                        printf("Hit the bottom wall!\n");
                        input = GAMEOVER;
                        break;
                    } else if(grid[head_x+1][head_y] == '0') {
                        // snake head has hit its body
                        printf("Hit yourself!\n");
                        input = GAMEOVER;
                        break;
                    } else {
                        push(&snake, head_x+1, head_y);
                        if(food[0] == head_x+1 && food[1] == head_y){
                            // snake head has reached a food
                            snakeLength++;
                            score++;
                            ate = true;
                        } else {
                            //snake did not eat, so delete last bit of tail (simulate movement)
                            deleteNode(&snake, snakeLength);
                        }
                    }
                    
                    break;
                case KEY_LEFT:
                    dir = l;
                    if (head_y == 0){
                        // user has hit the left wall
                        printf("Hit the left wall!\n");
                        input = GAMEOVER;
                        break;
                    } else if(grid[head_x][head_y-1] == '0') {
                        // snake head has hit its body
                        printf("Hit yourself!\n");
                        input = GAMEOVER;
                        break;
                    } else {
                        push(&snake, head_x, head_y-1);
                        if(food[0] == head_x && food[1] == head_y-1){
                            // snake head has reached a food
                            snakeLength++;
                            score++;
                            ate = true;
                        } else {
                            //snake did not eat, so delete last bit of tail (simulate movement)
                            deleteNode(&snake, snakeLength);
                        }
                    }
                    
                    break;
                case KEY_RIGHT:
                    dir = r;
                    if (head_y == GRID_SIZE-1){
                        // user has hit the right wall
                        printf("Hit the right wall!\n");
                        input = GAMEOVER;
                        break;
                    } else if(grid[head_x][head_y+1] == '0') {
                        // snake head has hit its body
                        printf("Hit yourself!\n");
                        input = GAMEOVER;
                        break;
                    } else {
                        push(&snake, head_x, head_y+1);
                        if(food[0] == head_x && food[1] == head_y+1){
                            // snake head has reached a food
                            snakeLength++;
                            score++;
                            ate = true;
                        } else {
                            //snake did not eat, so delete last bit of tail (simulate movement)
                            deleteNode(&snake, snakeLength);
                        }
                    }
                    
                    break;
            }

        // increase difficulty based on score
        if (score != prevScore && (score % 5 == 0)) {
            //every 5 score, increase speed by 50ms
            TIMER -= 50;
        }
        if (TIMER < 100) TIMER = 100; //enforce minimum time
        prevScore = score;

    }

    if (hiscore < score) hiscore = score;
    

    drawGame(grid, 2);
    cout << "Game Over." << endl;
    deleteList(&snake);
    score = 0;
    return hiscore;
}






void drawGame(char grid[][GRID_SIZE], int colour){

    if(enableColour) {
        printf("%s",setColour(colour).c_str());

        // clear screen
        printf("\033[2J");
        printf("\033[%d;%dH", 1, 1);
    } else {
        printf("Colours disabled\n");
        system("CLS");
    }
    printf("\n%s\n", divider.c_str());
    for(int x=0;x<GRID_SIZE;x++){
        printf(" | ");
        for(int y=0;y<GRID_SIZE;y++){
            if(grid[x][y] == '$' && enableColour){
                printf(" \033[32m%c ",grid[x][y]);
                printf("%s",setColour(colour).c_str());
            } else if((grid[x][y] == '0' || grid[x][y] == '^' || grid[x][y] == 'V' || grid[x][y] == '<' || grid[x][y] == '>') && enableColour){
                printf(" \033[33m%c ",grid[x][y]);
                printf("%s",setColour(colour).c_str());
            
            } else {
                printf(" %c ",grid[x][y]);
            }
            
        }
        printf(" | ");
        printf("\n");
    }
    printf("%s\n", divider.c_str());

    //print scores
    if (enableColour){
        printf("\033[33m[Score: %d]\n", score);
        printf("\033[33m[Hiscore: %d]\n", hiscore);
        setColour(0);
    } else {
        printf("[Score: %d]\n", score);
        printf("[Hiscore: %d]\n", hiscore);
    }
    
    
    return;

}

void push(Node** head, int x, int y){
    //adds a node to the front of the list, behind the head
    //allocate new node
    Node* new_node = new Node(x, y);
    //make next of the new node point to the previous front
    new_node->next = (*head);
    //make the head pointer point to the new node
    (*head) = new_node;
    //printf("Pushing (%d, %d)\n",x,y);
    return;
    // This code (function) is contributed by rathbhupendra
}

void append(Node** head, int x, int y){
    //adds a new node to the end of the list
    //allocate new node
    Node* new_node = new Node(x, y);
    Node* last = *head;
    new_node->next = NULL;
    if(*head == NULL){
        *head = new_node;
        return;
    }
    while(last->next != NULL){
        last = last->next;
    }
    last->next = new_node;
    //printf("Appending (%d, %d)\n",x,y);
    return;
}

void deleteNode(Node** head, int position){
    if(*head==NULL){
        return;
    }
    Node* temp = *head;
    if(position==0){
        *head = temp->next;
        free(temp);
        return;
    }
    for(int i=0;i<position-1 && temp != NULL; i++){
        temp=temp->next;
    }
    if(temp == NULL || temp->next == NULL){
        return;
    }
    Node* next = temp->next->next;
    temp->next = next;
    free(temp->next);

    // This code (function) is contributed by premsai2030
}

void deleteList(Node** head){
    Node* current = *head;
    Node* next = NULL;

    while(current != NULL){
        next = current->next;
        free(current);
        current=next;
    }
    *head = NULL;
    return;
}

int* updateGrid(char grid[GRID_SIZE][GRID_SIZE], Node** head, bool ate, int* prevFood, int input){
    
    // wipe board
    for(int x=0;x<GRID_SIZE;x++){
       for(int y=0;y<GRID_SIZE;y++){
           grid[x][y] = '.';
       }
    }
    
    Node* current = *head;

    // set up snake head
    if (current != NULL){
        
        switch(input){
                case KEY_UP:
                    //cout << endl << "Up" << endl;//key up
                    grid[current->x][current->y] = '^';
                    break;
                case KEY_DOWN:
                    //cout << endl << "Down" << endl;   // key down
                    grid[current->x][current->y] = 'V';
                    break;
                case KEY_LEFT:
                    //cout << endl << "Left" << endl;  // key left
                    grid[current->x][current->y] = '<';
                    break;
                case KEY_RIGHT:
                    //cout << endl << "Right" << endl;  // key right
                    grid[current->x][current->y] = '>';
                    break;
                default:
                    cout << endl << "invalid input when updating grid: " << input << endl;

        }
        //grid[current->x][current->y] = '@';
        //printf("Drawing head (%d, %d)\n",current->x,current->y);
        current = current->next;
    }
    
    // set up snake body
    while(current != NULL){
        grid[current->x][current->y] = '0';
        //printf("Drawing body (%d, %d)\n",current->x,current->y);
        current = current->next;
    }


    int x, y;
    if(ate || prevFood == nullptr) {
        // find a spot for food (not occupied by snake)
        do {
            x = rand() % GRID_SIZE;
            y = rand() % GRID_SIZE;
            //reshuffle until you find an empty space
            //TODO: add a condition that makes this a faster calculation with a big snake, and support end game
            //TODO: add a win condition when all spaces are full
        } while(grid[x][y] == '0' || grid[x][y] == '^' || grid[x][y] == 'V' || grid[x][y] == '<' || grid[x][y] == '>');

        // place food
        grid[x][y] = '$';

        // return where the food is placed
        static int food[2];
        food[0] = x;
        food[1] = y;
        return food;
    } else {
        grid[prevFood[0]][prevFood[1]] = '$';
        return prevFood;
    }
    
    
    
    
}

int getInput(direction dir){
    /*
    poll input until a valid input is accepted
    Acceptable inputs
    - Esc
    - Up Arrow
    - Down Arrow
    - Left Arrow
    - Right Arrow
    Anything else gets re-polled 
    (should it submit an empty input, or pause until a valid input is entered?)
    
    
    */
    //cout << "dir: " << dir << endl;

    int c;
    /*
    while(!_kbhit()){
        Sleep(10);
    }
    c = getch();
    */
    if(kbhit()){
        c = getch();
    }
    while(1) {
        /*
        if (c && c != 224 && c != KEY_ESCAPE) {
            cout << endl << "Not arrow: " << (char) c << endl;
            continue;
        }
        */
       //cout << "input: " << c << endl;
       if (c == 224 || c == 0){
            c = getch();
            //cout << "next input: " << c << endl;
            switch(c){

                case KEY_ESCAPE:
                    //cout << endl << "Escape" << endl;
                    return c;
                case KEY_UP:
                    //cout << endl << "Up" << endl;//key up
                    if (dir == 1){
                        // end game
                        dir = up;
                        return GAMEOVER;
                    }
                    break;
                case KEY_DOWN:
                    //cout << endl << "Down" << endl;   // key down
                    if (dir == 0){
                        // end game
                        dir = down;
                        return GAMEOVER;
                    }
                    break;
                case KEY_LEFT:
                    //cout << endl << "Left" << endl;  // key left
                    if (dir == 3){
                        // end game
                        dir = l;
                        return GAMEOVER;
                    }
                    break;
                case KEY_RIGHT:
                    //cout << endl << "Right" << endl;  // key right
                    if (dir == 2){
                        // end game
                        dir = r;
                        return GAMEOVER;
                    }
                    break;
                default:
                    cout << "not a valid input, try again" << endl;
                    cout << (char) c << endl;  // not arrow
                    continue;
            
            }
        } else if(c == KEY_ESCAPE){
            return c;
        } else {
            //printf("no input, returning dir\n");
            // No input received, so continue in the current direction
            switch(dir) {
                case 0:
                    return KEY_UP;
                case 1:
                    return KEY_DOWN;
                case 2: 
                    return KEY_LEFT;
                case 3:
                    return KEY_RIGHT;
                default:
                    printf("ERROR: This should not ever be printed\n");
                    return KEY_UP;

            }
            //continue;
        }
    
    //cout << "returning: " << (char) c << endl;
    return c;
    }
    //cout << "escaped while loop, returning c" << endl;
    return c;
    
}

string setColour(int colour){
    /*
    "\033[0m"	Reset special formatting (such as colour).
    "\033[30m"	Black text.
    "\033[31m"	Red text.
    "\033[32m"	Green text.
    "\033[33m"	Yellow text.
    "\033[34m"	Blue text.
    "\033[35m"	Magenta text.
    "\033[36m"	Cyan text.
    "\033[37m"	White text.
    */
   switch(colour){
        case 0:
            return("\033[0m"); //reset
        case 1:
            return("\033[30m"); //black text
        case 2:
            return("\033[31m"); //red text
        case 3:
            return("\033[32m"); //green text
        case 4:
            return("\033[33m"); //yellow text
        case 5:
            return("\033[34m"); //blue text
        case 6:
            return("\033[35m"); //magenta text
        case 7:
            return("\033[36m"); //cyan text
        case 8:
            return("\033[37m"); //white text
        default:
            return("invalid input");

   }
}


