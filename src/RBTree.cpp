#include "RBTree.h"
#include <iostream>
#include <queue>
#include <utility>

using namespace std;

redBlackTree::redBlackTree(SortType sortType) : root(nullptr), sortBy(sortType) {}

bool redBlackTree::insert(const Movie& movie) {
	// Insertion logic for the red-black tree
	if (root == nullptr) {
		root = new TreeNode(movie);
		root->isRed = false; // The root node must be black
		return true;
	} else {
		// Insert the node into the tree based on the sorting criteria
		TreeNode* parent = nullptr;
		TreeNode* current = root;
		while (current != nullptr) {
			parent = current;
			if (sortBy == BY_MOVIEID) {
				if (movie.movieID < current->movieData.movieID) {
					current = current->left;
				} else {
					current = current->right;
				}
			} else if (sortBy == BY_POPULARITY) {
				if (movie.popularity < current->movieData.popularity) {
					current = current->left;
				} else  {
					current = current->right;
				}
			} else { // BY_REVENUE
				if (movie.revenue < current->movieData.revenue) {
					current = current->left;
				} else {
					current = current->right;
				}
			}
		}

		TreeNode *node = new TreeNode(movie);
		if (sortBy == BY_MOVIEID) {
			if (movie.movieID < parent->movieData.movieID) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		} else if (sortBy == BY_POPULARITY) {
			if (movie.popularity < parent->movieData.popularity) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		} else { // BY_REVENUE
			if (movie.revenue < parent->movieData.revenue) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		}
		node->parent = parent; // Set the parent pointer of the new node
		balanceInsert(node); // Balance the tree after insertion
		return true;
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
		//TreeNode* current = popularityTree->root;
	}
	return root->movieData.popularity; // The most popular movie will be at the root if sorted by popularity
	
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
		//TreeNode* current = revenueTree->root;
	}
	return root->movieData.revenue; // The movie with the highest revenue will be at the root if sorted by revenue
}

Movie* redBlackTree::searchByMovieID(int64_t movieID) {
	// Logic to search for a movie by its ID
	TreeNode* current = root;
	while (current != nullptr) {
		if (current->movieData.movieID == movieID) {
			// Movie found, print its details
			cout << "Movie ID: " << current->movieData.movieID << endl;
			cout << "Title: " << current->movieData.title << endl;
			cout << "Genre: " << current->movieData.genre << endl;
			cout << "Original Language: " << current->movieData.originalLanguage << endl;
			cout << "Overview: " << current->movieData.overview << endl;
			cout << "Popularity: " << current->movieData.popularity << endl;
			cout << "Production Companies: " << current->movieData.productionCompanies << endl;
			cout << "Release Date: " << current->movieData.releaseDate << endl;
			cout << "Budget: " << current->movieData.budget << endl;
			cout << "Revenue: " << current->movieData.revenue << endl;
			cout << "Runtime: " << current->movieData.runtime << endl;
			cout << "Status: " << current->movieData.status << endl;
			cout << "Tagline: " << current->movieData.tagline << endl;
			cout << "Voter Average: " << current->movieData.voterAverage << endl;
			cout << "Voter Count: " << current->movieData.voterCount << endl;
			cout << "Credits: " << current->movieData.credits << endl;
			cout << "Keywords: " << current->movieData.keywords << endl;
			cout << "Poster Path: " << current->movieData.posterPath << endl;
			cout << "Backdrop Path: " << current->movieData.backdropPath << endl;
			cout << "Recommendations: " << current->movieData.recommendations << endl;
			return &(current->movieData); // Exit after finding the movie
		} else if (movieID < current->movieData.movieID) {
			current = current->left; // Move left
		} else {
			current = current->right; // Move right
		}
	}
	cout << "Movie with ID " << movieID << " not found." << endl; // Movie not found
	return nullptr;
}

vector <Movie> redBlackTree::levelOrderTraversal() {
	// Logic to perform level order traversal of the tree
	// Add pointers to the first 1000 nodes in the order they are visited to the result vector
	vector<Movie> result;
	if (root == nullptr) {
		return result; // Return empty vector if tree is empty
	}
	queue<TreeNode*> q;
	int count = 0; // Counter to keep track of the number of nodes added to the result vector
	q.push(root);
	while (!q.empty() || count < 1000) {
		TreeNode* current = q.front();
		q.pop();
		result.push_back(current->movieData); // Add the current node to the result vector
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