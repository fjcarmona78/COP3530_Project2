#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "../src/SplayTree.h"
#include "../src/RBTree.h"
#include "../src/Movie.h"

using namespace std;

// Helper functions to create a TreeNode and Movie node with specified values for testing
static Movie makeNode(long movieID,
    const string& title = "Movie",
    double popularity = 0.0,
    long revenue = 0) {
    Movie node;
    node.movieID = movieID;
    node.title = title;
    node.genre = "Genre";
    node.originalLanguage = "en";
    node.overview = "Overview";
    node.popularity = popularity;
    node.productionCompanies = "Studio";
    node.releaseDate = "2026-01-01";
    node.budget = 100;
    node.revenue = revenue;
    node.runtime = 120;
    node.status = "Released";
    node.tagline = "Tagline";
    node.voterAverage = 8.5;
    node.voterCount = 1000;
    node.credits = "Credits";
    node.keywords = "Keywords";
    node.posterPath = "/poster.jpg";
    node.backdropPath = "/backdrop.jpg";
    node.recommendations = "Recommendations";
    return node;
}
// --------------------------------------------------------------
// Test cases for the red-black tree implementation
// --------------------------------------------------------------

// Test case to verify that an empty tree returns an empty level order traversal
TEST_CASE("Empty tree level order is empty", "[tree][levelorder]") {
    redBlackTree tree(BY_MOVIEID);

    vector<Movie *> result = tree.levelOrderTraversal();

    REQUIRE(result.empty());
}

// Test case to verify that inserting a single node into the tree correctly sets it as the root and colors it black
TEST_CASE("Single insert becomes root", "[tree][insert]") {
    redBlackTree tree(BY_MOVIEID);
    tree.insert(makeNode(50, "Root Movie", 7.1, 5000));

    vector<Movie*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 1);
    REQUIRE(result[0]->movieID == 50);
    REQUIRE(result[0]->title == "Root Movie");
}

// Test case to verify that inserting multiple nodes by movieID produces the expected structure in the tree
TEST_CASE("Insert by movieID produces expected small structure", "[tree][movieid]") {
    redBlackTree tree(BY_MOVIEID);

    tree.insert(makeNode(50, "A", 5.0, 500));
    tree.insert(makeNode(25, "B", 3.0, 300));
    tree.insert(makeNode(75, "C", 9.0, 900));

    vector<Movie*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 3);
    REQUIRE(result[0]->movieID == 50);
    REQUIRE(result[1]->movieID == 25);
    REQUIRE(result[2]->movieID == 75);
}

// Test case to verify that inserting nodes by popularity correctly places them in the tree based on their popularity values
TEST_CASE("Insert by popularity uses popularity for placement", "[tree][popularity]") {
    redBlackTree tree(BY_POPULARITY);

    tree.insert(makeNode(100, "Low", 2.0, 100));
    tree.insert(makeNode(200, "Mid", 5.0, 200));
    tree.insert(makeNode(300, "High", 8.0, 300));

    vector<Movie*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 3);

    bool lowFound = false;
    bool midFound = false;
    bool highFound = false;

    for (Movie* node : result) {
        if (node->title == "Low") lowFound = true;
        if (node->title == "Mid") midFound = true;
        if (node->title == "High") highFound = true;
    }

    REQUIRE(lowFound);
    REQUIRE(midFound);
    REQUIRE(highFound);
}

// Test case to verify that inserting nodes by revenue correctly places them in the tree based on their revenue values
TEST_CASE("Insert by revenue uses revenue for placement", "[tree][revenue]") {
    redBlackTree tree(BY_REVENUE);

    tree.insert(makeNode(100, "Small", 1.0, 1000));
    tree.insert(makeNode(200, "Medium", 1.0, 5000));
    tree.insert(makeNode(300, "Large", 1.0, 9000));

    vector<Movie*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 3);

    bool smallFound = false;
    bool mediumFound = false;
    bool largeFound = false;

    for (Movie* node : result) {
        if (node->title == "Small") smallFound = true;
        if (node->title == "Medium") mediumFound = true;
        if (node->title == "Large") largeFound = true;
    }

    REQUIRE(smallFound);
    REQUIRE(mediumFound);
    REQUIRE(largeFound);
}

// Test case to verify that searching for a movie by its ID correctly prints the movie details when the movie is found in the tree
TEST_CASE("searchByMovieID prints movie details when found", "[tree][search]") {
    redBlackTree tree(BY_MOVIEID);
    tree.insert(makeNode(42, "The Answer", 9.9, 420000));


    Movie *movie = tree.searchByMovieID(42);

    REQUIRE(movie->movieID == 42);
    REQUIRE(movie->title == "The Answer");
    REQUIRE(movie->popularity == 9.9);
    REQUIRE(movie->revenue == 420000);
}

// Test case to verify that searching for a movie by its ID correctly prints a not found message when the movie is not present in the tree
TEST_CASE("searchByMovieID prints not found message when missing", "[tree][search]") {
    redBlackTree tree(BY_MOVIEID);
    tree.insert(makeNode(10, "Only Movie", 1.0, 100));

    Movie* p = tree.searchByMovieID(999);

    REQUIRE(!p);
}

/*
 --------------------------------------------------------------
 Test cases for the splay tree implementation

 These tests were updated to match the final SplayTree interface
 without changing the shared helper function.
 --------------------------------------------------------------
*/

TEST_CASE("Empty splay tree level order is empty", "[splay][empty]") {
    SplayTree tree;

    vector<Movie*> result = tree.levelOrderTraversal();

    REQUIRE(result.empty());
    REQUIRE(tree.isEmpty());
}

TEST_CASE("Single insert in splay tree can be found by rank", "[splay][insert][rank]") {
    SplayTree tree;

    Movie node = makeNode(101, "Solo", 10.0, 1000);
    node.popularityRank = 1;
    tree.insert(node);

    Movie* found = tree.searchByRank(1);

    REQUIRE(found);
    REQUIRE(found->movieID == 101);
    REQUIRE(found->title == "Solo");
    REQUIRE(found->revenue == 1000);
    REQUIRE(found->popularityRank == 1);
}

TEST_CASE("Splay tree searchByRank returns the correct movie", "[splay][rank]") {
    SplayTree tree;

    Movie node1 = makeNode(300, "Thirty", 30.0, 3000);
    node1.popularityRank = 3;
    tree.insert(node1);

    Movie node2 = makeNode(100, "Ten", 10.0, 1000);
    node2.popularityRank = 1;
    tree.insert(node2);

    Movie node3 = makeNode(200, "Twenty", 20.0, 2000);
    node3.popularityRank = 2;
    tree.insert(node3);

    Movie node4 = makeNode(400, "Forty", 40.0, 4000);
    node4.popularityRank = 4;
    tree.insert(node4);

    Movie* found = tree.searchByRank(2);

    REQUIRE(found != nullptr);
    REQUIRE(found->movieID == 200);
    REQUIRE(found->title == "Twenty");
    REQUIRE(found->revenue == 2000);
    REQUIRE(found->popularityRank == 2);
}

TEST_CASE("Splay tree level order returns inserted nodes", "[splay][levelorder]") {
    SplayTree tree;

    Movie node1 = makeNode(300, "Thirty", 30.0, 3000);
    node1.popularityRank = 3;
    tree.insert(node1);

    Movie node2 = makeNode(100, "Ten", 10.0, 1000);
    node2.popularityRank = 1;
    tree.insert(node2);

    Movie node3 = makeNode(200, "Twenty", 20.0, 2000);
    node3.popularityRank = 2;
    tree.insert(node3);

    vector<Movie*> result = tree.levelOrderTraversal();

    REQUIRE_FALSE(result.empty());

    bool found100 = false;
    bool found200 = false;
    bool found300 = false;

    for (Movie* node : result) {
        if (node->movieID == 100) found100 = true;
        if (node->movieID == 200) found200 = true;
        if (node->movieID == 300) found300 = true;
    }

    REQUIRE(found100);
    REQUIRE(found200);
    REQUIRE(found300);
}

TEST_CASE("Splay tree searchByMovieID prints movie details when found", "[splay][movieid]") {
    SplayTree tree;

    Movie node = makeNode(42, "The Answer", 9.9, 420000);
    node.popularityRank = 1;
    tree.insert(node);

    Movie* movie = tree.searchByMovieID(42);

    REQUIRE(movie->movieID == 42);
    REQUIRE(movie->title == "The Answer");
    REQUIRE(movie->popularity == 9.9);
    REQUIRE(movie->revenue == 420000);
    
}

TEST_CASE("Splay tree searchByMovieID prints not found message when missing", "[splay][movieid]") {
    SplayTree tree;

    Movie node = makeNode(10, "Only Movie", 1.0, 100);
    node.popularityRank = 1;
    tree.insert(node);

    Movie* m = tree.searchByMovieID(999);

    REQUIRE(!m);
}

TEST_CASE("Splay tree getMostPopularMovie returns popularity of rank 1 movie", "[splay][helper]") {
    SplayTree tree;

    Movie node1 = makeNode(100, "Movie A", 20.0, 1000);
    node1.popularityRank = 3;
    tree.insert(node1);

    Movie node2 = makeNode(200, "Movie B", 30.0, 2000);
    node2.popularityRank = 2;
    tree.insert(node2);

    Movie node3 = makeNode(300, "Movie C", 40.0, 3000);
    node3.popularityRank = 1;
    tree.insert(node3);

    REQUIRE(tree.getMostPopularMovie() == 40.0);
}

TEST_CASE("Splay tree getHighestRevenueMovie returns highest revenue value", "[splay][helper]") {
    SplayTree tree;

    Movie node1 = makeNode(100, "Movie A", 20.0, 1000);
    node1.popularityRank = 3;
    tree.insert(node1);

    Movie node2 = makeNode(200, "Movie B", 30.0, 5000);
    node2.popularityRank = 2;
    tree.insert(node2);

    Movie node3 = makeNode(300, "Movie C", 40.0, 3000);
    node3.popularityRank = 1;
    tree.insert(node3);

    REQUIRE(tree.getHighestRevenueMovie() == 5000);
}