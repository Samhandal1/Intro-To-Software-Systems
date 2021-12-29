#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zoomrecs.h"

// Samantha Handal 260983914

// variable of type ZoomRecord which contains the referance to the head of the linked list
struct ZoomRecord *head;

int main(int argc, char* argv[]) {
	
	// if 2 arguments aren't passed to the program, print usage message
	if (argc != 3) {
		printf("Usage ./labapp <sourcecsv> <outputcsv> \n");
		return 1;
	}

	// inputFile is a file variable which holds referance to the input csv file
	// using fopen, we open the inputted csv file using the read only option (just need
	// to copy the rows of the file, we don't write to it)
	FILE* inputFile;
	inputFile = fopen(argv[1], "r");
	
	// fopen returns null if it is not opened succesully/you cant open it
	// if we could no open the csv file, print error message and return with code 1
	if (inputFile == NULL) {
		printf("Error! Unable to open the input file %s \n", argv[1]);
                return 1;
	} 
	
	// header variable is used to store the first line of the csv file (which we don't need)
	char header[100];
	
	// removeing the header of the file we are reading from so we can access straight away the 
	// first line of data to feed to addZoomRecs()
	char *temp = fgets(header, 100, inputFile);
		
	// initializing variables which we will need for our list creation
	// currLine: array which stored the extracted lines from the input file 
	// (which we then need to parse into our linked list nodes)	
	char currLine[200];

	// using fgets, we get each line from the input file (each line is a new student's information), 
	// we check to see if the funtion returns null as it would have reached the end of the file
	// we simutaniously get acess to the content of the current line through the currLine var
	while(fgets(currLine, 200, inputFile) != NULL) {
		
		// using strtok with comas as delimeters, we parse each feild in the line and copy them
		// to temporary variables
		char *email = strtok(currLine, ",");
		char *name = strtok(NULL, ",");
		char *labString = strtok(NULL, ",");
		char *durationString = strtok(NULL, ",");
  		
		// since strtok only returns strings, we need to cast the duration feild to an int using
		// atoi, and since a string also has a end of string char in it, we need to take the first
		// index of the lab letter string and store it in a char variable
		char lab = labString[0];
		int duration = atoi(durationString);
	
		// we can now make a new node using these variables as feilds for our new node
		// addZoomRecords takes the adress of the head variable so we can change the referances
		// stored within in (as we must sort the list as we add a node)
		addZoomRecord(&head, email, name, duration, lab);
	}

	//since we no longer need the input file (extracted all nessesary information, we can close it)
	fclose(inputFile);
	
	// funtion to retreive information from the linked list and print to output file, we take the
	// adress of the head and output file to which we want to print to (which the user specifies
	// when they run the program - argv[2])
	generateAttendance(&head, argv[2]);
	
	// once we are done with the linked list, we free all the dynamically allocated memory we retrieved
	// throughout the program
	freeMem(head);
	
	return 0;
}
