#include <stdio.h>
#include "headers/global.h"
#include "headers/distracto_errors.h"

int distracto_error(char *error)
{
	printf("Error: %s\n", error);
	return FUNC_ERROR;
}
