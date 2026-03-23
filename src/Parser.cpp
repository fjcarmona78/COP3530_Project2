#include "Parser.h"
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

vector<string> Parser::splitCSVLine(const string& line) {
    vector<string> result;
    string current = "";
    bool insideQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            insideQuotes = !insideQuotes;
        }
        else if (ch == ',' && !insideQuotes) {
            result.push_back(current);
            current = "";
        }
        else {
            current += ch;
        }
    }

    result.push_back(current);
    return result;
}

int Parser::findColumnIndex(const vector<string>& headers, const string& name) {
    for (int i = 0; i < (int)headers.size(); i++) {
        if (headers[i] == name) {
            return i;
        }
    }
    return -1;
}

int Parser::toInt(const string& text) {
    if (text.empty() || text == "[null]") {
        return 0;
    }

    try {
        return stoi(text);
    }
    catch (...) {
        return 0;
    }
}

long long Parser::toLongLong(const string& text) {
    if (text.empty() || text == "[null]") {
        return 0;
    }

    try {
        double value = stod(text);
        return (long long)value;
    }
    catch (...) {
        return 0;
    }
}

double Parser::toDouble(const string& text) {
    if (text.empty() || text == "[null]") {
        return 0.0;
    }

    try {
        return stod(text);
    }
    catch (...) {
        return 0.0;
    }
}

vector<Movie> Parser::loadMovies(const string& filename) {
    vector<Movie> movies;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return movies;
    }

    string line;

    if (!getline(file, line)) {
        cout << "File is empty." << endl;
        return movies;
    }

    vector<string> headers = splitCSVLine(line);

    int idIndex = findColumnIndex(headers, "id");
    int titleIndex = findColumnIndex(headers, "title");
    int genresIndex = findColumnIndex(headers, "genres");
    int languageIndex = findColumnIndex(headers, "original_language");
    int overviewIndex = findColumnIndex(headers, "overview");
    int popularityIndex = findColumnIndex(headers, "popularity");
    int releaseDateIndex = findColumnIndex(headers, "release_date");
    int budgetIndex = findColumnIndex(headers, "budget");
    int revenueIndex = findColumnIndex(headers, "revenue");
    int companiesIndex = findColumnIndex(headers, "production_companies");

    while (getline(file, line)) {
        vector<string> row = splitCSVLine(line);

        if (row.size() < headers.size()) {
            continue;
        }

        int id = (idIndex >= 0) ? toInt(row[idIndex]) : 0;
        string title = (titleIndex >= 0) ? row[titleIndex] : "";
        string genres = (genresIndex >= 0) ? row[genresIndex] : "";
        string language = (languageIndex >= 0) ? row[languageIndex] : "";
        string overview = (overviewIndex >= 0) ? row[overviewIndex] : "";
        double popularity = (popularityIndex >= 0) ? toDouble(row[popularityIndex]) : 0.0;
        string releaseDate = (releaseDateIndex >= 0) ? row[releaseDateIndex] : "";
        long long budget = (budgetIndex >= 0) ? toLongLong(row[budgetIndex]) : 0;
        long long revenue = (revenueIndex >= 0) ? toLongLong(row[revenueIndex]) : 0;
        string companies = (companiesIndex >= 0) ? row[companiesIndex] : "";

        Movie movie(id, title, genres, language, overview,
                    popularity, 0, releaseDate, budget, revenue, companies);

        movies.push_back(movie);
    }

    file.close();
    return movies;
}

void Parser::assignPopularityRanks(vector<Movie>& movies) {
    sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        return a.popularity > b.popularity;
    });

    for (int i = 0; i < (int)movies.size(); i++) {
        movies[i].popularityRank = i + 1;
    }
}

void Parser::insertBalanced(vector<Movie>& movies, int left, int right, SplayTree& tree) {
    if (left > right) {
        return;
    }

    int mid = left + (right - left) / 2;

    tree.insert(movies[mid]);

    insertBalanced(movies, left, mid - 1, tree);
    insertBalanced(movies, mid + 1, right, tree);
}

void Parser::loadIntoSplayTree(const string& filename, SplayTree& tree) {
    vector<Movie> movies = loadMovies(filename);
    assignPopularityRanks(movies);

    insertBalanced(movies, 0, (int)movies.size() - 1, tree);

    cout << "Loaded " << movies.size() << " movies into Splay Tree." << endl;
}