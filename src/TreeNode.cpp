#include "TreeNode.h"
using namespace std;

TreeNode::TreeNode() : isRed(true), left(nullptr), right(nullptr) {}

redBlackTree::redBlackTree() : root(nullptr) {}

void redBlackTree::insert(TreeNode* node) {
	// Implementation of the insert function for the red-black tree
	if (root == nullptr) {
		root = node;
		root->isRed = false; // The root node must be black
	} else {
		// Insert the node into the tree and balance it
		TreeNode* parent = nullptr;
		TreeNode* current = root;
		while (current != nullptr) {
			parent = current;
			if (node->movieID < current->movieID) {
				current = current->left;
			} else {
				current = current->right;
			}
		}
		if (node->movieID < parent->movieID) {
			parent->left = node;
		} else {
			parent->right = node;
		}
		balanceInsert(node);
	}
}