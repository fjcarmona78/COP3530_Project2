#include "TreeNode.h"
using namespace std;

TreeNode::TreeNode() : isRed(true), left(nullptr), right(nullptr) {}

redBlackTree::redBlackTree(SortType sortType) : root(nullptr), sortBy(sortType) {}

void redBlackTree::insert(TreeNode* node) {
	// Insertion logic for the red-black tree
	if (root == nullptr) {
		root = node;
		root->isRed = false; // The root node must be black
	} else {
		// Insert the node into the tree based on the sorting criteria
		TreeNode* parent = nullptr;
		TreeNode* current = root;
		while (current != nullptr) {
			parent = current;
			if (sortBy == BY_POPULARITY) {
				if (node->popularity < current->popularity) {
					current = current->left;
				} else {
					current = current->right;
				}
			} else { // BY_REVENUE
				if (node->revenue < current->revenue) {
					current = current->left;
				} else {
					current = current->right;
				}
			}
		}
		if (sortBy == BY_POPULARITY) {
			if (node->popularity < parent->popularity) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		} else { // BY_REVENUE
			if (node->revenue < parent->revenue) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		}
		balanceInsert(node); // Balance the tree after insertion
	}
}

void redBlackTree::rotateLeft(TreeNode* node) {
	// Left rotation logic
	TreeNode* rightChild = node->right;
	node->right = rightChild->left;
	if (rightChild->left != nullptr) {
		rightChild->left->parent = node;
	}
	rightChild->parent = node->parent;
	if (node->parent == nullptr) {
		root = rightChild;
	} else if (node == node->parent->left) {
		node->parent->left = rightChild;
	} else {
		node->parent->right = rightChild;
	}
	rightChild->left = node;
	node->parent = rightChild;
}	

void redBlackTree::rotateRight(TreeNode* node) {
	// Right rotation logic
	TreeNode* leftChild = node->left;
	node->left = leftChild->right;
	if (leftChild->right != nullptr) {
		leftChild->right->parent = node;
	}
	leftChild->parent = node->parent;
	if (node->parent == nullptr) {
		root = leftChild;
	} else if (node == node->parent->right) {
		node->parent->right = leftChild;
	} else {
		node->parent->left = leftChild;
	}
	leftChild->right = node;
	node->parent = leftChild;
}

void redBlackTree::balanceInsert(TreeNode* node) {
	// Balancing logic after insertion
	TreeNode* parent = nullptr;
	TreeNode* grandparent = nullptr;
	while (node != root && node->isRed && node->parent->isRed) {
		parent = node->parent;
		grandparent = parent->parent;
		if (parent == grandparent->left) {
			TreeNode* uncle = grandparent->right;
			if (uncle != nullptr && uncle->isRed) {
				grandparent->isRed = true;
				parent->isRed = false;
				uncle->isRed = false;
				node = grandparent;
			} else {
				if (node == parent->right) {
					rotateLeft(parent);
					node = parent;
					parent = node->parent;
				}
				rotateRight(grandparent);
				swap(parent->isRed, grandparent->isRed);
				node = parent;
			}
		} else {
			TreeNode* uncle = grandparent->left;
			if (uncle != nullptr && uncle->isRed) {
				grandparent->isRed = true;
				parent->isRed = false;
				uncle->isRed = false;
				node = grandparent;
			} else {
				if (node == parent->left) {
					rotateRight(parent);
					node = parent;
					parent = node->parent;
				}
				rotateLeft(grandparent);
				swap(parent->isRed, grandparent->isRed);
				node = parent;
			}
		}
	}
	root->isRed = false; // Ensure the root is always black
}

double redBlackTree::getMostPopularMovie() {
	// Logic to retrieve the most popular movie
	// Make sure tree is sorted by popularity before calling this function
	if (sortBy != BY_POPULARITY) {
		redBlackTree* popularityTree = new redBlackTree(BY_POPULARITY);
		// Logic to populate popularityTree with the same nodes as the current tree
		if (popularityTree->root == nullptr) {
			return 0.0; // No movies in the tree
		}
		TreeNode* current = popularityTree->root;
	}
	return root->popularity; // The most popular movie will be at the root if sorted by popularity
	
}	

long redBlackTree::getHighestRevenueMovie() {
	// Logic to retrieve the movie with the highest revenue
	// Make sure tree is sorted by revenue before calling this function
	if (sortBy != BY_REVENUE) {
		redBlackTree* revenueTree = new redBlackTree(BY_REVENUE);
		// Logic to populate revenueTree with the same nodes as the current tree
		if (revenueTree->root == nullptr) {
			return 0; // No movies in the tree
		}
		TreeNode* current = revenueTree->root;
	}
	return root->revenue; // The movie with the highest revenue will be at the root if sorted by revenue
}