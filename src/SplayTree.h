#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "Movie.h"
#include <vector>
struct SplayNode {
    Movie data;
    SplayNode* left;
    SplayNode* right;

    SplayNode(const Movie& movie) : data(movie), left(nullptr), right(nullptr) {}
};


class SplayTree {
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

    void insert(const Movie& movie);

    Movie* search(int rank);
    Movie* searchByRank(int rank);
    Movie* searchById(int id);
    Movie* searchByRevenue(long long revenue);

    bool empty() const;
    void printInOrder() const;
    std::vector<Movie> getAllMovies() const;
};

#endif