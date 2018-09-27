#include <iostream>
#include <cstdio>
#include <stdlib.h> // for exit();
#include <getopt.h> // to parse long arguments.
#include <unistd.h> // sleep
#include <vector>
#include <string>
#include <fstream>


using namespace std;

static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Text-based version of Conway's game of life.\n\n"
"   --seed,-s     FILE     read start state from FILE.\n"
"   --world,-w    FILE     store current world in FILE.\n"
"   --fast-fw,-f  NUM      evolve system for NUM generations and quit.\n"
"   --help,-h              show this message and exit.\n";

size_t max_gen = 0; /* if > 0, fast forward to this generation. */
string wfilename =  "tmp/gol-world-current"; /* write state here */
string initfilename = "tmp/gol-world-current"; /* read initial state from here. */

vector<vector<bool> > initialStateVec;
vector<vector<bool> > nextStateVec;

/* NOTE: you don't have to write these functions -- this is just how
 * I chose to organize my code. */
size_t nbrCount(size_t i, size_t j, const vector<vector<bool> >& g);
void update();
int initFromFile(const string& fname); /* read initial state into vectors. */
void mainLoop();
void writeToFile(const string& fname);
void duplicateState(const vector<vector<bool> >& g);

char text[3] = ".O";

int main(int argc, char *argv[]) {
	// define long options
	static struct option long_opts[] = {
		{"seed",    required_argument, 0, 's'},
		{"world",   required_argument, 0, 'w'},
		{"fast-fw", required_argument, 0, 'f'},
		{"help",    no_argument,       0, 'h'},
		{0,0,0,0}
	};
	// process options:
	char c;
	int opt_index = 0;
	while ((c = getopt_long(argc, argv, "hs:w:f:", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 'h':
				printf(usage,argv[0]);
				return 0;
			case 's':
				initfilename = optarg;
				break;
			case 'w':
				wfilename = optarg;
				break;
			case 'f':
				max_gen = atoi(optarg);
				break;
			case '?':
				printf(usage,argv[0]);
				return 1;
		}
	}

	initFromFile(initfilename);
	mainLoop();
	return 0;
}

void mainLoop() {
	/* TODO: write this */
	/* update, write, sleep */

	if (max_gen == 0) {
		while(true){
			update();
			if(wfilename == "-"){
				duplicateState(nextStateVec);
			}else{
				writeToFile(wfilename);
			}
			sleep(1);
		}
	} else {
		for(int i = 0; i<max_gen; i++){
			update();
		}
		if(wfilename == "-"){
			duplicateState(nextStateVec);
		}else{
			writeToFile(wfilename);
		}
	}
}

size_t nbrCount(size_t i, size_t j, const vector<vector<bool> >& g){
	int count = 0;
	for(int r = -1; r<=1; r++){
		for(int s = -1; s<=1; s++){
			int index_i = (i + r + g.size())%g.size();
			int index_j = (j + s + g[0].size())%g[0].size();
			if(g[index_i][index_j] && !(r == 0 && s == 0)){
				count++;
			}
		}
	}
	return count;
}

void update(){
	initialStateVec = nextStateVec;
	for(int i = 0; i<initialStateVec.size(); i++){
		for(int j = 0; j<initialStateVec[0].size(); j++){
			if(nbrCount(i, j, initialStateVec) < 2 || nbrCount(i, j, initialStateVec)>3){
				nextStateVec[i][j]=false;
			}else if(nbrCount(i, j, initialStateVec) == 3){
				nextStateVec[i][j]=true;
			}else {
				nextStateVec[i][j] = initialStateVec[i][j];
			}
		}
	}
}

int initFromFile(const string& fname){
	ifstream file(fname);
	string row;

	if(file.is_open()){
		while(getline(file, row)){
			vector<bool> line;
			for(int i = 0; i<row.size(); i++){
				if(row[i]=='.'){
					line.push_back(false);
				}else{
					line.push_back(true);
				}
			}
			nextStateVec.push_back(line);
			line.clear();
		}
		file.close();
		return 0;
	}
	cout<<"file not found";
	return -1;
}

void writeToFile(const string& fname){
	ofstream file(fname);
	for(int i = 0; i<nextStateVec.size(); i++){
		for(int j = 0; j<nextStateVec[0].size(); j++){
			if(nextStateVec[i][j]){
				file<<"O";
			}else{
				file<<".";
			}
		}
		file<<endl;
	}
	file.close();
}

void duplicateState(const vector<vector<bool> >& g){
	for(int i = 0; i<g.size(); i++){
		for(int j = 0; j<g[0].size(); j++){
			if(g[i][j]){
				cout<<"O";
			}else{
				cout<<".";
			}
		}
		cout<<endl;
	}
}
