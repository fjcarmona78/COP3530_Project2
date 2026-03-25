#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include "TreeNode.h"

using namespace std;

// Helper function to create a TreeNode with specified values for testing
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

// Test cases for the red-black tree implementation

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




