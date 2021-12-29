#include <stdio.h>

/*
 Program to implement a text cipher 
 ***************************************************************
 * Author        Dept.        Date          Notes 
 ****************************************************************
 * Samantha H    Soft. Eng.   Mar 10 2021   Invalid input exceptions.
 * Samantha H    Soft. Eng.   Mar 11 2021   Finished writting.
 * Samantha H    Soft. Eng.   Mar 12 2021   Debugging incription.
 * Samantha H    Soft. Eng.   Mar 15 2021   Minor Debugging and final touches.
*/

// defining boolean values...
#define false 0
#define true !false

int main(int argc, char* argv[]) {
	
	// if 3 arguments aren't passed to the program, print usage message
	if (argc != 4) {
		printf("Usage : ./cipher [-e|-d] <key> <MESSAGE>\n");
		return 1;
	}
	
	// getting the length of the inputted key array
	// while the last char in the array isn't the end char ('0') increment length var	
	int KeyArrLen = 0;
	int i = 0;
	while(argv[2][i] != '\0') {
		KeyArrLen++;
		i++;
	}
	
	// if inputted key is any char string greater than 2, then weather it's a number or not it
	// will not be in range
	if (KeyArrLen > 2) {
		printf("Error %s is not a valid key\n", argv[2]);
                return 1;
	}
	
	// var which will hold the key value if tests r passed
	int key;	

	// the only 2 digit number which is acceptable to be a key is 10, so if the first char in the
	// string isn't 1 and second isn't 2, then the inputted number for key isn't 10 and therefore
	// isn't in range
	if (KeyArrLen == 2) {
		if (argv[2][0] != '1' && argv[2][1] != '0') {
			printf("Error %s is not a valid key\n", argv[2]);
                	return 1;
		}
		key = 10;
	}
	
	// if only one char was entered for the key, check to see if it's in range by changing the
	// value of the character to an integer, since we are subtracting the ASCII value of the zero
	// character, we will obtain a numeric value for whatever char was entered, so we are checking
	// simutaniously for weather the char entered was a number, AND if the char entered was in the range
	// lets say an k was inputted... 'k' - '0' = 59 which isnt in range because it's not a number
	if (KeyArrLen == 1) {
		char keyin = argv[2][0];
		key = keyin - '0';

		if (key < 2 || key > 10) {
                	printf("Error %s is not a valid key\n", argv[2]);
                	return 1;
		}
	}

	// getting the length of the inputted encode or decode array
        // while the last char in the array isn't the end char ('0') increment length var
        int EDArrLen = 0;
        int l = 0;
        while(argv[1][l] != '\0') {
                EDArrLen++;
                l++;
        }
	
	// if the string inputted for weather we want to encode or decode does not have a length of 2, 
	// then the input is invalid as you can only input -e or -d 
	if (EDArrLen != 2) {
		printf("Error %s is not a valid option\n", argv[1]);
                return 1;
	}
	
	// if the first char in the string isn't the dash, then input isn't valid 
	if (argv[1][0] != '-') {
		printf("Error %s is not a valid option\n", argv[1]);
                return 1;
	}

	// if the second char in the string isn't 'e' or 'd' (encode or decode), then input isn't valid
	if (argv[1][1] != 'e' && argv[1][1] != 'd') {
		printf("Error %s is not a valid option\n", argv[1]);
                return 1;
	}
	

	// since we don't implement decode, we quit the program with a success code
	// without any output (nothing is done...) 
	if (argv[1][1] == 'd') {
		printf("\n");
		return 0;
	}

	// get length of the code message
	int CodeArrLen = 0;
	int c = 0;
	while (argv[3][c] != '\0') {
		CodeArrLen++;
		c++;
	}
	
	int level;			// position variable, what level we are at

	for (level = 1; level <= key; level++) {
		
		// we start by going through each level... (the number of levels = key)
		// starting with the top level (ie hight/top/first row), then going down until we get
		// to the bottom row, after bottom row the message should be encoded
		//
		// position is the variable which will indicate what index we are at / should
		// be printed, while alternate is a makeshift boolean var which will keep track of what 
		// part of the zig zag we are on... 
		//
		// we can pinpoint levels in the zig zag by using it's hight/key, if we are at the top or
		// bottom of the zig zag (ie level = 1 or level = key), we know that we need to move through
		// key * 2 number of indicies before we get to the next index within the current level, 
		// this number will stay the same thourought the word... although if we are somewhere between 
		// the bottom or top, the number of indicies we need to cross alternate for each hump in the
		// zig zag
		//
		// this is where the alternate variable comes in handy, after going through the first hump
		// we need to switch the amount of indicies we itterate through
		//
		// for example... lets say we have a code with a key of 4, when encoding the the 2nd level
		// we move 4 indicies (2 down, 2 up) to get to the next letter (from H to S), 
		// although to get the next letter on this level, we need to only move 2 indicies 
		// (1 up, 1 down... from S to S).
		//
		// T * * * * * A * * * * * T * * * * * G *
		// * H * * * S * S * * * E * M * * * A * E
		// * * I * I * * * E * R * * * E * S * * *
		// * * * S * * * * * C * * * * * S * * * *

		int position = level - 1;
		int alternate = true;

		while (position < CodeArrLen) {
			
			// print the index
			putchar(argv[3][position]);
			
			// if you are on the last or first level
			// move position variable to 2 times the hight of the zig zag (since we start 
			// at index 0... we do key - 1
			if (level == 1 || level == key) {
				
				position += 2 * (key - 1);
			
			// if we are on the first itteration of a level that is not the first or last
			// we take the number of letters left until we reach the bottom hump, and 
			// multiply by 2, make alternate = false so that in the next itteration we count
			// the next hump 
			} else if (alternate) {
				
				position += 2 * (key - level);
				alternate = false;

			// we take the number of letters left until we reach the top hump and multiply 
			// by 2 to get the next letter in the level, we make alternate = false so we go
			// back the the first else if once we are done
			} else {

				position += 2 * (level - 1);
				alternate = true;
			}
		}
	}
	
	printf("\n");	
	return 0;
}
