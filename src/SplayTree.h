#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "Movie.h"
#include "BSTree.h"

#include <vector>
#include <queue>

struct SplayNode {
    Movie movieData;
    SplayNode* left = nullptr;
    SplayNode* right = nullptr;

    SplayNode(const Movie& movie) : movieData(movie), left(nullptr), right(nullptr) {}
};

class SplayTree : public BSTMovie {
private:
	SplayNode* root;
	SortType sortBy;

	SplayNode* rightRotate(SplayNode* x);
	SplayNode* leftRotate(SplayNode* x);
	SplayNode* splay(SplayNode* currentRoot, int32_t key);
	SplayNode* insert(SplayNode* currentRoot, const Movie& movie);
	SplayNode* searchMovieIDHelper(SplayNode* node, long movieID);

	void destroyTree(SplayNode* node);
	SplayNode* searchRankHelper(SplayNode* node, int rank);
	long getKey(const Movie& movie);

public:
	SplayTree(SortType sortType = BY_MOVIEID);
	~SplayTree();

	bool insert(const Movie& node) override;

	Movie* searchByRank(int rank) override;
	vector<Movie*> levelOrderTraversal() override;
	Movie* getMostPopularMovie() override;
	Movie* getHighestRevenueMovie() override;
	Movie* searchByMovieID(int32_t movieID) override;
	bool isEmpty() override {
		return !root;
	}
};

#endif