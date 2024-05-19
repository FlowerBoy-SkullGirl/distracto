#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/display.h"
#include "headers/global.h"

void display_welcome()
{
	printf("Hello!\n\nWhat are your goals for today?\n");
}

void display_goal_list(struct lnode *lroot)
{
	struct lnode *np = lroot;
	for (int i = 0; np != NULL; i++){
		if (np->goal == NULL)
			continue;
		if (np->complete_flag == FALSE)
			printf("Incomplete: ");
		else
			printf("Complete: ");
		printf("%d. %s", i + 1, np->goal);
		np = np->next;
	}
}
