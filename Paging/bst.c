#include "bst.h"

/* Binary Search Tree Assignment
	Alan Cranbury - October 30, 2012
	For Jack Cole
	*/

TreeNode * root = NULL;


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


/*TreeNode *BSTBalance(TreeNode *tree){
{
	use height to find the height

	iterate to the middle rung, set which ever one youre at to the head of the new tree
	insert each item from the old tree into the new tree and free it from the old tree


}*/

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
