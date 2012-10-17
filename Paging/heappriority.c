#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LENGTH 140	// Length of message in buffer	
#define UNKNOWN 25	// Unknown purpose 
#define FORTUNETXT	"G:\\2nd Year\\Paging\\heappriority\\Debug\\FortuneCookies.txt"
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
	//struct message * next; 
};

typedef struct message Msg;

// queue structure
struct pqueue {
	int itemCount;			// number of items in the queue
	struct item **items;	// array of items in the queue
};

// item structure
struct item{
	int priority;			// priority of item
	void *data;				// void pointer to data
};

typedef struct item Item;
typedef struct pqueue PQueue;

PQueue * top;

void GetMessageFromFile(char *buffer,int iLen);
void InitPQueue(int size);
Item *DeleteMax();
void MoveDown(int k);
void Insert(Item *);
void MoveUp(int k);
int GetRight(int k);
int GetLeft(int k);
int GetParent(int k);
int IsEmpty(int k);
void Swap(int i, int j);

/*void main(int argc, char *argv[]){
	char *buffer = (char*)malloc(sizeof(char) * LENGTH);
	int i, numMsg;
	Item *newItem;
	Msg *newMsg;

	// seed rand with time so we don't get the same numbers
	srand( time(NULL) );

	numMsg = NUMMSG;

	// get number of messages from the command line
	if(argc == 2){
		numMsg = atoi(argv[1]);
	}else if(argc > 2)
		printf("\n Usage: txtmessage.exe [Number of Messages] \n");

	// initialize the priority queue
	InitPQueue(numMsg);

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

		// fill out the item structure
		newItem = (Item *)malloc(sizeof(struct item));
		newItem->data = newMsg;
		newItem->priority = rand() % numMsg;
		// add the message to the queue
		Insert(newItem);
	}

	printf("\n Priority List: ");
	for(i = 0; i < numMsg; i++){
		printf(" %d ",top->items[i]->priority);
	}

	for(i = 0; i < numMsg; i++){
		newItem = DeleteMax();
		printf("\nMessage Number: %d   Priority: %d\n\n %s \n", ((Msg *)newItem->data)->msgNum, newItem->priority, ((Msg *)newItem->data)->txt);
		free(newItem->data);
		free(newItem);
	}
}*/

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

// build the queue
void InitPQueue(int size){
	top = (PQueue *)malloc(sizeof(struct pqueue));
	top->items = (Item **)malloc(sizeof(struct item) * size);
	top->itemCount = 0;
}

// get the first thing off the queue
Item *DeleteMax(){
	Item *value;
	if (top != NULL && top->itemCount >= 1){
		// store top item
		value = top->items[0];

		// move bottom item to top
		top->items[0] = top->items[top->itemCount - 1];

		// decrement count of items
		top->itemCount--;

		// try to move the top element down to maintain order
		MoveDown(0);
		return value;
	}
}

// move a given item down the heap
void MoveDown(int k){
	int larger, right, left;

	// get the left and the right children
	left = GetLeft(k);
	right = GetRight(k);

	// make sure the current item is valid
	if(!IsEmpty(k)){

		// assume larger is left, and test
		larger = left;
		if(!IsEmpty(right))

			// if right is larger than left, set larger to right
			if(top->items[right]->priority > top->items[left]->priority)
				larger = right;
		if(!IsEmpty(larger))
			// if priority of largest child is larger than current
			// swap current and largest child, and try to move again
			if(top->items[k]->priority < top->items[larger]->priority){
				Swap(k, larger);
				MoveDown(larger);
		}
	}
}

// insert a new item on the heap
void Insert(Item *newItem){

	// put the item at the end of the queue
	top->items[top->itemCount] = newItem;

	// increment number of items
	top->itemCount++;

	// try to move the item up the queue
	MoveUp(top->itemCount-1);
}

// try to move an item up the heap
void MoveUp(int k){
	int parent;

	// get the parent of the current leaf
	parent = GetParent(k);
	if(!IsEmpty(k)){
		if(!IsEmpty(parent))
			if(top->items[k]->priority > top->items[parent]->priority){
				Swap(k, parent);
				MoveUp(parent);
		}
	}
}

// get the index of the right child
int GetRight(int k){
	if(k == 0)
		return 2;
	return 2 * k + 1;
}

// get the index of the left child
int GetLeft(int k){
	if(k == 0)
		return 1;
	return 2 * k;
}

// get the parent of the current leaf
int GetParent(int k){
	return k / 2;
}

// check if the heap (or sub heap) is empty
int IsEmpty(int k){
	return (k >= top->itemCount);
}

// swap two items in the heap
void Swap(int i, int j){
	Item *temp = top->items[i];
	top->items[i] = top->items[j];
	top->items[j] = temp;
}
