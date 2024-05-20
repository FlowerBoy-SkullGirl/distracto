#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/config_loader.h"
#include "headers/global.h"

struct configs default_config = {
	.config_path = {"config_path", "~/.config/distracto"},
	.cache_path = {"cache_path", "~/.cache/distracto"},
	.cache_incomplete = {"cache_incomplete", "true"},
	.mode_key = {"mode_key", ":"},
	.del_key = {"del_key", "KEY_BACKSPACE"}
};

char *get_home_directory()
{
	char default_home[] = "HOME=~/";
	char *homedir = getenv("HOME");
	if (homedir == NULL){
		homedir = malloc(strlen(default_home)+1);
		snprintf(homedir, strlen(default_home)+1, default_home);
	}

	return homedir;
}

char *copy_and_truncate_env(char *src)
{
	char *dest = malloc(strlen(src) +1);
	char passed_equal = FALSE;
	int start = 0;
	for (int i = 0; i <= strlen(src); i++){
		if (passed_equal)
			dest[i-start] = src[i];

		if (src[i] == '='){
			passed_equal = TRUE;
			start = i + 1;
		}
	}
	
	return dest;
}

char *append_config_path(char *dest, char *config_path)
{
	if (dest == NULL)
		return NULL;

	char *temp = malloc(strlen(dest)+strlen(config_path)+1);
	
	for (int i = 0; i < strlen(dest); i++)
		temp[i] = dest[i];
	int start = strlen(dest)+1;

	//Additional +1 is for \0
	for (int i = start; i < start+strlen(config_path)+1; i++)
		temp[i+start] = config_path[i];

	free(dest);
	return temp;
}

struct config_var *extract_values_from_line(char *config_line)
{
	struct config_var *cv = malloc(sizeof(struct config_var));
	cv->var = malloc(MAX_GOAL_SIZE);
	cv->val = malloc(MAX_GOAL_SIZE);
	
	if (config_line == NULL)
		return cv;

	int passed_equals = FALSE;
	for (int i = 0; config_line[i] != '\0'; i++)
	{
		if (config_line[i] == '='){
			passed_equals = TRUE;
			continue;
		}
		if (passed_equals){
			(cv->val)[i] = config_line[i];
		}else{
			(cv->var)[i] = config_line[i];
		}

	}

	return cv;
}

