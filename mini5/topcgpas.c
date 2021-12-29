/*
 * Program to generate a report on the students having the top CGPAs
 *****************************************************************
 * Author  	  Dept.		  Date		  Notes
 *****************************************************************
 * Samantha H	  Soft. Eng.	  Apr 6 2021	  Started condition checks
 * Samantha H	  Soft. Eng.	  Apr 7 2021	  Linked List
 * Samantha H	  Soft. Eng.	  Apr 8 2021	  Debugging
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defining boolean values...
#define false 0
#define true !false

// defining prototypes...
struct StudentRecord* makeRecord(struct StudentRecord *ptr, long sid, char *email, char *lname, char *fname, float cgpa);

void printNodes(struct StudentRecord *ptr);

void sortStudentRecords(struct StudentRecord *head, int listLen);

void freeNodes(struct StudentRecord *ptr);

void swap (struct StudentRecord *n1, struct StudentRecord *n2);

void linkedListToString(struct StudentRecord *node, char *returnString);

// struct for linked list nodes (from instructions)...
struct StudentRecord {
	long sid;
	char email[30];
	char lname[20];
	char fname[20];
	float cgpa;
	struct StudentRecord *next;
};

int main(int argc, char* argv[]) {
	
	// if 2 arguments aren't passed to the program, print usage message
	if (argc != 3) {
		printf("Usage ./topcgpas <sourcecsv> <outputcsv> \n");
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
	char header[28];
	
	// fgets checks to see if the first line is null, if there is no content in the first line the
	// file is empty, if there is it still gets stored to header variable and used in the next check.
	// if file is empty, print error message and return with code 1
	if (fgets(header, 28, inputFile) == NULL) {
		printf("Error! Input CSV file %s is empty\n", argv[1]);
		return 1;
	}
	
	// rather than using feof() or checking the last index of the header variable, the most effective
	// way I found to retreive the EOF in a file that only has the header is by storing the EOF character
	// inside a variable using getc. Since fgets() used in the previous check placed the file right before
	// where the EOF should be (assuming the csv file is the same as in the instructions)
	int c = getc(inputFile);
	
	// here we check if the end of the first line is the end of the file, using the variable c which 
	// would hold EOF is it is the last line in the file and anything else if not, we print an 
	// error message and exit with a code of 1
	if (c == EOF) {
		printf("Error! Input CSV file %s is empty\n", argv[1]);
		return 1;
	}

	// initializing variables which we will need for our list creation
	// head: head of list, listLength: length of list, currLine: array which stored the extracted lines
	// from the input file (which we then need to parse into our linked list nodes)	
	struct StudentRecord *head = NULL;
	int listLength = 0;	
	char currLine[100];
	
	// using fgets, we get each line from the input file (each line is a new student's information), 
	// we check to see if the funtion returns null as it would have reached the end of the file
	// we simutaniously get acess to the content of the current line through the currLine var
	while(fgets(currLine, 100, inputFile) != NULL) {
		
		// using strtok with comas as delimeters, we parse each feild in the line and copy them
		// to temporary variables
		char *sidString = strtok(currLine, ",");
		char *email = strtok(NULL, ",");
		char *lname = strtok(NULL, ",");
		char *fname = strtok(NULL, ",");
		char *cgpaString = strtok(NULL, ",");
  		
		// since strtok only returns strings, we need to cast the sid and cgpa feild to their
		// respective data types (long and float)
		long sid = atol(sidString);
		float cgpa = atof(cgpaString);
		//float cgpa = roundCGPA(cgpaNotRound);
	
		// we can now make a new node using these variables as feilds for our now node
		// to be able to get the previous node, we always feed makenode head var, which after creation
		// gets stored back into the head variable. Since we are making our list backwards (for simplicity)
		// the head will point to the last element in the linked list
		head = makeRecord(head, sid, email, lname, fname, cgpa);

		// after a node is added, we increment the length of the list
		listLength++;	
	}

	//since we no longer need the input file (extracted all nessesary information, we can close it)
	fclose(inputFile);
	
	// sort the linked list we obtained from the input file
	sortStudentRecords(head, listLength);
	
	// by using two file variables, we can first check if the file exists with the read comand
	// then open the file for writting so we can copy the sorted nodes into it
	//FILE* outputFile;
	FILE* testOutputFile;
	testOutputFile = fopen(argv[2], "r");
	//outputFile = fopen(argv[2], "w");
	
	// check if we can open file (fopen return null if we can't open) print error and exit with code 1
        if (testOutputFile == NULL) {
                printf("Error! Unable to open the output file %s \n", argv[2]);
		// if we exiting we should also free the nodes we created as we wont be using them
		freeNodes(head);
                return 1;
        }
	
	// opening for writting after we check if it exists, or else another file will be created
	FILE* outputFile;
	outputFile = fopen(argv[2], "w");
		
	// write header to output file
	fputs("sid,email,cgpa", outputFile);
	fputs("\n", outputFile);
	
	// temporary node which is used to itterate through the list
	struct StudentRecord *itterator = head;
	
	// if the list has less than 5 elements in it, print all the elements
	if (listLength < 5) {
		
		// since we know it is less than 5 elements, we can just itterate though them
		// until there are no nodes left to go though
		while (itterator != NULL) {	
			
			// buffer char which holds the converted node data in a string format
			// using fputs we put that line which was converted from linkedListToString
			// we then move the itterator down and repeat until no more nodes
			char listToString[150];
                	linkedListToString(itterator, listToString);
                	fputs(listToString, outputFile);
                	fputs("\n", outputFile);
                	itterator = itterator->next;
		}

	} else {
		
		// otherwise we only want the people with the top 5 gpas, so we reapeat the itteration
		// only 5 times (controlled with for loop)
		// var prevNode is there to check is the value of the 5th cgpa is the same as the 6th
		// do the same process as less than 5 elements to tranfer data to output file
		int k;
		float prevNode;
		for (k = 1; k <= 5; k++) {
		
			char listToString[150];
			linkedListToString(itterator, listToString);
			fputs(listToString, outputFile);
			fputs("\n", outputFile);
			
			prevNode = itterator->cgpa;

			itterator = itterator->next;
		}
		
		// if the 5th highest cgpa is the same as the 6th, then we also have to include it on the
		// file, opening a for loop to continue printing the extra data aslong as they have the same
		// value as the 5th... the prevNode var stores the 5th value and we copy the data the same way
		// as the other two cases
		if (prevNode == itterator->cgpa) {
			while (prevNode == itterator->cgpa) {
				
				char listToString[150];
                        	linkedListToString(itterator, listToString);
                        	fputs(listToString, outputFile);
                        	fputs("\n", outputFile);

                        	prevNode = itterator->cgpa;

                        	itterator = itterator->next;
			}
		}
	}
	
	// close output file and free all allocated memory
	fclose(outputFile);		
	freeNodes(head);
	
	return 0;
}

// function is passed a referance to the previous node in the list, along with the feild it should have upon creation
struct StudentRecord* makeRecord(struct StudentRecord *ptr, long sid, char *email, char *lname, char *fname, float cgpa) { 
	
	// making + allocating space (the size of a node) for a new node
	struct StudentRecord *aStudentRecord;
        aStudentRecord = (struct StudentRecord*)malloc(sizeof(struct StudentRecord));
	
	// when malloc runs out of space, it returns null
	// in this case we need to notify the user (print error message and exit with code of 1)	
	if (aStudentRecord == NULL) {
		printf("Error! program ran out of memory");
                exit(1);
	}
	
	// copying information to fields in nodes, we use strcpy for char strings because if we just point to
	// the node it wont actually copy the data
	aStudentRecord->sid = sid;
	strcpy(aStudentRecord->email, email);
	strcpy(aStudentRecord->lname, lname);
	strcpy(aStudentRecord->fname, fname);
	aStudentRecord->cgpa = cgpa;
	
	// if first node in list, then next feild doesnt point to anything
	if (ptr == NULL) {
		aStudentRecord->next = NULL;
	} else {
	// else it points to the previous node (copying the list from the input file backward)
		aStudentRecord->next = ptr;
	}

	return aStudentRecord;
}

// for testing purporses, to print all nodes in the list
void printNodes(struct StudentRecord *ptr) {
	while (ptr != NULL) {
		printf("%ld - %s - %s - %s - %.1f \n", ptr->sid, ptr->email, ptr->lname, ptr->fname, ptr->cgpa);
		ptr = ptr->next;
	}
}

// free allocated space using free once we are done
void freeNodes(struct StudentRecord *ptr) {

	struct StudentRecord *holder = NULL;
	
	// removing each node by holder the referance to the next one in holder var
        while (ptr != NULL) {
		holder = ptr->next;
                free(ptr);
		ptr = holder;
        }
}

// function for sorting linked list (using bubblesort algorythm), takes the referance to the head of the list
// and its length
void sortStudentRecords(struct StudentRecord *head, int listLen) {
	
	// temporary nodes used for comparing cgpas
	struct StudentRecord *currentRecord = NULL; 
	struct StudentRecord *check = NULL;
	
	// if the list is empty, nothing should be returned
	if (head == NULL) {
		return;
	}
	
	// bubble sort
	// first we initialize the list to not sorted and number of node gone through as 0 
	int sorted = false;
	int i = 0;

	// while list is not sorted...
	while (!sorted) {
		
		sorted = true;
		
		// place currentRecord at top of list and check at next done so they are next to eachother
		currentRecord = head;
		check = currentRecord->next;
		
		// we itterate though the list O(n^2) times, each time going through a smaller section of the list
		// length - numOfItterations - 2
		int j;
		for (j = 0; j <= (listLen-i-2); j++) {
			
			// if the cgpa of the currentRecord is higher than that of check, then swap them
			// set sorted to false
			// currentRecord should always be before check
			if (currentRecord->cgpa < check->cgpa) {
				swap(currentRecord, check);
				sorted = false;
			}
			
			// continue down the list
			currentRecord = currentRecord->next;
                        check = check->next;

		}

		// increment amonut of times we have gone though the list
		i++;
	}
}

// swap function which swaps two nodes in the list
void swap (struct StudentRecord *n1, struct StudentRecord *n2) {
	
	// rather than swaping the nodes themselves, it is a lot easier just to swap the data in the nodes
	// so by creating a temp var for each feild in the node then using it to preform the swap
	long tempSid = n1->sid;
	n1->sid = n2->sid;
	n2->sid = tempSid;
	
	// without memcpy, and using a pointer rather than a fixed array
	// we get segment core dumps... so easiest way to fix this was with
	// memcpy instead
	char tempEmail[30]; 
	memcpy(tempEmail, n1->email, sizeof(tempEmail));
	memcpy(n1->email, n2->email, sizeof(tempEmail));
	memcpy(n2->email, tempEmail, sizeof(tempEmail));

	char tempLName[20];
	memcpy(tempLName, n1->lname, sizeof(tempLName));
	memcpy(n1->lname, n2->lname, sizeof(tempLName));
	memcpy(n2->lname, tempLName, sizeof(tempLName));

	char tempFName[20]; 
	memcpy(tempFName, n1->fname, sizeof(tempFName));
	memcpy(n1->fname, n2->fname, sizeof(tempFName));
	memcpy(n2->fname, tempFName, sizeof(tempFName));

	float tempCGPA = n1->cgpa;
	n1->cgpa = n2->cgpa;
	n2->cgpa = tempCGPA;

}

// helper method to turn the data inside the nodes back to strings so we can print them to the output file
void linkedListToString(struct StudentRecord *node, char *returnString) {
	
	// convert float and long to char, %.1f specifies the amount of decimal places we want
	char floatBuf[32];
	sprintf(floatBuf, "%.1f", node->cgpa);

	char longBuf[100];
	sprintf(longBuf, "%ld", node->sid);
	
	// we copy the first peice of data onto the line then append the rest with strcat
	// including comas
	strcpy(returnString, longBuf);
	strcat(returnString, ",");
	strcat(returnString, node->email);
	strcat(returnString, ",");
	strcat(returnString, floatBuf);
}
