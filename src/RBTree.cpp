#include "RBTree.h"
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

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
			} else if (sortBy == BY_RANK) {
				if (movie.popularityRank < current->movieData.popularityRank) {
					current = current->left;
				} else  {
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
		} else if (sortBy == BY_RANK) {
			if (movie.popularityRank < parent->movieData.popularityRank) {
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
	while (node != root  && node->isRed && node->parent->isRed) {
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

Movie* redBlackTree::getMostPopularMovie() {
	if (root == nullptr) {
		std::cout << "null root" << std::endl;
		return nullptr;
	}

	vector<TreeNode*> stack;
	stack.push_back(root);

	Movie* mostPop = &(root->movieData);

	while (!stack.empty()) {
		TreeNode* current = stack.back();
		stack.pop_back();

		if (current->movieData.popularity > mostPop->popularity) {
			mostPop = &(current->movieData);
		}

		if (current->right != nullptr) {
			stack.push_back(current->right);
		}
		if (current->left != nullptr) {
			stack.push_back(current->left);
		}
	}

	return mostPop;
}

Movie* redBlackTree::getHighestRevenueMovie() {
	if (root == nullptr) {
		return 0;
	}

	vector<TreeNode*> stack;
	stack.push_back(root);

	Movie* bestRevenue = &(root->movieData);

	while (!stack.empty()) {
		TreeNode* current = stack.back();
		stack.pop_back();

		if (current->movieData.revenue > bestRevenue->revenue) {
			bestRevenue = &(current->movieData);
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

Movie* redBlackTree::searchByMovieID(int32_t movieID) {
	// Logic to search for a movie by its ID
	TreeNode* current = root;
	while (current != nullptr) {
		if (current->movieData.movieID == movieID) {
			return &(current->movieData); // Exit after finding the movie
		} else if (movieID < current->movieData.movieID) {
			current = current->left; // Move left
		} else {
			current = current->right; // Move right
		}
	}
	return nullptr;
}

Movie* redBlackTree::searchRankHelper(TreeNode* node, int32_t rank) {
	if (node == nullptr) {
		return nullptr;
	}

	vector<TreeNode*> stack;
	stack.push_back(node);

	while (!stack.empty()) {
		TreeNode* current = stack.back();
		stack.pop_back();

		if (current->movieData.popularityRank == rank) {
			return &(current->movieData);
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

Movie* redBlackTree::searchByRank(int32_t rank) {
	if (sortBy == BY_RANK) {
		TreeNode* current = root;
		while (current != nullptr) {
			if (current->movieData.popularityRank == rank) {
				return &(current->movieData); // Exit after finding the movie
			} else if (rank < current->movieData.popularityRank) {
				current = current->left; // Move left
			} else {
				current = current->right; // Move right
			}
		}
		return &(current->movieData);
	}
	else {
		return searchRankHelper(root, rank);
	}
}

vector <Movie*> redBlackTree::levelOrderTraversal() {
	// Logic to perform level order traversal of the tree
	// Add pointers to the first 1000 nodes in the order they are visited to the result vector
	vector<Movie *> result;
	if (root == nullptr) {
		return result; // Return empty vector if tree is empty
	}
	queue<TreeNode*> q;
	int count = 0; // Counter to keep track of the number of nodes added to the result vector
	q.push(root);
	while (!q.empty() && count < 300) {
		TreeNode* current = q.front();
		q.pop();
		result.push_back(&(current->movieData)); // Add the current node to the result vector
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