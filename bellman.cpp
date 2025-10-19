#include "bellman.h"

void BF(int edgeList[][3], int numEdges, char start_vertices, int BFValue[], int BFPrev[])
{
    const int INF = 1e9;

    //Vector contains all unique vertices
    vector<char> vertices;
    for (int i = 0; i < numEdges; ++i) {
        vertices.push_back((char)edgeList[i][0]);
        vertices.push_back((char)edgeList[i][1]);
    }

    sort(vertices.begin(), vertices.end());
    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());
  
    int V = vertices.size();

    //Initialize the mapping from letter to index and vice versa
    int vertexIndex[128];
    char indexVertex[V];

    for (int i = 0; i < 128; ++i) {
        vertexIndex[i] = -1; 
    }

    for (int i = 0; i < V; ++i) {
        vertexIndex[(int)vertices[i]] = i;
        indexVertex[i] = vertices[i];
    }

    int startIndex = vertexIndex[(int)start_vertices];

    vector<vector<int>> weightMatrix(V, vector<int>(V, INF));
    for (int i = 0; i < V; ++i) {
        weightMatrix[i][i] = 0; 
    }
    
    for(int i = 0; i < numEdges; ++i)
    {
        int u = vertexIndex[(int)edgeList[i][0]];
        int v = vertexIndex[(int)edgeList[i][1]];
        int weight = edgeList[i][2];

        weightMatrix[u][v] = weight; 
    }
    
    //Initialize BFValue and BFPrev
    for (int i = 0; i < V; ++i) {
        if(BFValue[i] == -1)
        {
            BFValue[i] = (i == startIndex) ? 0 : INF; 
        }
    }

    //Create temporary arrays to store current values and previous vertices
    vector<int>currVal(V);
    vector<int>currPrev(V);
    for (int i = 0; i < V; ++i) {
        currVal[i] = BFValue[i];
        currPrev[i] = BFPrev[i];
    }
    
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if(weightMatrix[u][v] == INF)
                continue;
            if (BFValue[u] !=INF && BFValue[u] + weightMatrix[u][v] < currVal[v]) {
                currVal[v] = BFValue[u] + weightMatrix[u][v];
                currPrev[v] = u; 
                
            }
        }
    }
    
    // Update BFValue và BFPrev
    for (int i = 0; i < V; ++i) {
        if (currVal[i] < BFValue[i] ) {
            BFValue[i] = currVal[i];
            BFPrev[i] = currPrev[i];
        }
        
        // Change the value to -1 if it is still INF
        if (BFValue[i] == INF) {
            BFValue[i] = -1;
        }
    }
}      

string BF_Path(int edgeList[][3], int numEdges, char start_vertices, char end_vertices)
{
    vector<char> vertices;

    // Collect unique vertices
    for(int i = 0; i < numEdges; ++i) {
        vertices.push_back((char)edgeList[i][0]);
        vertices.push_back((char)edgeList[i][1]);
    }

    sort(vertices.begin(), vertices.end());
    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());

    int V = vertices.size();

    // Create a mapping from characters to indices and vice versa
    int vertexIndex[128];
    char indexToVertex[V];

    for(int i = 0; i < 128; ++i) {
        vertexIndex[i] = -1;
    }

    // Fill the mapping
    for(int i = 0; i < V; ++i) {
        vertexIndex[(int)vertices[i]] = i;
        indexToVertex[i] = vertices[i];
    }

    int destination = vertexIndex[(int)end_vertices];

    int BFValue[128];
    int BFPrev[128];

    // Initialize arrays
    for(int i = 0; i < V; ++i) {
        BFValue[i] = -1;  // -1 as initially expected by BF
        BFPrev[i] = -1;
    }
    
    for(int i = 0; i < V; ++i)
        BF(edgeList, numEdges, start_vertices, BFValue, BFPrev);

    // Check if the goal is reachable
    if(BFValue[destination] == -1) {
        return "No path found";
    }

    // Reconstruct the path
    vector<char> path_vertices;
    int current = destination;
    
    while(current != -1) {
        path_vertices.push_back(indexToVertex[current]);
        current = BFPrev[current];
    }
    
    reverse(path_vertices.begin(), path_vertices.end());
    
    string path = "";
    for(char v : path_vertices) {
        path += v;
        path += ' ';
    }
    
    return path;
}
