#include <stdio.h>
#include <stdlib.h>

#include "../src/headers/list.h"
#include "../src/list.c"
#include "../src/headers/distracto_errors.h"


int main()
{
	struct lnode *root = create_list();
	append_list_node(root);

	update_node_goal(root, 1, "I want to test all the functions in list.c");

	printf("Root next goal:%s\n", root->next->goal);

	flip_complete_flag(root, 0);

	printf("Complete flag root: %d\n", root->complete_flag);

	root = remove_list_node(root, 0);

	printf("Goal new root:%s\n", root->goal);
	
	append_list_node(root);

	remove_list_node(root, 1);

	if (root->next == NULL)
		printf("Successful remove index 1\n");

	return 0;

}
