#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "headers/display.h"
#include "headers/global.h"
#include "headers/modeswitching.h"

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
	erase();
	refresh();
	struct lnode *np = lroot;
	for (int i = 0; np != NULL; i++){
		if (np->goal == NULL){
			np = np->next;
			continue;
		}

		if (i == selected)
			wattron(win, A_REVERSE);

		if (np->complete_flag == FALSE){
			mvwprintw(win, i, 0, "Incomplete: ");
		}else{
			mvwprintw(win, i, 0, "Complete: ");
		}
		mvwprintw(win, i, 12, "%d. %s", i + 1, np->goal);

		wattroff(win, A_REVERSE);

		np = np->next;
	}

	if (current_text_mode() == M_EDIT){
		getmaxyx(win, y, x); 
		wattron(win, A_REVERSE);
		mvwprintw(win, y - 1, 0, "%s", buffer);
		wattroff(win, A_REVERSE);
	}
	
	wrefresh(win);
}
