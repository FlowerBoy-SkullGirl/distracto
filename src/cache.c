#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/list.h"
#include "headers/cache.h"
#include "headers/distracto_errors.h"
#include "headers/global.h"

int write_to_cache(struct lnode *lroot, char *cache_path)
{
	struct lnode *np = lroot;

	FILE *fp = fopen(cache_path, "w");
	if (fp == NULL){
		distracto_error("Could not open cache\n");
		return FUNC_ERROR;
	}
	
	while (np != NULL){
		if (np->complete_flag == FALSE && np->goal != NULL)
			fprintf(fp, "%s\n", np->goal);
		np = np->next;
	}

	if (fp != NULL)
		fclose(fp);

	return 0;
}

int load_from_cache(struct lnode *lroot, char *cache_path)
{
	struct lnode *np = lroot;

	FILE *fp = fopen(cache_path, "r");
	if (fp == NULL){
		distracto_error("Could not open cache\n");
		return FUNC_ERROR;
	}
	
	char *line = malloc(MAX_GOAL_SIZE);
	if (line == NULL){
		distracto_error("Could not create string from cache\n");
		return FUNC_ERROR;
	}

	while (fgets(line, MAX_GOAL_SIZE, fp)){
		if (np->goal != NULL)
			np = append_list_node(lroot);

		//Do not load empty lines
		if (strcmp(line, "\n") == 0)
			continue;

		//Remove excess newlines from cached goals
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';

		update_node_goal(np, 0, line);
	}

	if (line != NULL)
		free(line);

	return 0;
}
