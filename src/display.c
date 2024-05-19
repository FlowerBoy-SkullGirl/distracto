#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "headers/display.h"
#include "headers/global.h"
#include "headers/modeswitching.h"

#define SIZE_SPLASH 4

//Start the ncurses screen and input control
WINDOW *initialize_display()
{
	initscr();
	cbreak();
	noecho();
	clear();
	keypad(stdscr, TRUE);
	noecho();

	return newwin(0, 0, 0, 0);
}

void clear_row(WINDOW *win, int row, int size_row)
{
	for (int i = 0; i < size_row; i++){
		mvwprintw(win, row, i, " ");
	}
}

void display_welcome(WINDOW *win)
{
	clear();
	wprintw(win, "Hello!\n\nWhat are your goals for today?\n");
	wrefresh(win);
}

void display_goal_list(WINDOW *win, struct lnode *lroot, int selected, char *buffer)
{
	int x = 0;
	int y = 0;
	getmaxyx(win, y, x); 
	int offset;
	struct lnode *np = lroot;
	for (int i = 0; np != NULL; i++){
		offset = i + SIZE_SPLASH;

		clear_row(win, offset, x);

		if (np->goal == NULL){
			np = np->next;
			continue;
		}


		if (i == selected)
			wattron(win, A_REVERSE);

		if (np->complete_flag == FALSE){
			mvwprintw(win, offset, 0, "Incomplete: ");
		}else{
			mvwprintw(win, offset, 0, "Complete:   ");
		}

		mvwprintw(win, offset, 12, "%d. %s", i + 1, np->goal);

		wattroff(win, A_REVERSE);

		np = np->next;
	}

	//in case last item was deleted
	clear_row(win, ++offset, x);
	

	if (current_text_mode() == M_EDIT){
		wattron(win, A_REVERSE);
		clear_row(win, y - 1, x);
		mvwprintw(win, y - 1, 0, "%s", buffer);
		wattroff(win, A_REVERSE);
	}
	
	wrefresh(win);
}
