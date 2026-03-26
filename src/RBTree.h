// Class definition for TreeNode, which represents a node in a Red Black tree used to store movie data.

#pragma once
#include <string>
#include <vector>

#include "Movie.h"

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
};


class redBlackTree {
public:
	enum SortType { BY_MOVIEID, BY_POPULARITY, BY_REVENUE };
	redBlackTree(SortType sortType = BY_MOVIEID);

	// Public member functions for the red-black tree creation and manipulation
	void insert(TreeNode* node);
	void rotateLeft(TreeNode* node);
	void rotateRight(TreeNode* node);
	void balanceInsert(TreeNode* node);
	double getMostPopularMovie();
	long getHighestRevenueMovie();
	void searchByMovieID(long movieID);
	vector <TreeNode*> levelOrderTraversal();
private:
	TreeNode* root;
	SortType sortBy;
};