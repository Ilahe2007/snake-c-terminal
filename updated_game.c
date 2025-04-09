#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define COL 20
#define ROW 10
#define DIG 100

// hər şeyi global etmək işləri daha çətin edir
char grid[ROW][COL];
int snake[ROW * COL];
int blank_spaces[ROW * COL];
int snake_score = 0, grow = 0, size = 0;
int random_row = 3, random_column = 4, row = 0, column = 0;

void clearGrid(char grid[ROW][COL], int *snake_size){
	// This function clears the grid. Fills every cell with whitespace
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			if((i == 0) || (i == ROW - 1))
				grid[i][j] = '-';
			else if((j == 0) || (j == COL - 1))
				grid[i][j] = '|';
			else
				grid[i][j] = ' ';
		}
	}grid[random_row][random_column] = 'o'; 
}

void blank(char grid[ROW][COL]){
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			if(grid[i][j] == ' '){
				blank_spaces[size] = i * 100 + j;
				size++;
			}
		}
	}
}

void clear_blank_space(int blank_spaces[size]){
	for(int i = 0; i < size; i++){
		blank_spaces[i] = 0;
	}size = 0;
}

void random_food_appear(char grid[ROW][COL],int *snake_size){
    /*
    This function generates a food on random place in the screen. and each time snake eats the food, it regenerates the food in another place and keeps score.
    Input:
        grid[ROW][COL]: char
    Output:
        void
    */	
	if(*snake_size < (ROW - 1) * (COL - 1)){
		grow = 1;
		int random_number = rand() % size;
		random_row = blank_spaces[random_number] / 100;
		random_column = blank_spaces[random_number] % 100;
	}else{
		return ;
	}snake_score += 100;
}

void init_snake(int snake[], int *snake_size){
	// This function initialize snake. It sets it body parts next to each other
	for(int i = 0; i < *snake_size; i++){
		snake[i] = 5 * 100 + (5 + i);
	}
}

//checking to see if snake's head collides with walls' symbols or its body's symbols
char collision_check(int snake[], int snake_size, char grid[ROW][COL]){
	switch (grid[snake[snake_size - 1] / 100][snake[snake_size - 1] % 100]){
		case '|':
			return 2;
		case '-':
			return 2;
		case '#':
			return 2;

	
		case 'o':
			return 1;
	}

	return 0;
}

void move_snake(int snake[], int *snake_size, int snake_facing){
	int i;
	switch(snake_facing){
		case '>':
			row = snake[*snake_size - 1] / 100;
			column = snake[*snake_size - 1] % 100 + 1;
			break;
		case '^':
			row = snake[*snake_size - 1] / 100 - 1;
			column = snake[*snake_size - 1] % 100;
			break;
		case '<':
			row = snake[*snake_size - 1] / 100;
			column = snake[*snake_size - 1] % 100 - 1;
			break;
		case 'v':
			row = snake[*snake_size - 1] / 100 + 1;
			column = snake[*snake_size - 1] % 100;
			break;
	}

	if (grow == 1) {
        // adds new segment to the snake
		snake[*snake_size] = row * 100 + column;
		*snake_size += 1;
        grow = 0;
    } else {
		//changes the place of snake without growing
        for(i = 0; i < *snake_size - 1; i++){
			snake[i] = snake[i + 1];
		}
		snake[*snake_size - 1] = row * 100 + column;
    }
}

int main(void){
	// Initializing ncurses for getting real time input
	initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);  
    nodelay(stdscr, TRUE);
	
	int snake[ROW * COL], snake_size = 3;
	char snake_facing = '>';

	srand(time(0));
	clearGrid(grid, &snake_size);
	init_snake(snake, &snake_size);

	blank(grid);
	random_food_appear(grid, &snake_size);

	int i, j;
    char buf, run = 1, moved = 0,
	move_delay = 12, move_delay_index = 0;

    while (run){
		buf = getch();
        switch (buf){
            case 'd':
				if((snake_facing != '<') && !moved){
					snake_facing = '>';
					moved = 1;
				}
                break;
            case 'w':
				if((snake_facing != 'v') && !moved){
					snake_facing = '^';
					moved = 1;
				}
                break;
            case 'a':
				if((snake_facing != '>') && !moved){
					snake_facing = '<';
					moved = 1;
				}
                break;
            case 's':
				if((snake_facing != '^') && !moved){
					snake_facing = 'v';
					moved = 1;
				}
                break;
            case 'q':
                run = 0;
        }
		clearGrid(grid, &snake_size);
		erase();

		// yer.1 izah üçün buranı göstərəcəm

		if(move_delay < move_delay_index){
			move_snake(snake, &snake_size, snake_facing);

			if (snake[snake_size - 1] / 100 == random_row && snake[snake_size - 1] % 100 == random_column){
				clear_blank_space(blank_spaces);
				blank(grid);
				random_food_appear(grid, &snake_size);

			}size = 0;
			move_delay_index = 0;
			moved = 0;
		}

		for(i = 0; i < snake_size - 1; i++){
			grid[snake[i] / 100][snake[i] % 100] = '#';
		} 

        if (collision_check(snake,snake_size,grid) == 2){
			run = 0;
		}

		grid[snake[snake_size - 1] / 100][snake[snake_size - 1] % 100] = '@';

		for(i = 0; i < ROW; i++){
			for(j = 0; j < COL; j++){
				printw("%c", grid[i][j]);
			}
			printw("\n");
		}

		refresh();
		move_delay_index++;
		usleep(16000);

    }

	endwin();
    return 0;
}
