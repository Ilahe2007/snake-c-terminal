#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

int COL = 20;
int ROW = 20;
int DIFFICULTY = 1; 
int SPEED = 9; 

#include "snake.c"

void configure(int row, int difficulty, int speed){
	int row_value, speed_value;
	switch(row){
		case 1:
			row_value = 10;
			break;
		case 2:
			row_value = 15;
			break;
		case 3:
			row_value = 20;
			break;
		case 4:
			row_value = 25;
			break;
		case 5:
			row_value = 30;
			break;
	}
	switch(speed){
		case 1:
			speed_value = 12;
			break;
		case 2:
			speed_value = 9;
			break;
		case 3:
			speed_value = 6;
			break;
	}


	COL = row_value;
	ROW = row_value;
	DIFFICULTY = difficulty - 1;
	SPEED = speed_value;
}

int main() {
	// Initializing ncurses for getting real time input
	initscr();
	//cbreak();
	noecho();
	start_color();
	use_default_colors();
	keypad(stdscr, TRUE);  
	nodelay(stdscr, TRUE);

	short menu = 0;
	short select = 1, subSelect = 3;
	short limit = 3, subLimit = 5;

	short gridSize = 3, difficulty = 2, speed = 2;

	char gridSizeText[11];
	char difficultyText[11];
	char speedText[11];
	char message[50];

	while (1) {
		erase();
		if (menu == 0) {
			printw("+---- Snake Game ----+\n");

			switch (select) {
			case 1:
				printw("|      > Play <      |\tStarts the game with\n"
					"|      Settings      |\tgiven settings...\n"
					"|        Exit        |\n");
				break;
			case 2:
				printw("|        Play        |\tConfigure settings\n"
					"|    > Settings <    |\tfor the game...\n"
					"|        Exit        |\n");
				break;
			case 3:
				printw("|        Play        |\tPlease don't click me...\n"
					"|      Settings      |\n"
					"|      > Exit <      |\n");
				break;
			default:
				printw("|  An error occured  |\n");
				break;
			}

			printw("+--------------------+\n");
		}
		else {

			if (select == 1)
				gridSize = subSelect;
			else if (select == 2)
				difficulty = subSelect;
			else
				speed = subSelect;


			if (difficulty == 1)
				sprintf(difficultyText, "#    ");
			else if (difficulty == 2)
				sprintf(difficultyText, "# #  ");
			else
				sprintf(difficultyText, "# # #");

			if (speed == 1)
				sprintf(speedText, "#    ");
			else if (speed == 2)
				sprintf(speedText, "# #  ");
			else
				sprintf(speedText, "# # #");

			switch (gridSize) {
			case 1:
				sprintf(gridSizeText, "10x10");
				break;
			case 2:
				sprintf(gridSizeText, "15x15");
				break;
			case 3:
				sprintf(gridSizeText, "20x20");
				break;
			case 4:
				sprintf(gridSizeText, "25x25");
				break;
			case 5:
				sprintf(gridSizeText, "30x30");
				break;
			}

			printw("+----- Configuration -----+\n");



			printw("|   Grid size   ");

			if (select == 1 && subSelect != 1)
				printw("< ");
			else
				printw("  ");

			printw("%s", gridSizeText);

			if (select == 1 && subSelect != subLimit)
				printw(" >");
			else
				printw("  ");

			printw(" |\n");



			printw("|  Difficulty   ");

			if (select == 2 && subSelect != 1)
				printw("< ");
			else
				printw("  ");

			printw("%s", difficultyText);

			if (select == 2 && subSelect != subLimit)
				printw(" >");
			else
				printw("  ");

			printw(" |\n");



			printw("|       Speed   ");

			if (select == 3 && subSelect != 1)
				printw("< ");
			else
				printw("  ");

			printw("%s", speedText);

			if (select == 3 && subSelect != subLimit)
				printw(" >");
			else
				printw("  ");

			printw(" |\n");



			printw("+-------------------------+\n");
		}

		switch (getch())
		{
		case 'w': case KEY_UP:
			select--;
			if (menu == 1) {
				if (select == 1)
					subSelect = gridSize;
				else if (select == 2)
					subSelect = difficulty;
				else
					subSelect = speed;
			}
			break;
		case 's': case KEY_DOWN:
			select++;
			if (menu == 1) {
				if (select == 1)
					subSelect = gridSize;
				else if (select == 2)
					subSelect = difficulty;
				else
					subSelect = speed;
			}
			break;
		case 'a': case KEY_LEFT:
			if (menu == 1)
				subSelect--;
			break;
		case 'd': case KEY_RIGHT:
			if (menu == 1)
				subSelect++;
			break;
		case ' ':
			if (menu == 0) {
				switch (select)
				{
				case 1:
					game();
					printw("Game starts...");
					break;
				case 2:
					menu = 1;
					select = 1;
					limit = 3;
					subLimit = 5;
					subSelect = gridSize;
					break;
				case 3:
					endwin();
					exit(0);
					break;
				}
			}
			else {
				menu = 0;
				select = 1;
				limit = 3;
				configure(gridSize, difficulty, speed);
			}
			break;
		}

		if (select > limit) {
			select = 1;
			if (menu == 1) {
				subSelect = gridSize;
			}
		}
		else if (select <= 0) {
			select = limit;
			if (menu == 1) {
				subSelect = speed;
			}
		}

		switch (select)
		{
		case 1:
			subLimit = 5;
			break;
		case 2:
			subLimit = 3;
			break;
		case 3:
			subLimit = 3;
			break;
		default:
			subLimit = 5;
		}

		if (subSelect > subLimit)
			subSelect = subLimit;
		else if (subSelect <= 0)
			subSelect = 1;
		refresh();
		usleep(16000);
	}

    endwin();
	return 0;
}
