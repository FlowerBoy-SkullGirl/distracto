#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "headers/terminal_mode.h"
#include "headers/global.h"

void restore_flag_state()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &_global_term_flag_state);
}

void start_term_raw_mode()
{

	tcgetattr(STDIN_FILENO, &_global_term_flag_state);
	atexit(restore_flag_state);

	struct termios raw = _global_term_flag_state;

	raw.c_lflag &= ~(ECHO | ICANON); //ISIG flag determines behavior of CTRL-C 
					 //ICRNL flag determines if CR is translated as \n

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
