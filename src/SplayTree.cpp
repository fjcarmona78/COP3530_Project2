#include "SplayTree.h"

#include <iostream>
#include <vector>

using namespace std;

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

SplayNode* SplayTree::rightRotate(SplayNode* x) {
	SplayNode* y = x->left;
	x->left = y->right;
	y->right = x;
	return y;
}

SplayNode* SplayTree::leftRotate(SplayNode* x) {
	SplayNode* y = x->right;
	x->right = y->left;
	y->left = x;
	return y;
}

SplayNode* SplayTree::splay(SplayNode* currentRoot, long key) {
	if (currentRoot == nullptr) {
		return nullptr;
	}

	SplayNode header = SplayNode(Movie());

	SplayNode* leftTreeMax = &header;
	SplayNode* rightTreeMin = &header;

	while (true) {
		long currentKey = getKey(currentRoot->movieData);

		if (key < currentKey) {
			if (currentRoot->left == nullptr) {
				break;
			}

			if (key < getKey(currentRoot->left->movieData)) {
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

			if (key > getKey(currentRoot->right->movieData)) {
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

SplayNode* SplayTree::insert(SplayNode* currentRoot, const Movie& movie) {
	if (currentRoot == nullptr) {
		return new SplayNode(movie);
	}

	long movieKey = getKey(movie);
	currentRoot = splay(currentRoot, movieKey);

	long currentKey = getKey(currentRoot->movieData);

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

bool SplayTree::insert(const Movie& movie) {
	root = insert(root, movie);
    return root;
}

SplayNode* SplayTree::searchRankHelper(SplayNode* node, int rank) {
	if (node == nullptr) {
		return nullptr;
	}

	// If this splay tree is organized by rank, use real splay search
	if (sortBy == BY_RANK) {
		root = splay(root, rank);

		if (root != nullptr && root->movieData.popularityRank == rank) {
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

		if (current->movieData.popularityRank == rank) {
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

Movie* SplayTree::searchByRank(int rank) {
	SplayNode* result = searchRankHelper(root, rank);
	if (result == nullptr) {
		return nullptr;
	}
	return &(result->movieData);
}

Movie* SplayTree::getMostPopularMovie() {
	Movie* movie = searchByRank(1);
	if (movie == nullptr) {
		return nullptr;
	}
	return movie;
}

SplayNode* SplayTree::searchMovieIDHelper(SplayNode* node, long movieID) {
	if (node == nullptr) {
		return nullptr;
	}

	// If this splay tree is organized by movie ID, use real splay search
	if (sortBy == BY_MOVIEID) {
		root = splay(root, movieID);

		if (root != nullptr && root->movieData.movieID == movieID) {
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

		if (current->movieData.movieID == movieID) {
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

Movie* SplayTree::searchByMovieID(int64_t movieID) {
	SplayNode* result = searchMovieIDHelper(root, movieID);

	if (result == nullptr) {
		cout << "Movie with ID " << movieID << " not found." << endl;
		return nullptr;
	}

    return &(result->movieData);
}

Movie* SplayTree::getHighestRevenueMovie() {
	if (root == nullptr) {
		return 0;
	}

	vector<SplayNode*> stack;
	stack.push_back(root);

	Movie* bestRevenue = &(root->movieData);

	while (!stack.empty()) {
		SplayNode* current = stack.back();
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

long SplayTree::getKey(const Movie& movie) {
	if (sortBy == BY_MOVIEID) {
		return movie.movieID;
	}
	return movie.popularityRank;
}

vector<Movie*> SplayTree::levelOrderTraversal() {
	vector<Movie*> result;

	if (root == nullptr) {
		return result;
	}

	queue<SplayNode*> q;
	q.push(root);

	int count = 0;
	while (!q.empty() || count < 300) {
		SplayNode* current = q.front();
		q.pop();

		result.push_back(&(current->movieData));

		if (current->left != nullptr) {
			q.push(current->left);
		}
		if (current->right != nullptr) {
			q.push(current->right);
		}
		count++;
	}

	return result;
}