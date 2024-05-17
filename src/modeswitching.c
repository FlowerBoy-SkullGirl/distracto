#include "headers/global.h"
#include "headers/modeswitching.h"


int set_text_mode(int m)
{
	switch(m){
		case M_NAVIGATION:
			return _global_text_mode = M_NAVIGATION;
			break;
		case M_EDIT:
			return _global_text_mode = M_EDIT; 
			break;
		default:
			distracto_error("Bad mode");
			return FUNC_ERROR;
			break;

	return FUNC_ERROR;
}

int current_text_mode()
{
	return _global_text_mode;
}
