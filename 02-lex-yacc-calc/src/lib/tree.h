#ifndef __TREE_H__
#define __TREE_H__

typedef struct TreeItemData
{
  int type;           // token type
  long double ldval;  // numeric value
  char* vname;        // variable name
  int opval;          // operator value
} TreeData;

typedef struct TreeItem
{
  TreeData data;
  struct TreeItem* left_child;
  struct TreeItem* right_child;
} TreeNode;

TreeNode* build_tree(
    int type, long double ldval, char* vname, int opval,
    TreeNode* left_child, TreeNode* right_child
);
void print_tree(TreeNode* tree);
void free_tree(TreeNode* tree);

#endif
