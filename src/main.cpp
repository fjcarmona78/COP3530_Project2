// Project2_CineRank.cpp : Input a CSV file and organize each movie in a red-black tree by a custom datapoint. 
// Data points: Movie ID, title, genre, original language, overview, popularity, production companies, release date, budget, revenue, runtime, status, tagline, voter average, voter count, credits, keywords, poster path, backdrop path, recommendations

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "WindowManager.h"
#include "SplayTree.h"
#include "RBTree.h"
#include "Parser.h"

int main() {
	int opt = -1;
	do {
		std::cout << "Splay (0) or Red/Black (1)?" << std::endl;
		std::cin >> opt;
	} while (!(opt == 1 || opt == 0));
	
	BSTMovie *treeRank;
	BSTMovie *treeID;
	if (opt == 0) {
		treeID = new SplayTree();
		treeRank = new SplayTree(BY_RANK);
	}
	else {
		treeRank = new redBlackTree(BY_RANK);
		treeID = new redBlackTree();
	}

	Parser parser;

	std::cout << "Parsing the File" << std::endl;
	std::vector<Movie> movies = parser.loadMovies("../src/first120k.csv", 120000);
	parser.assignPopularityRanks(movies);

	std::cout << "Inserting movies into the tree" << std::endl;
	for (auto m : movies) {
		treeID->insert(m);
		treeRank->insert(m);
	}

	bool temp = opt == 1 ? false : true;
	WindowManager wm(temp, treeRank, treeID);

	std::cout << "Creating the plot" << std::endl;
	for (auto m : movies) {
		wm.insertGraphPoint(m.revenue, m.popularity);
	}

	wm.calculateStats();

	std::cout << "Cleaning memory up" << std::endl;
	std::vector<Movie>().swap(movies);

    while (!wm.shouldClose()) {
		wm.update();
		wm.render();
    }
 
    return 0;
}