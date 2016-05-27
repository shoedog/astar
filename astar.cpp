
#include <cstdlib>
#include <vector>
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


int main( int argc, char *argv[] )
{
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

	//Testing Calc for distance from City: 0 to all cities
	for( a = 1; a < coords.size(); a++ ){ 
		distance = sqrt( pow((coords[0].xVal-coords[a].xVal), 2 ) + pow((coords[0].yVal-coords[a].yVal), 2) );
		cout << "Elem " << a << " Distance from 0 = " << distance << "\n";
		totalDistance += distance;
	}

	cout << "Total Distance: " << totalDistance;
 
	//Store Coordinates in map as key, using std:pair TYPE,  with city as value
	//Maps are sorted by default, with coordinated pairs they are sorted by xval, then yval by default.
	for( a = 0; a < coords.size(); a++ ){ 
		coordMap[make_pair( coords[a].xVal, coords[a].yVal )] = a;
		counter++;
	}
	cout << "\n\n********************* Map Coords By X-Val***** \n";	
	//Out map Coordinates to test, using std:pair TYPE,  with city as value, map is sorted by coord pair by default
	for( map<pair<int,int>, int>::iterator it = coordMap.begin(); it != coordMap.end(); ++it ){ 
		cout << "City: " << it->second << " \t xval: " << get<0>(it->first) << "\tyval: " << get<1>(it->first) << "\n";
	}
	
	//Store Coordinates in map as key, using std:pair TYPE,  with city as value
	//Maps are sorted by default, with coordinated pairs they are sorted by y-val, then x-val.
	for( a = 0; a < coords.size(); a++ ){ 
		yCoordMap[make_pair( coords[a].yVal, coords[a].xVal )] = a;
	}
	cout << "\n\n********************* Map Coords By Y-Val***** \n";	
	//Out map Coordinates to test, using std:pair TYPE,  with city as value, map is sorted by coord pair by default
	for( map<pair<int,int>, int>::iterator it = yCoordMap.begin(); it != yCoordMap.end(); ++it ){ 
		cout << "City: " << it->second << " \t yval: " << get<0>(it->first) << "\t xval: " << get<1>(it->first) << "\n";
	}
	
	//AdjacencyMatrix
	//Doesn't work for large inputs like example 3
	if( counter < 500 ){
		int adjMatrix[counter][counter]; 
		
		for( a = 0; a < counter; a++ ){ 
	
			for( b = 0; b < counter; b++ ){ 
				adjMatrix[a][b] = sqrt( pow((coords[a].xVal-coords[b].xVal), 2 ) + pow((coords[a].yVal-coords[b].yVal), 2) );
			}
		}

		//Print AdjMatrix for testing
		//
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
		}
	}

	//Write vector struct to output file
	if( flag == 1 ){
		filename.append(  ".tour" );
		outFile.open( filename );
		
		if( !outFile.is_open() )
			cout << "Could not open file\n";
		else{
			while( !coords.empty() ){
				outFile << coords[0].city << " " << coords[0].xVal << " " << coords[0].yVal << "\n";
				coords.erase( coords.begin() );
			}
		}
		outFile.close();
	}

	return 0;
}
