#include "tsm.h"
const int INF = 1e9 + 7;
int numV = 0;
int idx[128];
char revIdx[25];
int adj[25][25];

const int POP_SIZE = 100;
const int GENERATIONS = 500;
const double MUTATION_RATE = 0.1;

vector<vector<int>> population;
vector<int> fitness;
vector<char> vertices;

string dynamic(int adj[25][25], int V, char start)
{
    int full = 1 << V;
    vector<vector<int>> dp(full, vector<int>(25, 1e7));
    vector<vector<int>> parent(full, vector<int>(25, -1));

    int startIdx = idx[start];

    dp[1 << startIdx][startIdx] = 0;
    for(int mask = 1; mask < full; ++mask) {
        if(!(mask & (1 << startIdx))) continue; // Ensure the start vertex is "turning on" in the mask
        for (int u = 0; u < V; ++u) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == 1e7) continue; // Skip if this state is unreachable
            for (int v = 0; v < V; ++v) {
                if (mask & (1 << v)) continue;
                if (adj[u][v] == 1e7) continue; // Skip if there's no edge
                int nextMask = mask | (1 << v);
                int tmp = dp[mask][u] + adj[u][v];
                if (tmp < dp[nextMask][v]) {
                    dp[nextMask][v] = tmp;
                    parent[nextMask][v] = u; // Store the parent for path reconstruction
                }
            }
        }
    }
    int last = -1, minCost = 1e7;
    for (int i = 0; i < V; ++i) {
        int cost = dp[full - 1][i] + adj[i][startIdx];
        if (cost < minCost) {
            minCost = cost;
            last = i;
        }
    }

    if (last == -1) return "";
    int path[25 + 1];
    int n = V;
    int mask = full - 1;
    int now = last;

    path[n] = startIdx;
    for (int i = n - 1; i >= 0; --i)
    {
        path[i] = now;
        now = parent[mask][now];
        mask ^= (1 << path[i]);
    }
    path[0] = startIdx;

    string res = "";
    for (int i = 0; i <= n; ++i) {
        res += revIdx[path[i]];
        if (i < n) res += " ";
    }
    return res;

}

vector<int> generate_individual() {
    vector<int> ind(numV);
    for (int i = 0; i < numV; ++i) ind[i] = i;
    random_device rd;
    mt19937 g(rd());
    shuffle(ind.begin(), ind.end(), g);
    return ind;
}

vector<int> nearest_neighbor_individual(int startIdx) {
    vector<bool> visited(numV, false);
    vector<int> path;
    int curr = startIdx;
    path.push_back(curr);
    visited[curr] = true;
    for (int i = 1; i < numV; ++i) {
        int next = -1, minCost = INF;
        for (int j = 0; j < numV; ++j) {
            if (!visited[j] && adj[curr][j] < minCost) {
                minCost = adj[curr][j];
                next = j;
            }
        }
        if (next == -1) break;
        visited[next] = true;
        path.push_back(next);
        curr = next;
    }
    return path;
}

void initialize_population() {
    population.clear();
    population.push_back(nearest_neighbor_individual(0));
    for (int i = 1; i < POP_SIZE; ++i)
        population.push_back(generate_individual());
}

int calculate_fitness(const vector<int>& ind) {
    int cost = 0;
    for (int i = 0; i < numV - 1; ++i)
        cost += adj[ind[i]][ind[i + 1]];
    cost += adj[ind[numV - 1]][ind[0]];
    return cost;
}

void update_fitness() {
    fitness.clear();
    for (const auto& ind : population)
        fitness.push_back(calculate_fitness(ind));
}

vector<int> tournament_selection() {
    int a = rand() % POP_SIZE, b = rand() % POP_SIZE;
    return (fitness[a] < fitness[b]) ? population[a] : population[b];
}

pair<vector<int>, vector<int>> crossover(const vector<int>& p1, const vector<int>& p2) {
    int l = rand() % numV, r = rand() % numV;
    if (l > r) swap(l, r);
    vector<int> c1(numV, -1), c2(numV, -1);
    for (int i = l; i <= r; ++i) {
        c1[i] = p1[i];
        c2[i] = p2[i];
    }
    int i1 = (r + 1) % numV, i2 = (r + 1) % numV;
    for (int i = 0; i < numV; ++i) {
        int v = p2[(r + 1 + i) % numV];
        if (find(c1.begin(), c1.end(), v) == c1.end()) c1[i1++ % numV] = v;
        v = p1[(r + 1 + i) % numV];
        if (find(c2.begin(), c2.end(), v) == c2.end()) c2[i2++ % numV] = v;
    }
    return {c1, c2};
}

void mutate(vector<int>& ind) {
    for (int i = 0; i < numV; ++i) {
        if ((rand() % 1000) / 1000.0 < MUTATION_RATE) {
            int j = rand() % numV;
            if (i != j) swap(ind[i], ind[j]);
        }
    }
}

void evolve() {
    vector<vector<int>> new_pop;
    int best_idx = min_element(fitness.begin(), fitness.end()) - fitness.begin();
    new_pop.push_back(population[best_idx]);
    while ((int)new_pop.size() < POP_SIZE) {
        auto p1 = tournament_selection();
        auto p2 = tournament_selection();
        
        pair<vector<int>, vector<int>> children = crossover(p1, p2);
        vector<int> c1 = children.first;
        vector<int> c2 = children.second;

        mutate(c1);
        mutate(c2);
        new_pop.push_back(c1);
        if ((int)new_pop.size() < POP_SIZE) new_pop.push_back(c2);
    }
    population = new_pop;
}

string path_to_string(const vector<int>& path) {
    string res = "";
    for (int i = 0; i < (int)path.size(); ++i) {
        res += revIdx[path[i]];
        if (i < (int)path.size() - 1) res += " ";
    }
    res += " ";
    res += revIdx[path[0]];
    return res;
}


string Traveling(int edgeList[][3], int numEdges, char start)
{
    for (int i = 0; i < numEdges; ++i) {
        vertices.push_back((char)edgeList[i][0]);
        vertices.push_back((char)edgeList[i][1]);
    }
    sort(vertices.begin(), vertices.end());
    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());
    int V = vertices.size();
    numV = V;

    for (int i = 0; i < V; ++i) {
        idx[(int)vertices[i]] = i;
        revIdx[i] = vertices[i];
    }

    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            adj[i][j] = 1e7;
        
    for (int i = 0; i < numEdges; ++i) {
        int u = idx[edgeList[i][0]];
        int v = idx[edgeList[i][1]];
        adj[u][v] = edgeList[i][2];
    }
    string res = "";
        if(V <= 20)
            res = dynamic(adj, V, start);
        else
        {
            initialize_population();
            update_fitness();
            for (int gen = 0; gen < GENERATIONS; ++gen) {
                evolve();
                update_fitness();
            }
            int best_idx = min_element(fitness.begin(), fitness.end()) - fitness.begin();
            res = path_to_string(population[best_idx]);
        }
    
    return res;
}

