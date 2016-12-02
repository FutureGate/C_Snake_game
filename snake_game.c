//=============================================================================================
// Include Headers.

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <time.h>

//=============================================================================================
// Define Macro Constant

#define BOARD_SIZE 15
#define START_X 7
#define START_Y 7

#define BLANK 0
#define SNAKE 1
#define STAR 2
#define WALL 3

#define UP 100
#define DOWN 101
#define LEFT 102
#define RIGHT 103

#define UP_KEY 'w'
#define UP_KEY2 'W'
#define DOWN_KEY 's'
#define DOWN_KEY2 'S'
#define LEFT_KEY 'a'
#define LEFT_KEY2 'A'
#define RIGHT_KEY 'd'
#define RIGHT_KEY2 'D'

#define GAME_OVER 40

//=============================================================================================
// Snake Structure

typedef struct Snake {
	int x;
	int y;
	
	struct Snake *next;
	struct Snake *prev;
} Snake;

//=============================================================================================
// Function Prototype

void initGame();
void setWall();
void setSnake();
void setFood();
void render();
int move();

Snake *createSnake(int, int);
void addSnake(int, int);
void deleteSnake();

void printSnake();
void printKey(int key);

//=============================================================================================
// Global Variables

Snake *head = NULL;
Snake *tail = NULL;

int board[BOARD_SIZE][BOARD_SIZE];
int snake_x = START_X, snake_y = START_Y;

char direction = RIGHT;

//=============================================================================================
// Main

int main()
{
	char key, result;
	
	initGame();

	

	while(1) {
		setWall();

		while(1) {
			system("cls");

			setSnake();
			setFood();
			render();
			
			//printSnake();
			//printKey(key);
			
			Sleep(200);

			if(kbhit()) {
				key = getch();

				switch(key) {
					case UP_KEY:
					case UP_KEY2:
						direction = UP;
					break;
					
					case DOWN_KEY:
					case DOWN_KEY2:
						direction = DOWN;
					break;
					
					case LEFT_KEY:
					case LEFT_KEY2:
						direction = LEFT;
					break;
					
					case RIGHT_KEY:
					case RIGHT_KEY2:
						direction = RIGHT;
					break;	
				}
			}

			result = move();

			if(result == GAME_OVER) {
				system("pause");
			}
		}
	}
}

//=============================================================================================
// Function - Game

void initGame() {
	head = (Snake *)malloc(sizeof(Snake));
	tail = (Snake *)malloc(sizeof(Snake));

	head->next = tail;
	head->prev = NULL;
	tail->prev = head;
	tail->next = NULL;
	
	addSnake(snake_x, snake_y);
}

void setWall() {
	int i, j;
	
	for(i=0;i<BOARD_SIZE;i++) {
         for(j=0;j<BOARD_SIZE;j++) {
            board[i][j] = BLANK;
            
			if(j==0 || j==BOARD_SIZE-1 || i==0 || i==BOARD_SIZE-1)
            	board[i][j] = WALL;
        }
    }
}

void setSnake() {
	Snake *targetSnake = head->next;
	
	while(targetSnake != tail) {
		int x = targetSnake->x;
		int y = targetSnake->y;
		
		board[x][y] = SNAKE;
		
		targetSnake = targetSnake->next;
	}
}

void setFood() {
	int x, y;
	int i, j, isStarExsist = 0;
	
	srand((unsigned int)time(NULL));
	
	while(1) {
		x = (rand()%BOARD_SIZE-3)+1;
		y = (rand()%BOARD_SIZE-3)+1;
	
		if(board[x][y] != WALL && board[x][y] != SNAKE && x >= 0 && y >= 0)
			break;
	}

	for(i=0; i<BOARD_SIZE; i++) {
		for(j=0; j<BOARD_SIZE; j++) {
			if(board[i][j] == STAR) {
				isStarExsist = 1;
			}
		}
	}      

	if(isStarExsist != 1)
   		board[x][y] = STAR;
}

void render() { 
	int x, y;
	
	for(x=0; x<BOARD_SIZE; x++) {
		for(y=0; y<BOARD_SIZE; y++) {
			switch(board[x][y]) {
				case BLANK:
					printf("  ");
					break;
					
				case WALL:
					printf("¡á");
					
					if(y == BOARD_SIZE-1 && x != BOARD_SIZE-1)
						printf("\n");
					break;
					
				case SNAKE:
					printf("¢Ã");
					break;
					
				case STAR:
					printf("¡Ú");
					break; 
			}
		}
	}
}

int move()
{
	int count;

	switch(direction) {
		case UP:
			snake_x -= 1;
   			break;
   			
   		case DOWN:
   			snake_x += 1;
   			break;
   			
   		case LEFT:
   			snake_y -= 1;
   			break;
   		
		case RIGHT:
			snake_y += 1;
   			break;
	}
	
	switch(board[snake_x][snake_y]) {
		case STAR:
			break;
		
		case SNAKE:
		case WALL:
			return GAME_OVER;
			break;
		default:
			deleteSnake();
			break;
	}
	
	addSnake(snake_x, snake_y);
}

//=============================================================================================
// Function - Snake Structure Control

Snake *createSnake(int x, int y) {
	Snake *newSnake = (Snake *)malloc(sizeof(Snake));
	
	newSnake->x = x;
	newSnake->y = y;
	
	newSnake->prev = NULL;
	newSnake->next = NULL;
}

void addSnake(int x, int y) {
	Snake *newSnake = (Snake *)createSnake(x, y);
	
	head->next->prev = newSnake;
	newSnake->next = head->next;
	head->next = newSnake;
	newSnake->prev = head;
}

void deleteSnake() {
	Snake *tailSnake = tail->prev;
	
	int x = tailSnake->x;
	int y = tailSnake->y;
	
	board[x][y] = BLANK;
	
	tailSnake->prev->next = tail;
	tailSnake->next->prev = tailSnake->prev;
}

//=============================================================================================
// Function - Debug & Test

void printSnake() {
	Snake *targetSnake = head->next;
	
	while(targetSnake != tail) {
		int x = targetSnake->x;
		int y = targetSnake->y;
		
		printf("\n%d, %d", x, y);
		
		targetSnake = targetSnake->next;
	}
}

void printKey(int key) {
	printf("\n\n");
	
	switch(key) {
		case UP_KEY:
		case UP_KEY2:
			printf("UP\n");
		break;
		
		case DOWN_KEY:
		case DOWN_KEY2:
			printf("DOWN\n");
		break;
		
		case LEFT_KEY:
		case LEFT_KEY2:
			printf("LEFT\n");
		break;
		
		case RIGHT_KEY:
		case RIGHT_KEY2:
			printf("RIGHT\n");
		break;
	}
}
