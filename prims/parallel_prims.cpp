#include "include/parallel_prims.h"


int main()
{
    vector<vector<int>> graph{{0, 2, 0, 6, 0}, 
                              {2, 0, 3, 8, 5}, 
                              {0, 3, 0, 0, 7}, 
                              {6, 8, 0, 0, 9}, 
                              {0, 5, 7, 9, 0}}; 

    // Start Timer
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Call Prims
    vector<int> mst = primsMST(graph);

    // Stop Timer
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time: %f\n", elapsed);

    printf("Edge \tWeight\n"); 
    for (int i = 1; i < graph.size(); i++) {
        printf("%d - %d \t%d \n", mst[i], i, graph[i][mst[i]]); 
    }

}

vector<int> primsMST(vector<vector<int>> graph){
    int numVertices = graph.size();
    vector<int> mst(numVertices);
    vector<bool> inMst(numVertices);

    vector<int> dist(numVertices);

    //Initalize all edges as infinite and not included yet
    for (int i = 0; i < numVertices; ++i) {
        dist[i] = max;
        inMst[i] = false;
    }
    dist[0] = 0;

    // Sequential Execution
    for (int currVertex = 0; currVertex < numVertices; ++currVertex) {
        // Find smallest edge
        int u;
        int minEdgeDist = max;
        for(int i = 0; i < dist.size(); ++i){
            if (inMst[i] == false && minEdgeDist > dist[i]) {
                u = i;
                minEdgeDist = dist[i];
            }
        }
        // Put vertex into mst
        inMst[u] = true;

        // Parallel Section
        for (int v = 0; v < numVertices; ++v) {
            int edge = graph[u][v];
            if (inMst[v] == false && edge != 0 && edge < dist[v]) {
                mst[v] = u;
                dist[v] = edge;
            }
        }
    }
    return mst;
}