#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

#define max 2147483647
#define numVertices 5

int graph[numVertices][numVertices];	// Adjacency Matrix
vector<int> mst(numVertices);			// Stores final MST 
vector<bool> inMst(numVertices);		// Keeps track of vertices in MST while iterating

vector<int> dist(numVertices);

int minEdge(vector<int> dist ,vector<bool> inMst){
    int min;
    int minEdgeDist = max;

    for(int i = 0; i < dist.size(); ++i){
        if (inMst[i] == false && minEdgeDist > dist[i]) {
            min = i;
            minEdgeDist = dist[i];
        }
    }
    return min;
}

void init_AdjMatrix()
{
	for (int i = 0; i < numVertex; i++){
		for (int j = i; j < numVertex; j++) {		// Traversing through upper triangular
			//if node is self referencing no distance
			if (i = j)
				graph[i][j] = 0;
			else {
				// Weights between 1-9 ensures fully connected.
				graph[i][j] = rand() % 9 + 1; 
				graph[j][i] = rand() % 9 + 1;
			}
		}
	}
}

void sequenPrimMST() {
	    //Initalize all edges as infinite and not included yet
    for (int i = 0; i < numVertices; ++i) {
        dist[i] = max;
        inMst[i] = false;
    }

    dist[0] = 0;

    for (int count = 0; count < numVertices; ++count) {
        int u = minEdge(dist, inMst);
        inMst[u] = true;

        for (int v = 0; v < numVertices; ++v) {
            int edge = graph[u][v];
            if (inMst[v] == false && edge != 0 && edge < dist[v]) {
                mst[v] = u;
                dist[v] = edge;
            }
        }
    }

    printf("Edge \tWeight\n"); 
    for (int i = 1; i < numVertices; i++) {
        printf("%d - %d \t%d \n", mst[i], i, graph[i][mst[i]]); 
}

int main()
{

    // vector<vector<int>> graph{{0, 2, 0, 6, 0}, 
    //                           {2, 0, 3, 8, 5}, 
    //                           {0, 3, 0, 0, 7}, 
    //                           {6, 8, 0, 0, 9}, 
    //                           {0, 5, 7, 9, 0}}; 


    init_AdjMatrix();					// Initializes Adjacency matrix

    // Start timing for sequential
  	clock_t start = clock(), diff;
    sequenPrimMST();
    diff = clock() - start;

    int milli_sec = diff * 1000/ CLOCKS_PER_SEC;

    printf("Time taken: %d seconds %d milliseconds \n", milli_sec/1000, milli_sec%1000);



   	return 0;

}

