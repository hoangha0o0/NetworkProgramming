/* 
 * @copyright (c) 2020, ICT, Hanoi University of Technology
 * @author HoangThuy Ha
 * @version 1.0
 */

#include <stdio.h>
#include <string.h>
#include "HAZCHEM.h"

#define MAXL_YES_NO 3

HAZCHEM read() {
	char code[MAXL_HAZCHEM_CODE + 1];
	printf("Enter HAZCHEM code: ");
	scanf("%s", code);
	while(strlen(code) < MAXL_HAZCHEM_CODE)
		code[strlen(code)] = '_';
	code[MAXL_HAZCHEM_CODE] = '\0';

	bool _black = true;
	char reversed[MAXL_YES_NO + 1];
	printf("Is the %c reverse coloured? ", code[1]);
	scanf("%s", reversed);
	reversed[MAXL_YES_NO] = '\0';
	if(strcmp(reversed, "yes") == 0)
		_black = false;
	
	return HAZCHEM_init(code, _black);
}

int main(int argc, char const *argv[]) {
	
	HAZCHEM a = read();
	puts("");
	HAZCHEM_decode(a);

	return 0;
}