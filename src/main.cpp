// Project2_CineRank.cpp : Input a CSV file and organize each movie in a red-black tree by a custom datapoint.
// Data points: Movie ID, title, genre, original language, overview, popularity, production companies, release date, budget, revenue, runtime, status, tagline, voter average, voter count, credits, keywords, poster path, backdrop path, recommendations

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "TreeNode.h"
using namespace std;



int main() {
	TreeNode node;

	// Open the CSV file
	ifstream file("movies.csv");

	if (!file.is_open()) {
		cerr << "Error opening file!" << endl;
		return 1;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string value;
		// Read each value from the line and assign it to the corresponding member of the TreeNode
		getline(ss, value, ',');
		node.movieID = stol(value); // Convert string to long
		getline(ss, node.title, ',');
		getline(ss, node.genre, ',');
		getline(ss, node.originalLanguage, ',');
		getline(ss, node.overview, ',');

		getline(ss, value, ',');
		node.popularity = stod(value); // Convert string to double
		getline(ss, node.productionCompanies, ',');
		getline(ss, node.releaseDate, ',');
		getline(ss, value, ',');
		node.budget = stol(value); // Convert string to long
		getline(ss, value, ',');
		node.revenue = stol(value); // Convert string to long
		getline(ss, value, ',');
		node.runtime = stoi(value); // Convert string to int
		getline(ss, node.status, ',');
		getline(ss, node.tagline, ',');
		getline(ss, value, ',');
		node.voterAverage = stod(value); // Convert string to double
		getline(ss, value, ',');
		node.voterCount = stoi(value); // Convert string to int
		getline(ss, node.credits, ',');
		getline(ss, node.keywords, ',');
		getline(ss, node.posterPath, ',');
		getline(ss, node.backdropPath, ',');
		getline(ss, node.recommendations);
	}
};
