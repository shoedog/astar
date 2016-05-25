
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
