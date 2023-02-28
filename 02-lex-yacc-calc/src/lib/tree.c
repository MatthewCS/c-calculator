#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "tree.h"

void _inner_print_tree(TreeNode* tree, int level);

TreeNode* build_tree(
    int type, long double ldval, char* vname, int opval,
    TreeNode* left_child, TreeNode* right_child
)
{
  TreeData new_data = {
    .type = type,
    .ldval = ldval,
    .vname = (char*)malloc(sizeof(vname)+1),
    opval = opval
  };
  strcpy(new_data.vname, vname);
  TreeNode* new_tree = (TreeNode*) malloc(sizeof(TreeNode));

  new_tree->data = new_data;
  new_tree->left_child = left_child;
  new_tree->right_child = right_child;

  return new_tree;
}

void print_tree(TreeNode* tree)
{
  _inner_print_tree(tree, 0);
}

void _inner_print_tree(TreeNode* tree, int level)
{
  if(tree == NULL)  return;

  for(int i = 0; i < level; ++i)
    fprintf(stderr, "  ");

  if(tree->data.type == T_NUM)
    fprintf(stderr, "%d [NUM       : %Lf]\n", level+1, tree->data.ldval);
  else if(tree->data.type == T_VARIABLE)
    fprintf(stderr, "%d [VARIABLE  : \"%s\"]\n", level+1, tree->data.vname);
  else if(tree->data.type == T_ADDOP)
    fprintf(
        stderr, "%d [ADDOP     : %d (%c)]\n",
        level+1, tree->data.opval, (char)tree->data.opval
    );
  else if(tree->data.type == T_MULOP)
    fprintf(
        stderr, "%d [MULOP     : %d (%c)]\n",
        level+1, tree->data.opval, (char)tree->data.opval
    );
  else
    fprintf(
        stderr, "%d [UNKNOWN   : (type: %d) (numval: %Lf) (vname: \"%s\") (opval: %d (%c))]\n",
        level+1,
        tree->data.type, tree->data.ldval,
        tree->data.vname, tree->data.opval, tree->data.opval
    );

  _inner_print_tree(tree->left_child, level + 1);
  _inner_print_tree(tree->right_child, level + 1);
}

void free_tree(TreeNode* tree)
{
  if(tree == NULL)
    return;

  free_tree(tree->left_child);
  free_tree(tree->right_child);
  free(tree);
}
