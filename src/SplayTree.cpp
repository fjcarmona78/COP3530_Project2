#include "SplayTree.h"
#include <iostream>
#include <vector>

using namespace std;

SplayTree::SplayTree() {
    root = nullptr;
}

SplayTree::~SplayTree() {
    destroyTree(root);
    root = nullptr;
}

bool SplayTree::empty() const {
    return root == nullptr;
}

void SplayTree::destroyTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    vector<Node*> stack;
    stack.push_back(node);

    while (!stack.empty()) {
        Node* current = stack.back();
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

SplayTree::Node* SplayTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

SplayTree::Node* SplayTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

SplayTree::Node* SplayTree::splay(Node* currentRoot, int rank) {
    if (currentRoot == nullptr) {
        return nullptr;
    }

    Movie tempMovie;
    Node header(tempMovie);

    Node* leftTreeMax = &header;
    Node* rightTreeMin = &header;

    while (true) {
        if (rank < currentRoot->data.popularityRank) {
            if (currentRoot->left == nullptr) {
                break;
            }

            if (rank < currentRoot->left->data.popularityRank) {
                currentRoot = rightRotate(currentRoot);
                if (currentRoot->left == nullptr) {
                    break;
                }
            }

            rightTreeMin->left = currentRoot;
            rightTreeMin = currentRoot;
            currentRoot = currentRoot->left;
        }
        else if (rank > currentRoot->data.popularityRank) {
            if (currentRoot->right == nullptr) {
                break;
            }

            if (rank > currentRoot->right->data.popularityRank) {
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

SplayTree::Node* SplayTree::insert(Node* currentRoot, const Movie& movie) {
    if (currentRoot == nullptr) {
        return new Node(movie);
    }

    currentRoot = splay(currentRoot, movie.popularityRank);

    if (movie.popularityRank < currentRoot->data.popularityRank) {
        Node* newNode = new Node(movie);
        newNode->left = currentRoot->left;
        newNode->right = currentRoot;
        currentRoot->left = nullptr;
        return newNode;
    }
    else if (movie.popularityRank > currentRoot->data.popularityRank) {
        Node* newNode = new Node(movie);
        newNode->right = currentRoot->right;
        newNode->left = currentRoot;
        currentRoot->right = nullptr;
        return newNode;
    }

    return currentRoot;
}

void SplayTree::insert(const Movie& movie) {
    root = insert(root, movie);
}

Movie* SplayTree::search(int rank) {
    return searchByRank(rank);
}

Movie* SplayTree::searchByRank(int rank) {
    if (root == nullptr) {
        return nullptr;
    }

    root = splay(root, rank);

    if (root->data.popularityRank == rank) {
        return &(root->data);
    }

    return nullptr;
}

Movie* SplayTree::searchById(int id) {
    if (root == nullptr) {
        return nullptr;
    }

    vector<Node*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        Node* current = stack.back();
        stack.pop_back();

        if (current->data.id == id) {
            return &(current->data);
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

Movie* SplayTree::searchByRevenue(long long revenue) {
    if (root == nullptr) {
        return nullptr;
    }

    vector<Node*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        Node* current = stack.back();
        stack.pop_back();

        if (current->data.revenue == revenue) {
            return &(current->data);
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

void SplayTree::inorder(Node* node) const {
    if (node == nullptr) {
        return;
    }

    inorder(node->left);
    cout << "Rank: " << node->data.popularityRank
         << " | Title: " << node->data.title
         << " | ID: " << node->data.id
         << " | Revenue: " << node->data.revenue
         << endl;
    inorder(node->right);
}

void SplayTree::printInOrder() const {
    inorder(root);
}

void SplayTree::collectMovies(Node* node, vector<Movie>& movies) const {
    if (node == nullptr) {
        return;
    }

    collectMovies(node->left, movies);
    movies.push_back(node->data);
    collectMovies(node->right, movies);
}

vector<Movie> SplayTree::getAllMovies() const {
    vector<Movie> movies;
    collectMovies(root, movies);
    return movies;
}