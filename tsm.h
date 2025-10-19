
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;



string dynamic(int adj[25][25], int n, char start);
vector<int> generate_individual();
vector<int> nearest_neighbor_individual(int startIdx);
void initialize_population();
int calculate_fitness(const vector<int>& ind);
void update_fitness();
vector<int> tournament_selection();
pair<vector<int>, vector<int>> crossover(const vector<int>& p1, const vector<int>& p2);
void mutate(vector<int>& ind);
void evolve();
string path_to_string(const vector<int>& path);
string Traveling(int edgeList[][3], int numEdges, char start);


