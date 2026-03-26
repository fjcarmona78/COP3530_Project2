#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "TreeNode.h"
#include "SplayTree.h"

using namespace std;

// Helper functions to create a TreeNode and Movie node with specified values for testing
static TreeNode* makeNode(long movieID,
    const string& title = "Movie",
    double popularity = 0.0,
    long revenue = 0) {
    TreeNode* node = new TreeNode();
    node->movieID = movieID;
    node->title = title;
    node->genre = "Genre";
    node->originalLanguage = "en";
    node->overview = "Overview";
    node->popularity = popularity;
    node->productionCompanies = "Studio";
    node->releaseDate = "2026-01-01";
    node->budget = 100;
    node->revenue = revenue;
    node->runtime = 120;
    node->status = "Released";
    node->tagline = "Tagline";
    node->voterAverage = 8.5;
    node->voterCount = 1000;
    node->credits = "Credits";
    node->keywords = "Keywords";
    node->posterPath = "/poster.jpg";
    node->backdropPath = "/backdrop.jpg";
    node->recommendations = "Recommendations";
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
    node->isRed = true;

    return node;
}

static Movie makeMovie(int rank,
    int id,
    const string& title = "Movie",
    long long revenue = 0) {
    Movie movie{};
    movie.popularityRank = rank;
    movie.id = id;
    movie.title = title;
    movie.revenue = revenue;
    return movie;
}

// --------------------------------------------------------------
// Test cases for the red-black tree implementation
// --------------------------------------------------------------

// Test case to verify that an empty tree returns an empty level order traversal
TEST_CASE("Empty tree level order is empty", "[tree][levelorder]") {
    redBlackTree tree(redBlackTree::BY_MOVIEID);

    vector<TreeNode*> result = tree.levelOrderTraversal();

    REQUIRE(result.empty());
}

// Test case to verify that inserting a single node into the tree correctly sets it as the root and colors it black
TEST_CASE("Single insert becomes root", "[tree][insert]") {
    redBlackTree tree(redBlackTree::BY_MOVIEID);
    tree.insert(makeNode(50, "Root Movie", 7.1, 5000));

    vector<TreeNode*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 1);
    REQUIRE(result[0]->movieID == 50);
    REQUIRE(result[0]->title == "Root Movie");
    REQUIRE(result[0]->isRed == false); // root should be black
}

// Test case to verify that inserting multiple nodes by movieID produces the expected structure in the tree
TEST_CASE("Insert by movieID produces expected small structure", "[tree][movieid]") {
    redBlackTree tree(redBlackTree::BY_MOVIEID);

    tree.insert(makeNode(50, "A", 5.0, 500));
    tree.insert(makeNode(25, "B", 3.0, 300));
    tree.insert(makeNode(75, "C", 9.0, 900));

    vector<TreeNode*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 3);
    REQUIRE(result[0]->movieID == 50);
    REQUIRE(result[1]->movieID == 25);
    REQUIRE(result[2]->movieID == 75);
}

// Test case to verify that inserting nodes by popularity correctly places them in the tree based on their popularity values
TEST_CASE("Insert by popularity uses popularity for placement", "[tree][popularity]") {
    redBlackTree tree(redBlackTree::BY_POPULARITY);

    tree.insert(makeNode(100, "Low", 2.0, 100));
    tree.insert(makeNode(200, "Mid", 5.0, 200));
    tree.insert(makeNode(300, "High", 8.0, 300));

    vector<TreeNode*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 3);

    bool lowFound = false;
    bool midFound = false;
    bool highFound = false;

    for (TreeNode* node : result) {
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
    redBlackTree tree(redBlackTree::BY_REVENUE);

    tree.insert(makeNode(100, "Small", 1.0, 1000));
    tree.insert(makeNode(200, "Medium", 1.0, 5000));
    tree.insert(makeNode(300, "Large", 1.0, 9000));

    vector<TreeNode*> result = tree.levelOrderTraversal();

    REQUIRE(result.size() == 3);

    bool smallFound = false;
    bool mediumFound = false;
    bool largeFound = false;

    for (TreeNode* node : result) {
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
    redBlackTree tree(redBlackTree::BY_MOVIEID);
    tree.insert(makeNode(42, "The Answer", 9.9, 420000));

    ostringstream captured;
    streambuf* oldCout = cout.rdbuf(captured.rdbuf());

    tree.searchByMovieID(42);

    cout.rdbuf(oldCout);
    string output = captured.str();

    REQUIRE(output.find("Movie ID: 42") != string::npos);
    REQUIRE(output.find("Title: The Answer") != string::npos);
    REQUIRE(output.find("Popularity: 9.9") != string::npos);
    REQUIRE(output.find("Revenue: 420000") != string::npos);
}

// Test case to verify that searching for a movie by its ID correctly prints a not found message when the movie is not present in the tree
TEST_CASE("searchByMovieID prints not found message when missing", "[tree][search]") {
    redBlackTree tree(redBlackTree::BY_MOVIEID);
    tree.insert(makeNode(10, "Only Movie", 1.0, 100));

    ostringstream captured;
    streambuf* oldCout = cout.rdbuf(captured.rdbuf());

    tree.searchByMovieID(999);

    cout.rdbuf(oldCout);
    string output = captured.str();

    REQUIRE(output.find("Movie with ID 999 not found.") != string::npos);
}

// --------------------------------------------------------------
// Splay tree tests
// --------------------------------------------------------------

// Test case to verify that an empty splay tree reports as empty and that searches for any movie return nullptr
TEST_CASE("Empty splay tree reports empty and missing searches return nullptr", "[splay][empty]") {
    SplayTree tree;

    REQUIRE(tree.empty());
    REQUIRE(tree.search(10) == nullptr);
    REQUIRE(tree.searchByRank(10) == nullptr);
    REQUIRE(tree.searchById(10) == nullptr);
    REQUIRE(tree.searchByRevenue(1000) == nullptr);
    REQUIRE(tree.getAllMovies().empty());
}

// Test case to verify that inserting a single movie into the splay tree allows it to be found by rank, id, and search, and that the tree is no longer empty
TEST_CASE("Single insert in splay tree can be found", "[splay][insert][search]") {
    SplayTree tree;
    Movie movie = makeMovie(10, 101, "Solo", 1000);

    tree.insert(movie);

    REQUIRE_FALSE(tree.empty());

    Movie* foundByRank = tree.searchByRank(10);
    REQUIRE(foundByRank != nullptr);
    REQUIRE(foundByRank->id == 101);
    REQUIRE(foundByRank->title == "Solo");
    REQUIRE(foundByRank->revenue == 1000);

    Movie* foundBySearch = tree.search(10);
    REQUIRE(foundBySearch != nullptr);
    REQUIRE(foundBySearch->id == 101);

    Movie* foundById = tree.searchById(101);
    REQUIRE(foundById != nullptr);
    REQUIRE(foundById->popularityRank == 10);
}

// Test case to verify that inserting multiple movies into the splay tree results in an inorder traversal that is sorted by popularity rank
TEST_CASE("Splay tree inorder data is sorted by popularity rank after inserts", "[splay][order]") {
    SplayTree tree;

    tree.insert(makeMovie(30, 300, "Thirty", 3000));
    tree.insert(makeMovie(10, 100, "Ten", 1000));
    tree.insert(makeMovie(20, 200, "Twenty", 2000));
    tree.insert(makeMovie(40, 400, "Forty", 4000));

    vector<Movie> movies = tree.getAllMovies();

    REQUIRE(movies.size() == 4);
    REQUIRE(movies[0].popularityRank == 10);
    REQUIRE(movies[1].popularityRank == 20);
    REQUIRE(movies[2].popularityRank == 30);
    REQUIRE(movies[3].popularityRank == 40);
}

// Test case to verify that searching for a movie by its popularity rank in the splay tree correctly returns the movie details when the movie is found
TEST_CASE("Splay tree can search by rank", "[splay][rank]") {
    SplayTree tree;

    tree.insert(makeMovie(30, 300, "Thirty", 3000));
    tree.insert(makeMovie(10, 100, "Ten", 1000));
    tree.insert(makeMovie(20, 200, "Twenty", 2000));
    tree.insert(makeMovie(40, 400, "Forty", 4000));

    Movie* found = tree.searchByRank(20);

    REQUIRE(found != nullptr);
    REQUIRE(found->id == 200);
    REQUIRE(found->title == "Twenty");
    REQUIRE(found->revenue == 2000);
}

// Test case to verify that searching for a movie by its ID and revenue in the splay tree correctly returns the movie details when the movie is found
TEST_CASE("Splay tree can search by id and revenue", "[splay][search]") {
    SplayTree tree;

    tree.insert(makeMovie(5, 501, "Alpha", 1500));
    tree.insert(makeMovie(15, 502, "Beta", 2500));
    tree.insert(makeMovie(25, 503, "Gamma", 3500));

    Movie* byId = tree.searchById(502);
    REQUIRE(byId != nullptr);
    REQUIRE(byId->title == "Beta");
    REQUIRE(byId->popularityRank == 15);

    Movie* byRevenue = tree.searchByRevenue(3500);
    REQUIRE(byRevenue != nullptr);
    REQUIRE(byRevenue->title == "Gamma");
    REQUIRE(byRevenue->id == 503);
}

// Test case to verify that searching for a movie by a non-existent popularity rank, ID, or revenue in the splay tree correctly returns nullpt
TEST_CASE("Missing splay tree searches return nullptr", "[splay][missing]") {
    SplayTree tree;

    tree.insert(makeMovie(10, 1001, "One", 10000));
    tree.insert(makeMovie(20, 1002, "Two", 20000));

    REQUIRE(tree.search(999) == nullptr);
    REQUIRE(tree.searchByRank(999) == nullptr);
    REQUIRE(tree.searchById(9999) == nullptr);
    REQUIRE(tree.searchByRevenue(999999) == nullptr);
}

// Test case to verify that inserting a movie with a duplicate popularity rank into the splay tree does not create a second node and that the original movie remains unchanged
TEST_CASE("Duplicate popularity rank does not create a second node", "[splay][duplicate]") {
    SplayTree tree;

    tree.insert(makeMovie(50, 5001, "Original", 1111));
    tree.insert(makeMovie(50, 5002, "Duplicate", 2222));

    vector<Movie> movies = tree.getAllMovies();

    REQUIRE(movies.size() == 1);
    REQUIRE(movies[0].popularityRank == 50);
    REQUIRE(movies[0].id == 5001);
    REQUIRE(movies[0].title == "Original");
}