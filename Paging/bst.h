#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Item structure
struct Item{
    int key;
    void * data;
};

typedef struct Item Item;

// structure for a binary search tree
struct TreeNode{
    Item * item;
    struct TreeNode * pLeft;
    struct TreeNode * pRight;
};

typedef struct TreeNode TreeNode;

extern TreeNode * root;


void BSTPrint(TreeNode *tree);
void GetMessageFromFile (char *buffer, int iLen);
TreeNode *CreateBST(Item *item, TreeNode * left, TreeNode * right);
Item  *BSTSearch(TreeNode * tree, int searchKey );
TreeNode *BSTInsert(TreeNode * tree, Item * newItem);
TreeNode *BSTDelete(TreeNode * tree, Item * toDelete);
int CountBSTNodes(TreeNode * tree);
int HeightBST(TreeNode * tree);

#endif // BST_H
