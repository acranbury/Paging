#include "bst.h"
/* Binary Search Tree Assignment
	Alan Cranbury - October 30, 2012
	For Jack Cole
	*/

TreeNode * root = NULL;

/*void main(int argc, char *argv[]){
	char *buffer = (char*)malloc(sizeof(char) * LENGTH);
	int i, numMsg, searchKey;
	Item *newItem, *searchItem = NULL;
	Msg *newMsg;

	// seed rand with time so we don't get the same numbers
	srand( time(NULL) );

	numMsg = NUMMSG;
	searchKey = SEARCHKEY;

	// get number of messages from the command line
	if(argc >= 2){
		numMsg = atoi(argv[1]);
	}if(argc == 3){
		searchKey = atoi(argv[2]);
	}else if(argc > 3)
		printf("\n Usage: bst [Number of Messages] [Search Key]\n");

	// initialize the BST
	root = NULL;

	// add the messages to the BST
	for(i = 0; i < numMsg; i++){

		// get the message from the file
		GetMessageFromFile(buffer, LENGTH);

		// fill out the message structure
		if(!(newMsg = (Msg *)malloc(sizeof(struct message)))){
			printf("Error: Malloccing newMsg failed.");
			exit(0);
		}
		strcpy(newMsg->txt, buffer);
		newMsg->senderID = 2*i;
		newMsg->receiverID = 2*i+1;
		newMsg->msgNum = i+1;

		// fill out the item structure
		if(!(newItem = (Item *)malloc(sizeof(struct Item)))){
			printf("Error: Malloccing newItem failed.");
			exit(0);
		}
		newItem->data = newMsg;
		newItem->key = rand() % numMsg;
		// add the message to the queue
		root = BSTInsert(root, newItem);
	}
	
	// search for an item and print it
	
	printf("\nSearching for key: %d ...", searchKey);
	if(!(searchItem = BSTSearch(root, searchKey))){
		printf("\n Key not found");
	}
	else
		printf("\n Here it is: %s\n", ((Msg *)searchItem->data)->txt);

	// print out the BST in sorted order
	printf("\n");
	BSTPrint(root);

	printf("\nThe height of the tree is: %d\n", HeightBST(root));
	printf("\nThere are %d items in the tree.\n", CountBSTNodes(root));

}

// prints out the tree in sorted order
void BSTPrint(TreeNode *tree)
{
	if ( tree == NULL ) return;		// reached leaf
	BSTPrint( tree->pLeft );
	printf("Key: %d\n", tree->item->key );
	printf("Message: %s\n", ((Msg *)tree->item->data)->txt);
	BSTPrint( tree->pRight );
	return;
}

// gets a random 140 char message from FortuneCookies.txt
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
} */

// creates and sets up a new BST
TreeNode * CreateBST(Item *item, TreeNode * left, TreeNode * right) {
	TreeNode * newBST;
	if(!(newBST = (TreeNode *)malloc(sizeof(struct TreeNode)))){
		printf("Error: Mallocing newBST failed.");
		exit(0);
	}

	// fill out the new BST
	newBST->item = item; 
	newBST->pLeft=left;
	newBST->pRight = right;
	return( newBST );
}

// search for a key in the BST
Item  * BSTSearch(TreeNode * tree, int searchKey ) {
	// easy, tree is null, so it's not here
	if ( tree == NULL ) return( NULL );

	// found it! return the item pointer
	if ( tree->item->key == searchKey ) return( tree->item );
	if ( tree->item->key > searchKey ) return( BSTSearch(tree->pLeft,searchKey));
	else		   return( BSTSearch(tree->pRight,searchKey));
}

// inserts an item at the wanted location
TreeNode *BSTInsert(TreeNode * tree, Item * newItem) {
	// empty tree case, make a new tree with this item as the root
	if ( tree == NULL ) return( CreateBST(newItem, NULL, NULL) ); 

	// duplicates go to the right
	if ( newItem->key  < tree->item->key ) 
		tree->pLeft = BSTInsert(tree->pLeft, newItem);
	else		   
		tree->pRight = BSTInsert(tree->pRight, newItem);
	return tree;
}

TreeNode *BSTBalance(TreeNode *tree){
	/*
	use height to find the height

	iterate to the middle rung, set which ever one youre at to the head of the new tree
	insert each item from the old tree into the new tree and free it from the old tree
	*/
}

// counts the nodes in a BST
int CountBSTNodes(TreeNode * tree){
	if(tree == NULL) return 0;
	return( CountBSTNodes(tree->pLeft) + CountBSTNodes(tree->pRight) + 1 );
}

// determines the height of a BST
int HeightBST(TreeNode * tree){
	int left, right;

	// base case, return -1 to offset the + 1 at the end
	if(tree == NULL) return -1;

	// get the heights of the left and right subtrees
	left = HeightBST(tree->pLeft);
	right = HeightBST(tree->pRight);

	// return the length of the longest one plus this node
	if(left > right) return left + 1;
	else return right + 1;
}
