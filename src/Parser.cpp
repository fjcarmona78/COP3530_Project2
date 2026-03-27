#include "Parser.h"
#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm>

using namespace std;

vector<string> Parser::splitCSVLine(const string& line) {
    vector<string> result;
    string current = "";
    bool insideQuotes = false;

    for (size_t i = 0; i < line.size(); i++) {
        char ch = line[i];

        if (ch == '"') {
            if (insideQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                current += '"';
                i++;
            }
            else {
                insideQuotes = !insideQuotes;
            }
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

string Parser::cleanText(const string& text) {
    string cleaned = text;

    while (!cleaned.empty() && isspace((unsigned char)cleaned.front())) {
        cleaned.erase(cleaned.begin());
    }

    while (!cleaned.empty() && isspace((unsigned char)cleaned.back())) {
        cleaned.pop_back();
    }

    if (cleaned == "[null]" || cleaned == "null") {
        return "";
    }

    return cleaned;
}

int Parser::toInt(const string& text) {
    string cleaned = cleanText(text);

    if (cleaned.empty()) {
        return 0;
    }

    try {
        return stoi(cleaned);
    }
    catch (...) {
        return 0;
    }
}

long Parser::toLong(const string& text) {
    string cleaned = cleanText(text);

    if (cleaned.empty()) {
        return 0;
    }

    try {
        double value = stod(cleaned);
        return (long)value;
    }
    catch (...) {
        return 0;
    }
}

double Parser::toDouble(const string& text) {
    string cleaned = cleanText(text);

    if (cleaned.empty()) {
        return 0.0;
    }

    try {
        return stod(cleaned);
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
    int companiesIndex = findColumnIndex(headers, "production_companies");
    int releaseDateIndex = findColumnIndex(headers, "release_date");
    int budgetIndex = findColumnIndex(headers, "budget");
    int revenueIndex = findColumnIndex(headers, "revenue");
    int runtimeIndex = findColumnIndex(headers, "runtime");
    int statusIndex = findColumnIndex(headers, "status");
    int taglineIndex = findColumnIndex(headers, "tagline");
    int voteAverageIndex = findColumnIndex(headers, "vote_average");
    int voteCountIndex = findColumnIndex(headers, "vote_count");
    int creditsIndex = findColumnIndex(headers, "credits");
    int keywordsIndex = findColumnIndex(headers, "keywords");
    int posterPathIndex = findColumnIndex(headers, "poster_path");
    int backdropPathIndex = findColumnIndex(headers, "backdrop_path");
    int recommendationsIndex = findColumnIndex(headers, "recommendations");

    while (getline(file, line)) {
        vector<string> row = splitCSVLine(line);

        if (row.size() < headers.size()) {
            continue;
        }

        Movie movie;

        movie.movieID = (idIndex >= 0) ? toInt(row[idIndex]) : 0;
        movie.title = (titleIndex >= 0) ? cleanText(row[titleIndex]) : "";
        movie.genre = (genresIndex >= 0) ? cleanText(row[genresIndex]) : "";
        movie.originalLanguage = (languageIndex >= 0) ? cleanText(row[languageIndex]) : "";
        movie.overview = (overviewIndex >= 0) ? cleanText(row[overviewIndex]) : "";
        movie.popularity = (popularityIndex >= 0) ? toDouble(row[popularityIndex]) : 0.0;
        movie.productionCompanies = (companiesIndex >= 0) ? cleanText(row[companiesIndex]) : "";
        movie.releaseDate = (releaseDateIndex >= 0) ? cleanText(row[releaseDateIndex]) : "";
        movie.budget = (budgetIndex >= 0) ? toLong(row[budgetIndex]) : 0;
        movie.revenue = (revenueIndex >= 0) ? toLong(row[revenueIndex]) : 0;
        movie.runtime = (runtimeIndex >= 0) ? toInt(row[runtimeIndex]) : 0;
        movie.status = (statusIndex >= 0) ? cleanText(row[statusIndex]) : "";
        movie.tagline = (taglineIndex >= 0) ? cleanText(row[taglineIndex]) : "";
        movie.voterAverage = (voteAverageIndex >= 0) ? toDouble(row[voteAverageIndex]) : 0.0;
        movie.voterCount = (voteCountIndex >= 0) ? toInt(row[voteCountIndex]) : 0;
        movie.credits = (creditsIndex >= 0) ? cleanText(row[creditsIndex]) : "";
        movie.keywords = (keywordsIndex >= 0) ? cleanText(row[keywordsIndex]) : "";
        movie.posterPath = (posterPathIndex >= 0) ? cleanText(row[posterPathIndex]) : "";
        movie.backdropPath = (backdropPathIndex >= 0) ? cleanText(row[backdropPathIndex]) : "";
        movie.recommendations = (recommendationsIndex >= 0) ? cleanText(row[recommendationsIndex]) : "";
        movie.popularityRank = 0;

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