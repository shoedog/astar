
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

void buildVisits( int start, vector<vector<int>> &adjMatrix, list<int> &Unvisited, queue<int> &Visited, int nodeCount );


int main( int argc, char *argv[] )
{
	clock_t t;
	ifstream inFile;
	ofstream outFile;
	string filename;
	vector <cityCoord> coords;
	string line;
	int a, b, c;
	cityCoord aCity;
	int distance;
	int totalDistance;
	bool flag = 0;
	pair<int, int> coordPair;
	map<pair<int, int>, int> coordMap;
	map<pair<int, int>, int> yCoordMap;
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
		int row = counter;
		int col = counter; 
		//int **adjMatrix = new int*[row];	
		vector< vector<int> > adjMatrix;
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
	buildVisits( start, adjMatrix, Unvisited, Visited, counter );

	//delete [] adjMatrix;

	//Write Queue to output file
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

	t = clock() - t;
	printf( "Seconds: %f", ((float)t)/CLOCKS_PER_SEC);

	return 0;
}

void buildVisits( int start, vector<vector<int>> &adjMatrix, list<int> &Unvisited, queue<int> &Visited, int nodeCount ){

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







 
