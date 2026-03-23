#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "Movie.h"
#include <vector>

class SplayTree {
private:
    struct Node {
        Movie data;
        Node* left;
        Node* right;

        Node(const Movie& movie) : data(movie), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* rightRotate(Node* x);
    Node* leftRotate(Node* x);
    Node* splay(Node* currentRoot, int rank);
    Node* insert(Node* currentRoot, const Movie& movie);

    void destroyTree(Node* node);
    void inorder(Node* node) const;
    void collectMovies(Node* node, std::vector<Movie>& movies) const;

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