#ifndef MOVIE_H
#define MOVIE_H

#include <string>
using namespace std;

struct Movie {
    int id;
    string title;
    string genres;
    string originalLanguage;
    string overview;
    double popularity;
    int popularityRank;
    string releaseDate;
    long long budget;
    long long revenue;
    string productionCompanies;

    Movie() {
        id = 0;
        title = "";
        genres = "";
        originalLanguage = "";
        overview = "";
        popularity = 0.0;
        popularityRank = 0;
        releaseDate = "";
        budget = 0;
        revenue = 0;
        productionCompanies = "";
    }

    Movie(int movieId, const string& movieTitle, const string& movieGenres,
          const string& language, const string& movieOverview,
          double moviePopularity, int rank, const string& date,
          long long movieBudget, long long movieRevenue,
          const string& companies) {
        id = movieId;
        title = movieTitle;
        genres = movieGenres;
        originalLanguage = language;
        overview = movieOverview;
        popularity = moviePopularity;
        popularityRank = rank;
        releaseDate = date;
        budget = movieBudget;
        revenue = movieRevenue;
        productionCompanies = companies;
    }
};

#endif