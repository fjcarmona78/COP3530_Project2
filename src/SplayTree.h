#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "Movie.h"
#include "BSTree.h"

#include <vector>
#include <queue>

struct SplayNode {
    Movie data;
    SplayNode* left;
    SplayNode* right;

    SplayNode(const Movie& movie) : data(movie), left(nullptr), right(nullptr) {}
};


class SplayTree : public BSTMovie {
private:
    SplayNode* root;

    SplayNode* rightRotate(SplayNode* x);
    SplayNode* leftRotate(SplayNode* x);
    SplayNode* splay(SplayNode* currentRoot, int rank);
    SplayNode* insert(SplayNode* currentRoot, const Movie& movie);

    void destroyTree(SplayNode* SplayNode);
    void inorder(SplayNode* SplayNode) const;
    void collectMovies(SplayNode* SplayNode, std::vector<Movie>& movies) const;

public:
    SplayTree();
    ~SplayTree();

    bool insert(const Movie& movie) override;

    Movie* search(int rank);
    Movie* searchByRank(int rank);
    Movie* searchByMovieID(int64_t movieID) override;
    Movie* searchByRevenue(long long revenue);

    bool isEmpty() override {
        return !root;
    };
    void printInOrder() const;
    std::vector<Movie> getAllMovies() const;
    std::vector<Movie> levelOrderTraversal() override;
};

#endif