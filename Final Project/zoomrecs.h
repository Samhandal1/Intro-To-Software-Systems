#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ZOOMRECSH
#define ZOOMRECSH

// Samantha Handal 260983914

// struct from assignment instructions
struct ZoomRecord
{
  char email[60]; 	// email of the student
  char name[60]; 	// name of the student
  int durations[9]; 	// duration for each lab.
  struct ZoomRecord *next;
};

// prototypes of all the functions used in script
void printList(struct ZoomRecord *p);
void addZoomRecord(struct ZoomRecord **head, char *email, char *name, int duration, char letter);
void generateAttendance(struct ZoomRecord **head, char *outFile);
void linkedListToString(struct ZoomRecord *node, char *returnString);
void freeMem(struct ZoomRecord *head);
#endif
