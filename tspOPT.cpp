#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <ctime>
#include <vector>

using namespace std;

ifstream inFile;
ofstream outFile;

bool arrayCheck(int arr[], int test, int length);
int lineCount(char *infileName);
int setCoordinates(string cities[], int lines, int x[], int y[]);
int tourDistance(float x1, float y1, float x2, float y2);
int tourLength(int stops[], int vertices, int x[], int y[]);
int greedy(int stops[], int vertices, int x[], int y[]);
int opt2(int stops[], int vertices, int x[], int y[]);
int opt3(int stops[], int vertices, int x[], int y[]);
void results(int length, int vertices, int stops[], string cities[]);
int getTourDistance( vector<vector<int>> &adjMatrix, vector<int> &tour ){
	int distance=0;

	for( int i=0; i < (tour.size()-1); i++ ){
		distance += adjMatrix[tour[i]][tour[i+1]];
	}

	distance += adjMatrix[tour[tour.size()-1]][tour[0]];
	
	return distance;
}


int main(int argc, char *argv[])
{
	int lines = lineCount(argv[1]);
	
	int xDiff, yDiff, xSqrd, ySqrd, xySum, xyDist;
	vector< vector<int> > adjMatrix;
	adjMatrix.resize( lines, vector<int>(lines, 0));
	
	int lines = lineCount(argv[1]);
	double runTime = 0.0; 
    clock_t start, stop;
	int optArg = 1;
	start = clock();
	if (argc > 2) optArg = atoi(argv[2]);
	string cities[lines];
	int xarr[lines];
	int yarr[lines];
	int stops[lines];
  	for (int i = 0; i < lines; i++) stops[i] = -1;
	setCoordinates(cities, lines, xarr, yarr);

	//Store distance in Adjacency Matrix so that Distances between cities
	// only need to be calculated the one time and then can be accessed by
	// adjMatrix[city1][city2] for all cities. Should save time for larger data sets
	// since there are a lot of calls to tourDistance() in loops, so the same distance is calculated
	// multiple times like nested loops in tourLength that call tourdistance in 2-OPT & 3-OPT. Then 
	// tourLength can just sum the distances that are stored in the matrix. See getTourDistance() function
	// above for an example. adjMatrix and a Vector holding the order of cities visited in order is looped
	// over to sum the total distance. 
	// Need to refactor code calls to tourDistance() in order to use and use city/stop number in adjMatrix 
	// For example in greedy() it would be nextDistance = adjMatrix[current][j];
	for( int i = 0; i < lines; i++ ){ 
		Unvisited.push_back(a);			
		for( int j = 0; j < lines; j++ ){
			xDiff = xarr[i] - xarr[j];
			yDiff = yarr[i] - yarr[j];
			xSqrd = xDiff*xDiff;
			ySqrd = yDiff*yDiff;
			xySum = xSqrd + ySqrd;
			xyDist = round(sqrt(xySum));
			adjMatrix[i][j] = xyDist;
		}
	}

	int totalDistance = greedy(stops, lines, xarr, yarr);
	int tempDistance;
 	if (optArg == 2) {
		do {
			start = clock();
			tempDistance = totalDistance;
			totalDistance = opt2(stops, lines, xarr, yarr);
		} while (tempDistance != totalDistance);
	} else if (optArg == 3) {
		do {
			start = clock();
			tempDistance = totalDistance;
			totalDistance = opt3(stops, lines, xarr, yarr);
		} while (tempDistance != totalDistance);
	} 
	
	stop = clock();
	  runTime = ((double)(stop - start) / CLOCKS_PER_SEC);
	  std::cout << "Algorithm Runtime: " << runTime << std::endl;//display run time
	results(totalDistance, lines, stops, cities);//output to file 
	return 0;
}



bool arrayCheck(int arr[], int test, int length){
  for (int i = 0; i < length; i++) {
    if (arr[i] == test) return true;
  }
  return false;
}

int lineCount(char *infileName)
{
  int count = 0;
  inFile.open(infileName);

  string outfileName = infileName;
  outfileName += ".tour";
  outFile.open(outfileName);

	for (string line; getline(inFile, line); ++count);
	inFile.clear();
	inFile.seekg(0, ios::beg);
	return count;
}

int setCoordinates(string cities[], int lines, int x[], int y[])
{
	string str;
	int i = 0;
	while (i < lines)
	{
		inFile >> str;
		cities[i] = str;
		inFile >> str;
		x[i] = stoi(str);
		inFile >> str;
		y[i] = stoi(str);
		i++;
	}
}


int tourDistance(float x1, float y1, float x2, float y2) {
  return (int) (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) + .5);
}

int tourLength(int x[], int y[], int stops[], int vertices) {
  int length = 0;

  for (int i = 0; i < vertices - 1; i++) {
    length += tourDistance(x[stops[i]], y[stops[i]], x[stops[i+1]], y[stops[i+1]]);
  }
  length += tourDistance(x[stops[vertices - 1]], y[stops[vertices - 1]], x[stops[0]], y[stops[0]]);

  return length;
}

/*Greedy -- To initialize for OPT*/

int greedy(int stops[], int points, int x[], int y[])
{
  int length = 0;
  int bestPoint = 0; 
  int current, bestDistance, nextDistance;
  bool firstDistance;

  for (int i = 0; i < points; i++) {
    current = bestPoint;
    stops[i] = current;
    firstDistance = true;
    bestDistance = 0;
    for (int j = 0; j < points; j++) {
      if (arrayCheck(stops, j, points)) continue;
      nextDistance = tourDistance(x[current], y[current], x[j], y[j]);
      if (nextDistance < bestDistance || firstDistance) {
	bestDistance = nextDistance;
	bestPoint = j;
	firstDistance = false;
      }
    }
      length += bestDistance;
  }

  length += tourDistance(x[stops[points - 1]], y[stops[points - 1]], x[stops[0]], y[stops[0]]);
//  cout << "Greedy tour length: " << length << endl;
  return length;
}

/*2-OPT*/
int opt2(int stops[], int vertices, int x[], int y[])
{
  int length = 0;
  int index, bestDistance, nextDistance;
  bool flag;

  for (int i = 0; i < vertices - 1; i++) {
    bestDistance = tourLength(x, y, stops, vertices);
    index = i;
    for (int j = i+1; j < vertices; j++) {
      swap(stops[i], stops[j]);
      nextDistance = tourLength(x, y, stops, vertices);
      swap(stops[i], stops[j]);
      if (nextDistance < bestDistance) {
  	bestDistance = nextDistance;
	index = j;
      }
    }
    if (index != i) {
      swap(stops[i], stops[index]);
      flag = true;
    }
  }
  length = tourLength(x, y, stops, vertices);
  cout << "2-OPT length: " << length << endl;
  return length;
}

/*3-OPT Algorithm*/
int opt3(int stops[], int vertices, int x[], int y[])
{
  int length = 0, index1, index2, bestDistance, nextDistance;
  bool isChange; 
  
  for (int i = 0; i < vertices - 1; i++) {
    bestDistance = tourLength(x, y, stops, vertices);
    index1 = i;
    index2 = i;
    for (int j = i+1; j < vertices; j++) {
      swap(stops[i], stops[j]);
      nextDistance = tourLength(x, y, stops, vertices);
      swap(stops[i], stops[j]);
      if (nextDistance < bestDistance) {
  	bestDistance = nextDistance;
	index1 = j;
        index2 = i;
      }
      for (int k = j+1; k < vertices; k++) {
      swap(stops[i], stops[j]); 
      swap(stops[j], stops[k]); 
      nextDistance = tourLength(x, y, stops, vertices);
      swap(stops[j], stops[k]);
      swap(stops[i], stops[j]);
      if (nextDistance < bestDistance) {
  	bestDistance = nextDistance;
	index1 = j;
	index2 = k;
      }
      swap(stops[i], stops[k]); 
      swap(stops[j], stops[k]);
      nextDistance = tourLength(x, y, stops, vertices);
      swap(stops[j], stops[k]);
      swap(stops[i], stops[k]);
      if (nextDistance < bestDistance) {
  	bestDistance = nextDistance;
	index1 = k;
	index2 = j;
      }
      }
    }

    if (index1 != i) {
      if (index2 != i) {
	swap(stops[i], stops[index1]);
	swap(stops[index1], stops[index2]);
      } else {
        swap(stops[i], stops[index1]);
      }
      isChange = true;
    }
  }
  length = tourLength(x, y, stops, vertices);
  cout << "3-OPT length: " << length << endl;
  return length;
}

void results(int length, int vertices, int stops[], string cities[]) {
  outFile << length << endl;
  for (int i = 0; i < vertices; i++) {
    string identifier = cities[stops[i]];
    outFile << identifier << endl;
  } 	
}
