#include <iostream>
#include <vector>

using namespace std;

#define max 2147483647

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


int main()
{
    int numVertices = 5;
    vector<vector<int>> graph{{0, 2, 0, 6, 0}, 
                              {2, 0, 3, 8, 5}, 
                              {0, 3, 0, 0, 7}, 
                              {6, 8, 0, 0, 9}, 
                              {0, 5, 7, 9, 0}}; 

    vector<int> mst(numVertices);
    vector<bool> inMst(numVertices);

    vector<int> dist(numVertices);

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

}

