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
			if (sortBy == BY_MOVIEID) {
				if (node->movieID < current->movieID) {
					current = current->left;
				} else {
					current = current->right;
				}
			} else if (sortBy == BY_POPULARITY) {
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
		if (sortBy == BY_MOVIEID) {
			if (node->movieID < parent->movieID) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		} else if (sortBy == BY_POPULARITY) {
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

void redBlackTree::searchByMovieID(long movieID) {
	// Logic to search for a movie by its ID
	TreeNode* current = root;
	while (current != nullptr) {
		if (current->movieID == movieID) {
			// Movie found, print its details
			cout << "Movie ID: " << current->movieID << endl;
			cout << "Title: " << current->title << endl;
			cout << "Genre: " << current->genre << endl;
			cout << "Original Language: " << current->originalLanguage << endl;
			cout << "Overview: " << current->overview << endl;
			cout << "Popularity: " << current->popularity << endl;
			cout << "Production Companies: " << current->productionCompanies << endl;
			cout << "Release Date: " << current->releaseDate << endl;
			cout << "Budget: " << current->budget << endl;
			cout << "Revenue: " << current->revenue << endl;
			cout << "Runtime: " << current->runtime << endl;
			cout << "Status: " << current->status << endl;
			cout << "Tagline: " << current->tagline << endl;
			cout << "Voter Average: " << current->voterAverage << endl;
			cout << "Voter Count: " << current->voterCount << endl;
			cout << "Credits: " << current->credits << endl;
			cout << "Keywords: " << current->keywords << endl;
			cout << "Poster Path: " << current->posterPath << endl;
			cout << "Backdrop Path: " << current->backdropPath << endl;
			cout << "Recommendations: " << current->recommendations << endl;
			return; // Exit after finding the movie
		} else if (movieID < current->movieID) {
			current = current->left; // Move left
		} else {
			current = current->right; // Move right
		}
	}
	cout << "Movie with ID " << movieID << " not found." << endl; // Movie not found
}

vector <TreeNode*> redBlackTree::levelOrderTraversal() {
	// Logic to perform level order traversal of the tree
	// Add pointers to the first 1000 nodes in the order they are visited to the result vector
	vector<TreeNode*> result;
	if (root == nullptr) {
		return result; // Return empty vector if tree is empty
	}
	queue<TreeNode*> q;
	int count = 0; // Counter to keep track of the number of nodes added to the result vector
	q.push(root);
	while (!q.empty() && count < 1000) {
		TreeNode* current = q.front();
		q.pop();
		result.push_back(current); // Add the current node to the result vector
		count++;
		if (current->left != nullptr) {
			q.push(current->left); // Add left child to the queue
		}
		if (current->right != nullptr) {
			q.push(current->right); // Add right child to the queue
		}
	}
	return result; // Return the vector containing pointers to the first 1000 nodes in level order
}