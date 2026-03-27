#include "TreeNode.h"
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

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
		node->parent = parent; // Set the parent pointer of the new node
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
	if (root == nullptr) {
		return 0.0;
	}
	return root->popularity;
}

long redBlackTree::getHighestRevenueMovie() {
	if (root == nullptr) {
		return 0;
	}
	return root->revenue;
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
			return; // Exit after finding the movie
		} else if (movieID < current->movieID) {
			current = current->left; // Move left
		} else {
			current = current->right; // Move right
		}
	}
	cout << "Movie with ID " << movieID << " not found." << endl; // Movie not found
}

TreeNode* redBlackTree::searchRankHelper(TreeNode* node, int rank) {
	if (node == nullptr) {
		return nullptr;
	}

	vector<TreeNode*> stack;
	stack.push_back(node);

	while (!stack.empty()) {
		TreeNode* current = stack.back();
		stack.pop_back();

		if (current->popularityRank == rank) {
			return current;
		}

		if (current->right != nullptr) {
			stack.push_back(current->right);
		}

		if (current->left != nullptr) {
			stack.push_back(current->left);
		}
	}

	return nullptr;
}

TreeNode* redBlackTree::searchByRank(int rank) {
	return searchRankHelper(root, rank);
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

SplayTree::SplayNode::SplayNode(const TreeNode& movie) {
	data = movie;
	left = nullptr;
	right = nullptr;
}

SplayTree::SplayTree(SortType sortType) {
	root = nullptr;
	sortBy = sortType;
}

SplayTree::~SplayTree() {
	destroyTree(root);
	root = nullptr;
}

void SplayTree::destroyTree(SplayNode* node) {
	if (node == nullptr) {
		return;
	}

	vector<SplayNode*> stack;
	stack.push_back(node);

	while (!stack.empty()) {
		SplayNode* current = stack.back();
		stack.pop_back();

		if (current->left != nullptr) {
			stack.push_back(current->left);
		}
		if (current->right != nullptr) {
			stack.push_back(current->right);
		}

		delete current;
	}
}

SplayTree::SplayNode* SplayTree::rightRotate(SplayNode* x) {
	SplayNode* y = x->left;
	x->left = y->right;
	y->right = x;
	return y;
}

SplayTree::SplayNode* SplayTree::leftRotate(SplayNode* x) {
	SplayNode* y = x->right;
	x->right = y->left;
	y->left = x;
	return y;
}

SplayTree::SplayNode* SplayTree::splay(SplayNode* currentRoot, long key) {
	if (currentRoot == nullptr) {
		return nullptr;
	}

	TreeNode tempMovie;
	SplayNode header(tempMovie);

	SplayNode* leftTreeMax = &header;
	SplayNode* rightTreeMin = &header;

	while (true) {
		long currentKey = getKey(currentRoot->data);

		if (key < currentKey) {
			if (currentRoot->left == nullptr) {
				break;
			}

			if (key < getKey(currentRoot->left->data)) {
				currentRoot = rightRotate(currentRoot);
				if (currentRoot->left == nullptr) {
					break;
				}
			}

			rightTreeMin->left = currentRoot;
			rightTreeMin = currentRoot;
			currentRoot = currentRoot->left;
		}
		else if (key > currentKey) {
			if (currentRoot->right == nullptr) {
				break;
			}

			if (key > getKey(currentRoot->right->data)) {
				currentRoot = leftRotate(currentRoot);
				if (currentRoot->right == nullptr) {
					break;
				}
			}

			leftTreeMax->right = currentRoot;
			leftTreeMax = currentRoot;
			currentRoot = currentRoot->right;
		}
		else {
			break;
		}
	}

	leftTreeMax->right = currentRoot->left;
	rightTreeMin->left = currentRoot->right;
	currentRoot->left = header.right;
	currentRoot->right = header.left;

	return currentRoot;
}

SplayTree::SplayNode* SplayTree::insert(SplayNode* currentRoot, const TreeNode& movie) {
	if (currentRoot == nullptr) {
		return new SplayNode(movie);
	}

	long movieKey = getKey(movie);
	currentRoot = splay(currentRoot, movieKey);

	long currentKey = getKey(currentRoot->data);

	if (movieKey < currentKey) {
		SplayNode* newNode = new SplayNode(movie);
		newNode->left = currentRoot->left;
		newNode->right = currentRoot;
		currentRoot->left = nullptr;
		return newNode;
	}
	else if (movieKey > currentKey) {
		SplayNode* newNode = new SplayNode(movie);
		newNode->right = currentRoot->right;
		newNode->left = currentRoot;
		currentRoot->right = nullptr;
		return newNode;
	}

	return currentRoot;
}

void SplayTree::insert(TreeNode* node) {
	if (node == nullptr) {
		return;
	}
	root = insert(root, *node);
}

SplayTree::SplayNode* SplayTree::searchRankHelper(SplayNode* node, int rank) {
	if (node == nullptr) {
		return nullptr;
	}

	// If this splay tree is organized by rank, use real splay search
	if (sortBy == BY_RANK) {
		root = splay(root, rank);

		if (root != nullptr && root->data.popularityRank == rank) {
			return root;
		}

		return nullptr;
	}

	// Otherwise, do traversal search
	vector<SplayNode*> stack;
	stack.push_back(node);

	while (!stack.empty()) {
		SplayNode* current = stack.back();
		stack.pop_back();

		if (current->data.popularityRank == rank) {
			return current;
		}

		if (current->right != nullptr) {
			stack.push_back(current->right);
		}
		if (current->left != nullptr) {
			stack.push_back(current->left);
		}
	}

	return nullptr;
}

TreeNode* SplayTree::searchByRank(int rank) {
	SplayNode* result = searchRankHelper(root, rank);
	if (result == nullptr) {
		return nullptr;
	}
	return &(result->data);
}

double SplayTree::getMostPopularMovie() {
	TreeNode* movie = searchByRank(1);
	if (movie == nullptr) {
		return 0.0;
	}
	return movie->popularity;
}

SplayTree::SplayNode* SplayTree::searchMovieIDHelper(SplayNode* node, long movieID) {
	if (node == nullptr) {
		return nullptr;
	}

	// If this splay tree is organized by movie ID, use real splay search
	if (sortBy == BY_MOVIEID) {
		root = splay(root, movieID);

		if (root != nullptr && root->data.movieID == movieID) {
			return root;
		}

		return nullptr;
	}

	// Otherwise, do traversal search
	vector<SplayNode*> stack;
	stack.push_back(node);

	while (!stack.empty()) {
		SplayNode* current = stack.back();
		stack.pop_back();

		if (current->data.movieID == movieID) {
			return current;
		}

		if (current->right != nullptr) {
			stack.push_back(current->right);
		}
		if (current->left != nullptr) {
			stack.push_back(current->left);
		}
	}

	return nullptr;
}

void SplayTree::searchByMovieID(long movieID) {
	SplayNode* result = searchMovieIDHelper(root, movieID);

	if (result == nullptr) {
		cout << "Movie with ID " << movieID << " not found." << endl;
		return;
	}

	TreeNode* current = &(result->data);

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
}

long SplayTree::getHighestRevenueMovie() {
	if (root == nullptr) {
		return 0;
	}

	vector<SplayNode*> stack;
	stack.push_back(root);

	long bestRevenue = root->data.revenue;

	while (!stack.empty()) {
		SplayNode* current = stack.back();
		stack.pop_back();

		if (current->data.revenue > bestRevenue) {
			bestRevenue = current->data.revenue;
		}

		if (current->right != nullptr) {
			stack.push_back(current->right);
		}
		if (current->left != nullptr) {
			stack.push_back(current->left);
		}
	}

	return bestRevenue;
}

long SplayTree::getKey(const TreeNode& movie) {
	if (sortBy == BY_MOVIEID) {
		return movie.movieID;
	}
	return movie.popularityRank;
}

vector<TreeNode*> SplayTree::levelOrderTraversal() {
	vector<TreeNode*> result;

	if (root == nullptr) {
		return result;
	}

	queue<SplayNode*> q;
	q.push(root);

	while (!q.empty()) {
		SplayNode* current = q.front();
		q.pop();

		result.push_back(&(current->data));

		if (current->left != nullptr) {
			q.push(current->left);
		}
		if (current->right != nullptr) {
			q.push(current->right);
		}
	}

	return result;
}