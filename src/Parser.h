#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "Movie.h"
#include "SplayTree.h"

class Parser {
private:
    std::vector<std::string> splitCSVLine(const std::string& line);
    int findColumnIndex(const std::vector<std::string>& headers, const std::string& name);
    int toInt(const std::string& text);
    long long toLongLong(const std::string& text);
    double toDouble(const std::string& text);

    void insertBalanced(std::vector<Movie>& movies, int left, int right, SplayTree& tree);

public:
    std::vector<Movie> loadMovies(const std::string& filename);
    void assignPopularityRanks(std::vector<Movie>& movies);
    void loadIntoSplayTree(const std::string& filename, SplayTree& tree);
};

#endif