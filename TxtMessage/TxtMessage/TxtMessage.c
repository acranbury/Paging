/* List of Messages
 * Written by Mark Mahony and Alan Cranbury, October 5th, 2012
 * Includes FIFO functions written by Jack Cole. (InitQueue, isQueueEmpty, AddToQueue, DeleteFromQueue)
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h> // for rand()

#define LENGTH 140	// Length of message in buffer	
#define UNKNOWN 25	// Unknown purpose 
#define FORTUNETXT	"FortuneCookies.txt"
#define READMODE	"r"
#define NEWMSGCODE  "%%"
#define NUMMSG		15

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
void AddToQueue (Msg *newMsg);
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
		strcpy(newMsg->txt, buffer);
		newMsg->senderID = 2*i;
		newMsg->receiverID = 2*i+1;
		newMsg->msgNum = i+1;

		// add the message to the queue
		AddToQueue(newMsg);
	}

	// print the messages to the screen
	for(i = 0; i < numMsg; i++){
		Msg *message = DeleteFromQueue();
		printf("\n Message Number: %d  %s\n", message->msgNum, message->txt);
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
		printf(" OH GOD, I'VE NEVER SEEN SO MUCH BLOOD!!");
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

void AddToQueue (Msg *newMsg) {
	if (head == NULL) {				// If queue is empty, insert new message at the beginning.
		head = newMsg;				
	}	
	else {							// If there are already other nodes,
		tail->next = newMsg;		// Make the old tail point to the new tail.
	}
	newMsg->next = NULL;			// Set the new tail to point to the end, NULL.
	tail = newMsg;					// The new tail is the new message.

}

Msg *DeleteFromQueue (void) {
	Msg * temp;
	if (head == NULL) return (NULL);
	temp = head;
	head = head->next;
	return (temp);
}