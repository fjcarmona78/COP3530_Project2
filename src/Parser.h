#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "RBTree.h"
#include "Movie.h"

class Parser {
private:
    std::vector<std::string> splitCSVLine(const std::string& line);
    int findColumnIndex(const std::vector<std::string>& headers, const std::string& name);
    int toInt(const std::string& text);
    long toLong(const std::string& text);
    double toDouble(const std::string& text);
    std::string cleanText(const std::string& text);

public:
    std::vector<Movie> loadMovies(const std::string& filename);
    void assignPopularityRanks(std::vector<Movie>& movies);
};

#endif