#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zoomrecs.h"

// Samantha Handal 260983914

// helper method to print nodes in list (inpired by class notes)
void printList(struct ZoomRecord *p) {
	
	while (p != NULL) {
		printf("%s - %s - %d - %d \n", p->email, p->name, p->durations[0], p->durations[3]);
		p = p->next;
	}
}

// addZoomRecords takes as parameters all the information to be stored in the node, which can either be
// to make the node or find the referance to the already existing node
// we take the head of the list as a double pointer because we want to change the content of the original
// head node, so we need to take in the adress of the original head in labapp.c
// i learnt more about doubble pointers from : https://www.educba.com/double-pointer-in-c/
void addZoomRecord(struct ZoomRecord **head, char *email, char *name, int duration, char letter) {
	
	// to get the index of the lab so we where where to store the duration variable we subtarct
	// the ASCII value of capital A and subtract it from the char letter (this will give us proper index)
	// ie index of A ---> index = A - A = 65 - 65 = 0, therefore we store lab A data in durations[0]
	int index = letter - 65;
	
	// if there is no head/the list is empty, we make a head
	if (*head == NULL) {
		
		// allocating memory the size of a ZoomRecord
		struct ZoomRecord *headRecord;
		headRecord = (struct ZoomRecord*)malloc(sizeof(struct ZoomRecord));
	
		// if the program runs out of space the newly allocated ZoomRecord would be null
		if (headRecord == NULL) {
			printf("Error! program ran out of memory");
			exit(1);
		}
		
		// we use strcpy to copy the strudent information that are strings into the fields in the nodes
		// for integers we do not need to use strcpy as their size is fixed
		strcpy(headRecord->email, email);
		strcpy(headRecord->name, name);
		headRecord->durations[index] = duration;
		
		// we then set the new node we made as the head and exit the function
		headRecord->next = NULL;
		*head = headRecord;

		return;
	}

	// if we need to insert a new node at the head of the list but there is already a head...
	// we use strcmp to check to see if the email we want to inset belongs before (alphabetically)
	// the head's email (ie sam.handal@mail.mcgill.ca is the email of the head node, but we need to insert
	// jacaint.demerse@mail.mcgill.ca, alphabetically j goes before s, therefore jacaint needs to be at the 
	// head of the list)... by the defn of strcmp if the letter of the first variable goes after the second
	// in the alphabet, a positive non zero number will be returned

	if (strcmp((*head)->email, email) > 0) {
		
		// procedure is similar to the head, we allocate memory and copy the variables
		// into the nodes
                struct ZoomRecord* biggerHead;
                biggerHead = (struct ZoomRecord*)malloc(sizeof(struct ZoomRecord));

                if (biggerHead == NULL) {
                        printf("Error! program ran out of memory");
                        exit(1);
                }

                strcpy(biggerHead->email, email);
                strcpy(biggerHead->name, name);
                biggerHead->durations[index] = duration;

		// we then set the referance to the next node of the new node we just made to the old head then
		// make the new node the head of the list (and return out of the function because we are done)
		biggerHead->next = *head;
                *head = biggerHead;

		return;

	}
	
	// if we need to insert somewhere inside the list, we make a dummy itterator node which can help us
	// itterate through the list (hold the place of where we currently are in the list)
        struct ZoomRecord* current;
        current = *head;
	
	// while there are still nodes in the list
	while (current != NULL) {
		
		// if the current node has the same email as the information of the student we want to insert, this
		// means we already have a node for this student and we either want to append a lab time or insert a 
		// new lab time for a session... strcmp returns 0 when strings are idential (which should always happen
		// as emails are standard / always lowercase)
		if (strcmp(current->email, email) == 0) {
			
			// if there is information inside the index of the lab we want to store the duration var
			// in, this means the student probably disconnected from zoom and reconnected and we need
			// to append their new lab time onto the old one to get the net lab time spend in that session
			// we then return from the function because we are done
			if (current->durations[index] != 0) {
				current->durations[index] += duration;
				return;
			}
			
			// otherwise if there is no information inside that index, we simply add the duration 
			// specified to the proper index and return from the function again (done)
			current->durations[index] = duration;

			return;

		}
		
		// we also need to check the next as when we reach the end of the list it gets messy, to make sure we
		// get all the occurances of overlap i repeated the same if staement above but for current->next->next
		if (strcmp(current->next->email, email) == 0) {

                        if (current->next->durations[index] != 0) {
                                current->next->durations[index] += duration;
				return;
                        }

                        current->next->durations[index] = duration;

                        return;

                }

		// as we itterate through the list, we check to see of the next variable of the current node has an
		// email that bellongs lower than the one we need to insert, we check the next occurance and not the 
		// current as we need to change the value of current->next feild to properly insert the node
		// (does not account for add last)
		if (strcmp(current->next->email, email) > 0) {
			
			// similar procedure as all the other add nodes checks... we allocate memory to account for
			// the new ZoomRecord and copy the information into it's nodes
			struct ZoomRecord *insert;
                	insert = (struct ZoomRecord*)malloc(sizeof(struct ZoomRecord));

                	if (insert == NULL) {
                        	printf("Error! program ran out of memory");
                        	exit(1);
                	}

                	strcpy(insert->email, email);
                	strcpy(insert->name, name);
                	insert->durations[index] = duration;
                	
			// we then set the values of the next field of the node we just made to the next feild
			// of the current node (need to insert it BEFORE the node which we checked in the if statement)
			// then we set the current->next feild to the new node we just made closing the link of the
			// list... we then exit from the function as we are done
			insert->next = current->next;
                	current->next = insert;

			return;
		}
		
		// if we need to add a node at the end of the list, we check to see if the next next referance
		// is null, we do this because if we check for the current->next, we will get a core dumps as the 
		// statement above checks the current->next->email field which we cannot do... and if the current->next
		// isn't true, then the only place left to add the node would be at the end of the list
		if (current->next->next == NULL) {
			
			// if we need to add last then we do the same as before... allocate memory for the node
			// and copy the fields into the node
			struct ZoomRecord *insertLast;
                        insertLast = (struct ZoomRecord*)malloc(sizeof(struct ZoomRecord));

                        if (insertLast == NULL) {
                                printf("Error! program ran out of memory");
                                exit(1);
                        }

                        strcpy(insertLast->email, email);
                        strcpy(insertLast->name, name);
                        insertLast->durations[index] = duration;
			
			// we then set the the referance to the next node as null (since we are at the bottom)
			// and current->next->next which is the next feild of the last ndoe in the list to the
			// new variable we have created... we then return the function as we are done
                        insertLast->next = NULL;
                        current->next->next = insertLast;

                        return;
 
		}
		
		// to catch any unwanted error, if the current->next is null we set current to null so we dont
		// enter back into the loop, otherwise we move down by one in the list make our current itterator 
		// the next element
		if (current->next == NULL) {
			current = NULL;
		} else {
			current = current->next;
		}
	}

}

// we take in the referance to the head of the list which should contain the entire linked list in sorted order
// aswell ase the name/path of the output file so we can write to it 
void generateAttendance(struct ZoomRecord **head, char *outFile) {
	
	// if the list is empty, nothing happens and we return
	if ((*head) == NULL) {
		return;
	}
        
	// opening / create the specified file for writting
        FILE* outputFile;
        outputFile = fopen(outFile, "w");

        // write header to output file
        fputs("User Email,Name (Original Name),A,B,C,D,E,F,G,H,I,Attendance (Percentage)", outputFile);
        fputs("\n", outputFile);
	
	// make a temporary variable to itterate through the linked list so we can acess the members of the
	// nodes to copy onto the output file
	struct ZoomRecord *itterator;
	itterator = *head;
	
	// while the next feild isn't null, we copy the information
	while (itterator->next != NULL) {
		
		// outputLine is a buffer to store the formatted data in the form of a string, with the help of 
		// linkedListToString helper method define bellow. We then put the outputLine which stores the linked
		// list data in the form a string to the output file one line at a time 
		char outputLine[200];
		linkedListToString(itterator, outputLine);
		fputs(outputLine, outputFile);
		fputs("\n", outputFile);

		// more the itterator to the next node
		itterator = itterator->next;
	}
	
	// since the while doesn't pick up the last node in the list, we do it manually here
	char outputLine[200];
        linkedListToString(itterator, outputLine);
	fputs(outputLine, outputFile);
        fputs("\n", outputFile);
	
	// we then close the file as we don't need it anymore
	fclose(outputFile);

}

// helper method to convert a node to a string of data (in the right format) takes in the node which needs
// to be converted and the referance to the buffer string
void linkedListToString(struct ZoomRecord *node, char *returnString) {

	// to calculate weather a student gets the grade for a given lab session we itterate through 
	// each element inside the durations feild and check to see if the time spend in that lab (durations[lab])
	// is greater than or equal to 45 (min amount of time to get grades for a given time)
	int total = 0;
        for (int j = 0; j < 9; j++) {
                if (node->durations[j] >= 45) {
                        total += 1;
        
                }
        }
	
	// get the grade for each student by dividing the number of labs they receive grades for by
	// the total number of labs, then x by 100 to get the percent
	float percentage = ((float)total/9)*100;
	
	// change the float to a string so we can append it to one string
	char totalp[100];
	sprintf(totalp, "%.2f", percentage);
	
	// change all the lab data to strings aswell so we can also append it to one big string
	char labA[100];
	sprintf(labA, "%d", node->durations[0]);
	
	// need to do this for every lab...
	char labB[100];
	sprintf(labB, "%d", node->durations[1]);

	char labC[100];
        sprintf(labC, "%d", node->durations[2]);

        char labD[100];
        sprintf(labD, "%d", node->durations[3]);

        char labE[100];
        sprintf(labE, "%d", node->durations[4]);

        char labF[100];
        sprintf(labF, "%d", node->durations[5]);

        char labG[100];
        sprintf(labG, "%d", node->durations[6]);

        char labH[100];
        sprintf(labH, "%d", node->durations[7]);

        char labI[100];
        sprintf(labI, "%d", node->durations[8]);

	// we copy the first peice of data onto the line then append the rest with strcat
	// including comas, we then get the desired output which gets copied to the beffer string
	strcpy(returnString, node->email);
	strcat(returnString, ",");
	strcat(returnString, node->name);
	strcat(returnString, ",");
	strcat(returnString, labA);
	strcat(returnString, ",");
        strcat(returnString, labB);
	strcat(returnString, ",");
        strcat(returnString, labC);
	strcat(returnString, ",");
        strcat(returnString, labD);
	strcat(returnString, ",");
        strcat(returnString, labE);
	strcat(returnString, ",");
        strcat(returnString, labF);
	strcat(returnString, ",");
        strcat(returnString, labG);
	strcat(returnString, ",");
        strcat(returnString, labH);
	strcat(returnString, ",");
        strcat(returnString, labI);
	strcat(returnString, ",");
        strcat(returnString, totalp);
}

// helper method to free nodes, takes in the head as a referance
void freeMem(struct ZoomRecord *head) {
	
	// use itterator again to itterate through and store the current head (which is not freed)
	struct ZoomRecord *itterator;
	
	// while there are still elements left in the list
	while (head->next != NULL) {
		
		// we hold the current head node inside the itterator temporary variable
		// then we store the next head variable back in the head variable and remove the head node
		// or rather free node which is held in the itterator node
		itterator = head;
		head = head->next;
		free(itterator);
	}
	
	// since the last node in the list would be the head, we just free it on it's own
	free(head);
}
