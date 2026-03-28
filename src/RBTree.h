// Class definition for TreeNode, which represents a node in a Red Black tree used to store movie data.

#pragma once
#include <string>
#include <vector>

#include "Movie.h"
#include "BSTree.h"

using namespace std;

// TreeNode class definition
struct TreeNode {
	Movie movieData = Movie();
	bool isRed;             // Color of the node (true for red, false for black)
	TreeNode* left;         // Pointer to the left child node
	TreeNode* right;        // Pointer to the right child node
	TreeNode* parent;       // Pointer to the parent node
	// Constructor to initialize a TreeNode with default values
	TreeNode() : isRed(true), left(nullptr), right(nullptr) {}
	TreeNode(Movie m) : movieData(m), isRed(true), left(nullptr), right(nullptr) {}
};


class redBlackTree : public BSTMovie {
public:
	redBlackTree(SortType sortType = BY_MOVIEID);

	// Public member functions for the red-black tree creation and manipulation
	bool insert(const Movie& movie) override;
	Movie* getMostPopularMovie() override;
	Movie* getHighestRevenueMovie() override;
	Movie* searchByMovieID(int64_t movieID) override;
	std::vector<Movie *> levelOrderTraversal() override;
	Movie* searchByRank(int rank) override;
	bool isEmpty() override {
		return !root;
	}
private:
	TreeNode* root;
	SortType sortBy;
	Movie* searchRankHelper(TreeNode *node, int rank);
	void rotateLeft(TreeNode* node);
	void rotateRight(TreeNode* node);
	void balanceInsert(TreeNode* node);
};