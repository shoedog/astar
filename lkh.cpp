
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
struct Coord {
	int city;
	int xVal;
	int yVal;
};

void createCandidateSet();
double Ascent();
void generateCandidates(int, int);
double FindTour();
void readWriteFile( ifstream &inFile, ofstream &outFile, string filename, vector<Coord> &coords, vector<int> &tour, bool option );

int main( int argc, char *argv[] )
{
	clock_t t;
	ifstream inFile;
	ofstream outFile;
	string filename;
	vector <Coord> coords;
	vector<int> tour;	
	vector< vector<int> > adjMatrix;
	int bestCost, cost, run, runs;
	int counter = 0;

	t = clock();
	
	//Read file to struct in vector
	if( argc != 2 )
		cout << "usage: " << argv[0] << " <filename>\n";
	else{
		filename.append( argv[1] );
		readWriteFile( inFile, outFile, filename, coords, tour, 0 ); 	
	}

	createCandidateSet();
	bestCost = INT_MAX;
	for( run = 1; run <= runs; run++ ){
		cost = findTour();
		if( cost < bestCost ){
			recordBestTour();
			bestCost = cost;
		}
	}
	saveBestTour();

	readWriteFile( inFile, outFile, filename, coords, tour, 1 ); 	
	t = clock() - t;
	printf( "Seconds: %f\n", ((float)t)/CLOCKS_PER_SEC);

	return 0;
}

//Reads or Writes a file
// Option: 0 = Open to Read
// Option: 1 = Open to Write

void readWriteFile( ifstream &inFile, ofstream &outFile, string filename, vector<Coord> &coords, vector<int> &tour, bool option ){
	
	string line;
	Coord oneCity;

	//Read cities and coordinates in
	if( option == 0 ){
		inFile.open( filename );
		
		if( !inFile.is_open() )
			cout << "Could not open file\n";
		else{
			while(getline(inFile, line)){
				istringstream iss(line);
				if( !(iss >> oneCity.city >> oneCity.xVal >> oneCity.yVal )) { cout << "Read error\n"; break; }
				coords.emplace_back( oneCity );
			}
		}
		inFile.close();

	//Write results to output file
	} else if ( option == 1 ){
			filename.append(  ".tour" );
			outFile.open( filename );
			
			if( !outFile.is_open() )
				cout << "Could not open file\n";
			else{
				for( vector<int>::iterator it=tour.begin(); it != tour.end(); ++it ){
					outFile << *it << "\n";
				}
			outFile.close();
			}
		
	} else {
		printf( "ERROR: USAGE Read/Write function: 0 to read, 1 to write\n");
	}
}

void createCandidateSet(){
	double lowerBound = ascent();
	double excess; //some fraction of lowerBound
	long maxAlpha = excess * fabs(lowerBound);	
	generateCandidates( maxCandidates, maxAlpha );
}

void ascent(){
	node *n;
	double bestW, w;
	int Period = InitialPeriod, P, InitialPhase = 1;

	w = min1TreeCost();
	if (Norm == 0) return W;
	generateCandidates(AscentCandidates, LONG_MAX);
	bestW = w;

	forAllNodes(n) {
		n->LastV = n->V;
		n->BestPi = n->Pi;
	}
 
	for ( T = InitialStepSize; T > 0; Period /= 2, T /= 2) {
 
		for (P = 1; T > 0 && P <= Period; P++) {
			
			forAllNodes(n) {
				if ( n->V != 0){
					n->Pi += T*(7*n->V + 3*n->LastV)/10;
				}	
				n->LastV = n->V;
			}
			
			w = Minimum1TreeCost();
			if ( Norm == 0 ){ return w; }
 
			if ( w > bestW ) {
				bestW = w;
				
				forAllNodes(n)
					n->BestPi = n->Pi;
 
				if (InitialPhase) T *= 2;
				if (P == Period) Period *= 2;
			}
			else if (InitialPhase && P > InitalPeriod/2) {
				InitialPhase = 0;
				P = 0;
				T = 3*T/4;
			}
		}
	}
	forAllNodes(n)
	n->Pi = n->BestPi;
	return Minimum1TreeCost();	
}

void generateCandidates( int maxCandidates, int maxAlpha){
	Node *From, *To;
	long Alpha;
	Candidate *Edge;
 
	forAllNodes(From){
		From->Mark = 0;
	}
 
	forAllNodes(From) {
		if (From != FirstNode) {
			From->Beta = LONG_MIN;
	
			for (To = From; To->Dad != 0; To = To->Dad) {
				To->Dad->Beta = max(To->Beta, To->Cost);
				To->Dad->Mark = From;
			}
		}
 
		forAllNodes(To, To != From) {
			if (From == FirstNode){
				Alpha = To == From->Father ? 0 :
				C(From,To) - From->NextCost;
			} else if (To == FirstNode){
				Alpha = From == To->Father ? 0:
				C(From,To) - To->NextCost;
			} else {
				if (To->Mark != From){
					To->Beta = max(To->Dad->Beta, To->Cost);
				}
				Alpha = C(From,To) - To->Beta;
			}
			
			if (Alpha <= MaxAlpha){
				InsertCandidate(To, From->CandidateSet);
			}
		}
	}

	if (SymmetricCandidates){
		ForAllNodes(From){
			ForAllCandidates(To, From->CandidateSet){
				if (!IsMember(From, To->CandidateSet)){
					InsertCandidate(From, To->CandidateSet);
				}
			}
		}
	}
}

double FindTour() {
	int Trial;
	double BetterCost = DBL_MAX, Cost;

	for (Trial = 1; Trial <= Trials; Trial++) {
		ChooseInitialTour();
		Cost = LinKernighan();
		
		if (Cost < BetterCost) {
			RecordBetterTour();
			BetterCost = Cost;
			AdjustCandidateSet();
		}
	}
 
	ResetCandidateSet();
	return BetterCost;

}

double LinKernighan() {

 Node *t1, *t2;
 int X2, Failures;
 long G, Gain;
 double Cost = 0;
 
 ForAllNodes(t1){
	Cost += C(t1,SUC(t1));
 }
 
 do {
	Failures = 0;
	ForallNodes(t1, Failures < Dimension) {
		for (X2 = 1; X2 <= 2; X2++) {
			t2 = X2 == 1 ? PRED(t1) : SUC(t1);
			if (InBetterTour(t1,t2))
				continue;
			G = C(t1,t2);
			while (t2 = BestMove(t1, t2, &G, &Gain)) {
				if (Gain > 0) {
					Cost -= Gain;
					StoreTour();
					Failures = 0;
					goto Next_t1;
				}
			}
			Failures++;
			RestoreTour();
		}
		Next_t1: ;
	}
	if ((Gain = Gain23()) > 0) {
		Cost -= Gain;
		StoreTour();
	}
 } while (Gain > 0);
}

node *BestMove( node *t1, node *t2, long *G0, long *Gain ){

	node *t3, *t4, *t5, *t6, *t7, *t8, *t9, *t10;
	Node *T3, *T4, *T5, *T6, *T7, *T8, *T9, *T10 = 0;
	long G1, G2, G3, G4, G5, G6, G7, G8, BestG8 = LONG_MIN;
	int X4, X6, X8, X10;
 
	*Gain = 0; Reversed = SUC(t1) != t2;
	ForAllCandidates(t3, t2->CandidateSet) {
		if (t3 == PRED(t2) || t3 == SUC(t2) || (G1 = *G0 - C(t2,t3)) <= 0)
			continue;

		for (X4 = 1; X4 <= 2; X4++) {
			t4 = X4 == 1 ? PRED(t3) : SUC(t3);
			G2 = G1 + C(t3,t4);
			if (X4 == 1 && (*Gain = G2 - C(t4,t1)) > 0) {
				Make2OptMove(t1, t2, t3, t4);
				return t4;
			}
 
			ForAllCandidates(t5, t4->CandidateSet) {
				if (t5 == PRED(t4) || t5 == SUC(t4) || (G3 = G2 - C(t4,t5)) <= 0)
					continue;
				
				for (X6 = 1; X6 <= 2; X6++) {
					Determine (T3,T4,T5,T6,T7,T8,T9,T10) = (t3,t4,t5,t6,t7,t8,t9,t10)
						such that
					G8 = *G0 - C(t2,T3) + C(T3,T4)
						- C(T4,T5) + C(T5,T6)
						- C(T6,T7) + C(T7,T8)
						- C(T8,T9) + C(T9,T10)
					is maximum (= BestG8), and (T9,T10) has not previously been included; if during
					this process a legal move with *Gain > 0 is found, then make the move and exit
					from BestMove immediately;
				}
			}
		}
	}
 
	*Gain = 0;
	if (T10 == 0) return 0;
	Make5OptMove(t1,t2,T3,T4,T5,T6,T7,T8,T9,T10);
	*G0 = BestG8;
	return T10;

}
