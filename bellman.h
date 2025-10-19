#ifndef _H_BELLMAN_H_
#define _H_BELLMAN_H_
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
string BF_Path(int edgeList[][3], int edgeCount, char start, char goal);
void BF(int edgeList[][3], int numEdges, char start_vertices, int BFValue[], int BFPrev[]);
#endif // _H_BELLMAN_H_
