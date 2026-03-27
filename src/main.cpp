#include <iostream>
#include <vector>
#include <chrono>
#include "TreeNode.h"
#include "Parser.h"

using namespace std;
using namespace chrono;

/*
 Temporary console main for testing until the WindowManager / GUI is finished.

 This version keeps:
 1. Search by popularity rank
 2. Search by movie ID
 3. Benchmark rank searches
 4. Benchmark movie ID searches
 0. Exit
*/

void printMovieBasic(const TreeNode* movie) {
    if (movie == nullptr) {
        cout << "Movie not found." << endl;
        return;
    }

    cout << "Title: " << movie->title << endl;
    cout << "Genre: " << movie->genre << endl;
    cout << "Original Language: " << movie->originalLanguage << endl;
    cout << "Popularity: " << movie->popularity << endl;
    cout << "Popularity Rank: " << movie->popularityRank << endl;
    cout << "Release Date: " << movie->releaseDate << endl;
    cout << "Budget: " << movie->budget << endl;
    cout << "Revenue: " << movie->revenue << endl;
    cout << "Runtime: " << movie->runtime << endl;
    cout << "Status: " << movie->status << endl;
    cout << "Tagline: " << movie->tagline << endl;
    cout << "Rating: " << movie->voterAverage << endl;
}

void printBenchmarkResults(const string& title,
                           long long totalTime,
                           long long minTime,
                           long long maxTime,
                           int totalSearches) {
    double averageTime = 0.0;

    if (totalSearches > 0) {
        averageTime = (double) totalTime / totalSearches;
    }

    cout << "\n========== " << title << " ==========" << endl;
    cout << "Total Searches: " << totalSearches << endl;
    cout << "Average Time: " << averageTime << " microseconds" << endl;
    cout << "Min Time: " << minTime << " microseconds" << endl;
    cout << "Max Time: " << maxTime << " microseconds" << endl;
    cout << "======================================" << endl;
}

void benchmarkRankSearches(SplayTree& splayRankTree,
                           redBlackTree& rbPopularity,
                           int movieCount) {
    vector<int> testRanks;

    testRanks.push_back(1);

    if (movieCount >= 10) testRanks.push_back(10);
    if (movieCount >= 100) testRanks.push_back(100);
    if (movieCount >= 1000) testRanks.push_back(1000);
    if (movieCount >= 10000) testRanks.push_back(10000);

    if (movieCount > 1) {
        testRanks.push_back(movieCount / 4);
        testRanks.push_back(movieCount / 2);
        testRanks.push_back((movieCount * 3) / 4);
        testRanks.push_back(movieCount);
    }

    int repeats = 50;

    long long splayTotal = 0;
    long long rbTotal = 0;
    long long splayMin = numeric_limits<long long>::max();
    long long rbMin = numeric_limits<long long>::max();
    long long splayMax = 0;
    long long rbMax = 0;
    int totalSearches = 0;

    for (int rank : testRanks) {
        for (int i = 0; i < repeats; i++) {
            auto splayStart = high_resolution_clock::now();
            splayRankTree.searchByRank(rank);
            auto splayEnd = high_resolution_clock::now();

            auto rbStart = high_resolution_clock::now();
            rbPopularity.searchByRank(rank);
            auto rbEnd = high_resolution_clock::now();

            long long splayTime = duration_cast<microseconds>(splayEnd - splayStart).count();
            long long rbTime = duration_cast<microseconds>(rbEnd - rbStart).count();

            splayTotal += splayTime;
            rbTotal += rbTime;

            if (splayTime < splayMin) splayMin = splayTime;
            if (rbTime < rbMin) rbMin = rbTime;

            if (splayTime > splayMax) splayMax = splayTime;
            if (rbTime > rbMax) rbMax = rbTime;

            totalSearches++;
        }
    }

    printBenchmarkResults("Splay Rank Benchmark", splayTotal, splayMin, splayMax, totalSearches);
    printBenchmarkResults("Red-Black Rank Benchmark", rbTotal, rbMin, rbMax, totalSearches);
}

void benchmarkMovieIDSearches(SplayTree& splayMovieIDTree,
                              redBlackTree& rbMovieID,
                              const vector<TreeNode>& movies) {
    vector<long> testIDs;

    if (!movies.empty()) {
        testIDs.push_back(movies[0].movieID);
    }
    if (movies.size() >= 10) {
        testIDs.push_back(movies[9].movieID);
    }
    if (movies.size() >= 100) {
        testIDs.push_back(movies[99].movieID);
    }
    if (movies.size() >= 1000) {
        testIDs.push_back(movies[999].movieID);
    }
    if (movies.size() > 1) {
        testIDs.push_back(movies[movies.size() / 2].movieID);
        testIDs.push_back(movies.back().movieID);
    }

    int repeats = 20;

    long long splayTotal = 0;
    long long rbTotal = 0;
    long long splayMin = numeric_limits<long long>::max();
    long long rbMin = numeric_limits<long long>::max();
    long long splayMax = 0;
    long long rbMax = 0;
    int totalSearches = 0;

    for (long movieID : testIDs) {
        for (int i = 0; i < repeats; i++) {
            auto splayStart = high_resolution_clock::now();
            splayMovieIDTree.searchByMovieID(movieID);
            auto splayEnd = high_resolution_clock::now();

            auto rbStart = high_resolution_clock::now();
            rbMovieID.searchByMovieID(movieID);
            auto rbEnd = high_resolution_clock::now();

            long long splayTime = duration_cast<microseconds>(splayEnd - splayStart).count();
            long long rbTime = duration_cast<microseconds>(rbEnd - rbStart).count();

            splayTotal += splayTime;
            rbTotal += rbTime;

            if (splayTime < splayMin) splayMin = splayTime;
            if (rbTime < rbMin) rbMin = rbTime;

            if (splayTime > splayMax) splayMax = splayTime;
            if (rbTime > rbMax) rbMax = rbTime;

            totalSearches++;
        }
    }

    printBenchmarkResults("Splay Movie ID Benchmark", splayTotal, splayMin, splayMax, totalSearches);
    printBenchmarkResults("Red-Black Movie ID Benchmark", rbTotal, rbMin, rbMax, totalSearches);
}

int main() {
    Parser parser;
    vector<TreeNode> movies = parser.loadMovies("../src/movies.csv");
    parser.assignPopularityRanks(movies);

    if (movies.empty()) {
        cout << "No movies loaded." << endl;
        return 1;
    }

    redBlackTree rbPopularity(redBlackTree::BY_POPULARITY);
    redBlackTree rbMovieID(redBlackTree::BY_MOVIEID);

    SplayTree splayRankTree(SplayTree::BY_RANK);
    SplayTree splayMovieIDTree(SplayTree::BY_MOVIEID);

    for (size_t i = 0; i < movies.size(); i++) {
        rbPopularity.insert(new TreeNode(movies[i]));
        rbMovieID.insert(new TreeNode(movies[i]));

        splayRankTree.insert(&movies[i]);
        splayMovieIDTree.insert(&movies[i]);
    }

    cout << "Loaded " << movies.size() << " movies into the trees.\n" << endl;

    int choice = -1;

    while (choice != 0) {
        cout << "========== TEMP TEST MENU ==========" << endl;
        cout << "1. Search by popularity rank" << endl;
        cout << "2. Search by movie ID" << endl;
        cout << "3. Benchmark rank searches" << endl;
        cout << "4. Benchmark movie ID searches" << endl;
        cout << "0. Exit" << endl;
        cout << "====================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int rank;
            cout << "Enter popularity rank: ";
            cin >> rank;

            auto splayStart = high_resolution_clock::now();
            TreeNode* splayResult = splayRankTree.searchByRank(rank);
            auto splayEnd = high_resolution_clock::now();

            auto rbStart = high_resolution_clock::now();
            TreeNode* rbResult = rbPopularity.searchByRank(rank);
            auto rbEnd = high_resolution_clock::now();

            cout << "\n--- Splay Tree ---" << endl;
            printMovieBasic(splayResult);
            cout << "Search Time: "
                 << duration_cast<microseconds>(splayEnd - splayStart).count()
                 << " microseconds\n" << endl;

            cout << "--- Red-Black Tree ---" << endl;
            printMovieBasic(rbResult);
            cout << "Search Time: "
                 << duration_cast<microseconds>(rbEnd - rbStart).count()
                 << " microseconds\n" << endl;
        }
        else if (choice == 2) {
            long movieID;
            cout << "Enter movie ID: ";
            cin >> movieID;

            auto splayStart = high_resolution_clock::now();
            cout << "\n--- Splay Tree ---" << endl;
            splayMovieIDTree.searchByMovieID(movieID);
            auto splayEnd = high_resolution_clock::now();

            auto rbStart = high_resolution_clock::now();
            cout << "\n--- Red-Black Tree ---" << endl;
            rbMovieID.searchByMovieID(movieID);
            auto rbEnd = high_resolution_clock::now();

            cout << "\nSplay Search Time: "
                 << duration_cast<microseconds>(splayEnd - splayStart).count()
                 << " microseconds" << endl;

            cout << "Red-Black Search Time: "
                 << duration_cast<microseconds>(rbEnd - rbStart).count()
                 << " microseconds" << endl << endl;
        }
        else if (choice == 3) {
            benchmarkRankSearches(splayRankTree, rbPopularity, (int)movies.size());
        }
        else if (choice == 4) {
            benchmarkMovieIDSearches(splayMovieIDTree, rbMovieID, movies);
        }
        else if (choice == 0) {
            cout << "Exiting temporary test menu." << endl;
        }
        else {
            cout << "Invalid choice.\n" << endl;
        }
    }

    return 0;
}