/* 
 * @copyright (c) 2020, ICT, Hanoi University of Technology
 * @author HoangThuy Ha
 * @version 1.0
 */

#ifndef __HAZCHEM_H__
#define __HAZCHEM_H__

#include <string.h>
#include <stdio.h>

#define MAXL_HAZCHEM_CODE 3

typedef enum {
	true,
	false,
} bool;

typedef struct _HAZCHEM {
	char code[MAXL_HAZCHEM_CODE+1];
	bool black;
} HAZCHEM;

typedef enum {
	material,
	reactivity,
	protection,
	containment,
	evacuation,
} HAZCHEM_attribute;

HAZCHEM HAZCHEM_init(char* _code, bool _black) {
	HAZCHEM cipher;

	cipher.black = _black;
	if(strlen(_code) > 3) {
		printf("ERROR_CodeLength\n");
		strcpy(cipher.code, "");
	}
	else {
		strcpy(cipher.code, _code);
		for(int i = 0; i < MAXL_HAZCHEM_CODE - strlen(_code); i++)
			cipher.code[strlen(_code) + i] = '_';
	}
	return cipher;
}

// Material
char* HAZCHEM_decodeFirstCharacter(char c) {
	/*
	Decode the HAZCHEM_attribute Material of  first character c.
	*/
	switch(c) {
		case '1': return "Jets";
		case '2': return "Fog";
		case '3': return "Foam";
		case '4': return "Dry agent";
		default: return "";
	}
}

// Reactivity, Protection, Containment
char* HAZCHEM_decodeSecondCharacter(char c, bool black, HAZCHEM_attribute a) {
	/*
	Decode the HAZCHEM_attribute a of  second character c.
	*/	

	// Reactivity
	if(a == reactivity) {
		switch(c) {
			case 'P': 
			case 'S': 
			case 'W':
			case 'Y':
			case 'Z':
				return "can be violently reactive";
			default:
				return "";
		}
	}
	
	// Protection
	if(a == protection) {
		if(black == false) {
			switch(c) {
				case 'S':
				case 'T':
				case 'Y':
				case 'Z':
					return "breathing apparatus, protective gloves for fire only";
				default:
					return "";
			}
		}	
		switch(c) {
			case 'P':
			case 'R':
			case 'W':
			case 'X':
				return "full protective clothing must be worn";
			case 'S':
			case 'T':
			case 'Y':
			case 'Z':
				return "breathing apparatus, protective gloves";
			default:
				return "";
		}
	}
	
	// Containment
	if(a == containment) {
		switch(c) {
			case 'P':
			case 'R':
			case 'S':
			case 'T':
				return "Dilute";
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				return "Contain";
			default:
				return "";
		}
	}
	return "";
}

// Evacuation
char* HAZCHEM_decodeThirdCharacter(char c) {
	if(c == 'E')
		return "Consider Evacuation";
	return "";
}

// Decode HAZCHEM code
void HAZCHEM_decode(HAZCHEM cipher) {
	printf("***Emergency action advice***\n");
	printf("%s: %s\n", "Material", HAZCHEM_decodeFirstCharacter(cipher.code[0]));
	printf("%s: %s\n", "Reactivity", HAZCHEM_decodeSecondCharacter(cipher.code[1], cipher.black, reactivity));
	printf("%s: %s\n", "Protection", HAZCHEM_decodeSecondCharacter(cipher.code[1], cipher.black, protection));
	printf("%s: %s\n", "Containment", HAZCHEM_decodeSecondCharacter(cipher.code[1], cipher.black, containment));
	printf("%s: %s\n", "Evacuation", HAZCHEM_decodeThirdCharacter(cipher.code[2]));
	printf("*****************************\n");
}

#endif