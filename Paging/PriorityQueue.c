/* PriorityQueue.c
 * Originally written by Mark Mahony and Alan Cranbury, October 5th, 2012
 * Modified October 11th, 2012 by Mark Mahony.
 * Now implements a priority queue linked list.
 * Includes queue functions written by Jack Cole. (InitQueue, isQueueEmpty,DeleteFromQueue)
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h> // for rand()

#define LENGTH 140	// Length of message in buffer	
#define UNKNOWN 25	// Unknown purpose 
#define FORTUNETXT	"G:\\Visual Studio 2010\\Projects\\2012-2013\\TxtMessagePriority\\Debug\\FortuneCookies.txt"
#define READMODE	"r"
#define NEWMSGCODE  "%%"
#define NUMMSG		15
#define PRIORITYRANGE 7 // Allowable values for the priority of a message.

struct message {
	char txt [LENGTH];
	short senderID;
	short receiverID;
	char priority;
	short msgNum;
	char notSure [UNKNOWN];
	struct message * next; 
};

typedef struct message Msg;
Msg * tail, * head;

void GetMessageFromFile(char *buffer,int iLen);
void InitQueue (void);
int isQueueEmpty (void);
void AddToQueue (Msg *newMsg, int priority, int numMsg);
Msg *DeleteFromQueue (void);

void main(int argc, char *argv[]) {	
	char *buffer = (char*)malloc(sizeof(char) * LENGTH);
	int i, numMsg;
	Msg *newMsg;

	// seed rand with time so we don't get the same numbers
	srand( time(NULL) );

	numMsg = NUMMSG;

	// get number of messages from the command line
	if(argc == 2){
		numMsg = atoi(argv[1]);
	}else if(argc > 2)
		printf("\n Usage: txtmessage.exe [Number of Messages] \n");

	// add the messages to the queue
	for(i = 0; i < numMsg; i++){

		// get the message from the file
		GetMessageFromFile(buffer, LENGTH);

		// fill out the message structure
		newMsg = (Msg *)malloc(sizeof(struct message));
		if (newMsg == NULL)
			printf("ERROR COULD NOT MALLOC");
		strcpy(newMsg->txt, buffer);
		newMsg->senderID = 2*i;
		newMsg->receiverID = 2*i+1;
		newMsg->msgNum = i+1;
		newMsg->priority = rand() % PRIORITYRANGE;
		newMsg->next = NULL;

		// add the message to the queue
		AddToQueue(newMsg, newMsg->priority, numMsg);
	}

	// print the messages to the screen
	for(i = 0; i < numMsg; i++){
		Msg *message = DeleteFromQueue();
		printf("\n Message Number: %d Message Priority %d %s\n", message->msgNum, message->priority, message->txt);
	}

	
}

void GetMessageFromFile (char *buffer, int iLen) {
	FILE *fp;
	int i;
	int shortMsg = 0;
	int randomSpot;
	int msgCount = 0;
	char prevC = NULL, currC;
	
	// open the file and check if we've opened it
	fp = fopen(FORTUNETXT, READMODE);
	if(fp == NULL){
		printf("ERROR File was not opened successfully.");
		exit(0);
	}

	// get the first character
	currC = fgetc(fp);

	// loop to check number of messages in file
	while(currC != EOF){
		if(prevC == currC && currC == '%'){
			msgCount++;
		}
		prevC = currC;
		currC = fgetc(fp);
	}

	// get a random number for the message to print
	randomSpot = rand() % msgCount;
	printf(" %d", randomSpot);

	// go back to the beginning of the file
	fseek(fp, 0l, SEEK_SET);
	msgCount = 0;

	prevC = NULL;
	currC = fgetc(fp);

	// seek to the message we want
	while(msgCount != randomSpot){
		if(prevC == currC && currC == '%'){
			msgCount++;
		}
		prevC = currC;
		currC = fgetc(fp);
	}

	// fill the buffer with the first LENGTH characters of the message
	for(i = 0; i < iLen - 1; i++){
		buffer[i] = currC;
		prevC = currC;
		currC = fgetc(fp);
		if(currC == '%' && currC == prevC){
			buffer[i] = NULL;
			shortMsg = 1;
			break;
		}
	}

	if(!shortMsg)
		buffer[iLen - 1] = NULL;

	// close the file
	fclose(fp);
}


void InitQueue (void) {
	head = NULL;
	tail = NULL;
}

int isQueueEmpty (void) {
	return (head == NULL);
}

void AddToQueue (Msg *newMsg, int priority, int numMsg) {
	int i;
	Msg * curMsg, * prevMsg;					// Current message pointed at and previous message pointed at.

	curMsg = head;					// Set the current message.
	prevMsg = NULL;					// Set the prevMsg as nothing.
	

	if (head == NULL) {				// If queue is empty, insert new message at the beginning.
		head = newMsg;				
	}	
	else {							// If there are already other nodes,
		while (curMsg != NULL){ // Walk the list until the correct priority is found.			
			if (curMsg->priority >= newMsg->priority){ // if the priority of the new message is greater or the same as the current position.				
				newMsg->next = curMsg;
				if (prevMsg == NULL)	//If at the beginning of the list
					head = newMsg;// Make the new message the head.
				else		// if not at the beginning of the list,
					prevMsg->next = newMsg; // make the previous point to the new message.
				break;				
			}
			prevMsg = curMsg;
			curMsg = curMsg->next;
		}	
				if (curMsg == NULL){		// If the end of the list is reached, add the message to the end.
				prevMsg->next = newMsg;
				tail = newMsg;
		}

	}


}

Msg *DeleteFromQueue (void) {
	Msg * temp;
	if (head == NULL) return (NULL);
	temp = head;
	head = head->next;
	return (temp);
}