
#include <cstdlib>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <utility>
#include <string.h>
#include <time.h>
#include <chrono>
#include <random>


using namespace std;

//Open Set Functions
//1. Find best node and remove it
//2. Neighbor visiting checks whether a node is in the set
//3. Neighbor visiting inserts new nodes
struct cityCoord {
	int city;
	int xVal;
	int yVal;
};

void nearestNeighbor( int start, vector<vector<int>> &adjMatrix, list<int> &Unvisited, queue<int> &Visited, int nodeCount );
void simAnneal( vector<vector<int>> &adjMatrix, vector<int> &destCities, queue<int> &Visited );
double acceptanceProbability( int energy, int newEnergy, double temp );
int getTourDistance( vector<vector<int>> &adjMatrix, vector<int> &tour );


int main( int argc, char *argv[] )
{
	clock_t t;
	ifstream inFile;
	ofstream outFile;
	string filename;
	vector <cityCoord> coords;
	string line;
	int a, b, c, row, col, distance, totalDistance;
	cityCoord aCity;
	bool flag = 0;
	pair<int, int> coordPair;
	map<pair<int, int>, int> coordMap;
	map<pair<int, int>, int> yCoordMap;
	vector<int> destCities;	
	vector< vector<int> > adjMatrix;
	int counter = 0;

	t = clock();
	
	//Read file to struct in vector
	if( argc != 2 )
		cout << "usage: " << argv[0] << " <filename>\n";
	else{
		inFile.open( argv[1] );
		
		if( !inFile.is_open() )
			cout << "Could not open file\n";
		else{
			filename.append( argv[1] );
			
			while(getline(inFile, line)){
				istringstream iss(line);
				if( !(iss >> aCity.city >> aCity.xVal >> aCity.yVal )) { cout << "Read error\n"; break; }
				coords.emplace_back( aCity );
			}
			flag = 1;
		}
		inFile.close();
	}

	//Store Coordinates in map as key, using std:pair TYPE,  with city as value
	//Maps are sorted by default, with coordinated pairs they are sorted by xval, then yval by default.
	for( a = 0; a < coords.size(); a++ ){ 
		coordMap[make_pair( coords[a].xVal, coords[a].yVal )] = a;
		counter++;
	}
	//cout << "\n\n********************* Map Coords By X-Val***** \n";	
	//Output  map Coordinates to test, using std:pair TYPE,  with city as value, map is sorted by coord pair by default
	/*for( map<pair<int,int>, int>::iterator it = coordMap.begin(); it != coordMap.end(); ++it ){ 
		cout << "City: " << it->second << " \t xval: " << get<0>(it->first) << "\tyval: " << get<1>(it->first) << "\n";
	}*/
	
	//Store Coordinates in map as key, using std:pair TYPE,  with city as value
	//Maps are sorted by default, with coordinated pairs they are sorted by y-val, then x-val.
	for( a = 0; a < coords.size(); a++ ){ 
		yCoordMap[make_pair( coords[a].yVal, coords[a].xVal )] = a;
	}
	//cout << "\n\n********************* Map Coords By Y-Val***** \n";	
	//Output  map Coordinates to test, using std:pair TYPE,  with city as value, map is sorted by coord pair by default
	/*for( map<pair<int,int>, int>::iterator it = yCoordMap.begin(); it != yCoordMap.end(); ++it ){ 
		cout << "City: " << it->second << " \t yval: " << get<0>(it->first) << "\t xval: " << get<1>(it->first) << "\n";
	}*/
	
	//AdjacencyMatrix -> [city][city]
	//works for large inputs b/c it puts 2D array on heap,
	// 30 seconds for test-input-7.txt -> 4999 nodes
	// 5 seconds and under for test-input-6.txt through test-input-1.txt -> 1999 nodes, 999, 499, 249, 99, 49 nodes
	// Over 3 minutes for tsp_example_3.txt -> 12,648 nodes
	// BUT!!! -> this is based on outputting the adjMatrix to the console, times should be about half this if we don't output adjMatrix
	// Non-output time is under 3 seconds to build AdjMatrix for example 7 vs 30 seconds to output it
	// for tsp_example_3.txt, non-output time is under 10 seconds to build vs over 3 minutes to output
//	if( counter < 500 ){
		row = counter;
		col = counter; 
		destCities.resize( row );
		adjMatrix.resize( row, vector<int>(col, 0));
	
		list<int> Unvisited;
		int x, y, xSq, ySq, xySum, xyDist;

		for( a = 0; a < row; a++ ){ 
		//	adjMatrix[a] = new int[col];
			Unvisited.push_back(a);			
			for( b = 0; b < counter; b++ ){
				x = coords[a].xVal - coords[b].xVal;
				y = coords[a].yVal - coords[b].yVal;
				xSq = x*x;
				ySq = y*y;
				xySum = xSq + ySq;
				xyDist = round(sqrt(xySum));
				adjMatrix[a][b] = xyDist;
			}
		}

		//Print AdjMatrix for testing
		/*
		cout << "\n\n********************* Adjacency Matrix***** \n	";
		cout << "   ";
		
		for( a = 0; a < counter; a++ ){ 
				cout << " " << a;
		}
		cout << "\n";
		
		for( a = 0; a < counter; a++ ){ 
			cout << a << ": ";
			
			for( b = 0; b < counter; b++ ){ 
				cout << " " <<adjMatrix[a][b];
			}
			cout << "\n";
		}*/
//	}

	queue<int> Visited;
	map<pair<int,int>, int>::iterator it = yCoordMap.begin();
	int start = it->second;

	//Algorithm for nearest neighbor
	nearestNeighbor( start, adjMatrix, Unvisited, Visited, counter );
/*	
	//Write nearestNeighbor to output file
	if( flag == 1 ){
		filename.append(  ".tour" );
		outFile.open( filename );
		
		if( !outFile.is_open() )
			cout << "Could not open file\n";
		else{
			while( Visited.size() > 0 ){
				outFile << Visited.front() << "\n";
				Visited.pop();
			}
		}
		outFile.close();
	}
*/
	simAnneal( adjMatrix, destCities, Visited ); 

	//Write simAnneal results to output file
	if( flag == 1 ){
		filename.append(  ".tour" );
		outFile.open( filename );
		
		if( !outFile.is_open() )
			cout << "Could not open file\n";
		else{
			for( vector<int>::iterator it=destCities.begin(); it != destCities.end(); ++it ){
				outFile << *it << "\n";
			}
		}
		outFile.close();
	}

	t = clock() - t;
	printf( "Seconds: %f\n", ((float)t)/CLOCKS_PER_SEC);

	return 0;
}

void nearestNeighbor( int start, vector<vector<int>> &adjMatrix, list<int> &Unvisited, queue<int> &Visited, int nodeCount ){

	int i;
	int nextVisit;
	int currentNode;
	int totalDistance = 0;
	int distance = 999999999;
	Visited.push(distance);
	Visited.push(start);
	Unvisited.remove( start );

	//printf( "Start Node: %d\n", start );
	//Search for minimum distance to next node
	for( i = 0; i < nodeCount; i++ ){
		if ( (adjMatrix[start][i] < distance) && (adjMatrix[start][i] > 0) ){
			distance =  adjMatrix[start][i];
			nextVisit = i;
		}
	}
	Unvisited.remove( nextVisit );
	currentNode = nextVisit;
	Visited.push(nextVisit);
	totalDistance += distance;
	//printf( "Next node: %d \t Distance: %d \t Running Dist: %d\n", nextVisit, distance, totalDistance); 
	distance = 999999999;

	while( !Unvisited.empty()  ){
		
		for( list<int>::iterator it=Unvisited.begin(); it != Unvisited.end(); ++it ){
			if ( (adjMatrix[currentNode][*it] < distance) && (adjMatrix[currentNode][*it] > 0) ){
				distance =  adjMatrix[currentNode][*it];
				nextVisit = *it;
				//printf( "New Next Visit: %d, distance: %d ", *it, distance );
			}
		}
		Unvisited.remove( nextVisit );
		//printf( "\n**** Current node: %d, Next Visit: %d, distance to: %d \n", currentNode, nextVisit, distance );
		currentNode = nextVisit;
		Visited.push(nextVisit);
		totalDistance += distance;
		//printf( "Next node: %d \t Distance: %d \t Running Dist: %d\n", nextVisit, distance, totalDistance); 
		distance = 999999999;
	}
	
	distance = adjMatrix[currentNode][start];
	totalDistance += distance;
	//printf( "Next node: %d \t Distance: %d \t Running Dist: %d\n", start, distance, totalDistance); 

	Visited.front() = totalDistance;
		
}


//Set Initial Temperature(high enough to allow moves) & Random Initial Solution
//Loop until Stop condition is met( good enough solution or cool enough temp
//Select neighbor by making change to solution
//Decide whether to move to neighbor
//Decrease temp and continue looping
	
void simAnneal( vector<vector<int>> &adjMatrix, vector<int> &destCities, queue<int> &Visited ){

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	double temp = 100000000000000;
	double coolingRate = 0.99999;
	vector<int> tour;
	vector<int> newTour;
	int tourPos1, tourPos2, currentEnergy, neighborEnergy;
	int distance = 0;
	int size = destCities.size() -1;
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0,size);
	uniform_real_distribution<double> doubleDist( 0.0, 1.0 );
	int tempCity;
	int bestDistance=0;

	tour.resize( destCities.size() );
	if( destCities.size() < 99 ){
		double temp = 100000000000;
		double coolingRate = 0.99999;
	}	
	//Optimum for test-1 was found at 5402
	// starting temp = 100000000000; coolingRate = .99999 while temp > .1 using new random path
	
	//Optimum for test-2 was found at 7697
	// starting temp = 1000000000; coolingRate = .999999 while temp > .001 using results from Greedy search

	//Use our nearest neighbor results to initialize the simulated annealing
	int i = 0;
	bestDistance = Visited.front();
	Visited.pop();
	while( !Visited.empty() ){
		tour[i] = Visited.front();
		printf( "%d : %d \n", i, tour[i]);
		Visited.pop();
		i++;
	}
	
	//Shuffles Tour Vector using C++ Algorithm Shuffle
	//shuffle( tour.begin(), tour.end(), default_random_engine(seed));
	//bestDistance = getTourDistance( adjMatrix, tour);

	//set destCities to tour as current best tour	
	destCities = tour;

	while( temp > 0.00001 ){
		newTour = tour;

		//get random cities
		tourPos1 = distribution(generator);
		tourPos2 = distribution(generator);

		//Swap Cities
		tempCity = newTour[tourPos1];
		newTour.at(tourPos1) = newTour.at(tourPos2);
		newTour.at(tourPos2) = tempCity;

		//Get Distances for tours
		currentEnergy = getTourDistance( adjMatrix, tour );
		neighborEnergy = getTourDistance( adjMatrix, newTour );

		//Evaluate accepting new tour
		if( acceptanceProbability( currentEnergy, neighborEnergy, temp ) > doubleDist( generator )){
			tour = newTour;
			currentEnergy = neighborEnergy;
		}

		//Save best solution
		if( currentEnergy  < bestDistance ){
			destCities = tour;
			bestDistance = currentEnergy;
			//printf( "Current Distance: %d \n", bestDistance );
		}

		//Cool system
		temp *= coolingRate;
	}
	printf( "Best Distance: %d \n", bestDistance );
	destCities.emplace( destCities.begin(), bestDistance );
}

double acceptanceProbability( int energy, int newEnergy, double temp ){
	if( newEnergy < energy ){
		return 1.0;
	}

	return exp( ( energy - newEnergy ) / temp );
}

int getTourDistance( vector<vector<int>> &adjMatrix, vector<int> &tour ){

	int distance=0;

	//Sum distance from each city in tour to the next
	for( int i=0; i < (tour.size()-1); i++ ){
		distance += adjMatrix[tour[i]][tour[i+1]];
	}

	distance += adjMatrix[tour[tour.size()-1]][tour[0]];
	
	return distance;
}





 
