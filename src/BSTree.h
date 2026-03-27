#ifndef BST_H
#define BST_H

#include "Movie.h"
#include <vector>

// BST interface
class BSTMovie {
    public:
        virtual bool insert(const Movie& movie) = 0;
        virtual bool isEmpty() = 0;

        virtual std::vector<Movie *> levelOrderTraversal() = 0;

        virtual double getMostPopularMovie() = 0;
        virtual long getHighestRevenueMovie() = 0;

        virtual Movie* searchByRank(int rank) = 0;
        virtual Movie* searchByMovieID(int64_t movieID) = 0;

};

#endif // BST_H