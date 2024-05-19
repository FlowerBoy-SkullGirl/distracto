#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "headers/global.h"
#include "headers/distracto_errors.h"
#include "headers/modeswitching.h"
#include "headers/terminal_mode.h"
#include "headers/list.h"
#include "headers/display.h"


int main()
{
	display_welcome();
	
	char *input = malloc(MAX_GOAL_SIZE);
	struct lnode *root = create_list();

	for (int i = 0; fgets(input, MAX_GOAL_SIZE, stdin); i++){
		if (i > 0)
			append_list_node(root);
		update_node_goal(root, i, input);

		display_goal_list(root);
	}

	return 0;

}
