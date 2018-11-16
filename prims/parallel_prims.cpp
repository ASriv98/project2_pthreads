#include "include/parallel_prims.h"


int main()
{
    vector<vector<int>> graph{{0, 2, 0, 6, 0}, 
                              {2, 0, 3, 8, 5}, 
                              {0, 3, 0, 0, 7}, 
                              {6, 8, 0, 0, 9}, 
                              {0, 5, 7, 9, 0}}; 
    printGraph(graph);

    //printGraph(init_AdjMatrix(8));
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
    cout << endl;


    printf("Edge \tWeight\n"); 
    for (int i = 0; i < graph.size(); i++) {
        printf("%d - %d \t%d \n", mst[i], i, graph[i][mst[i]]); 
    }
    cout << endl;


    // Start Timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    int numThreads = 2;
    mst = parallel_primsMST(graph, numThreads);

    // Stop Timer
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time: %f\n", elapsed);


    printf("Edge \tWeight\n"); 
    for (int i = 0; i < graph.size(); i++) {
        printf("%d - %d \t%d \n", mst[i], i, graph[i][mst[i]]); 
    }

}

vector<int> primsMST(vector<vector<int>> graph){
    int numVertices = graph.size();
    vector<int> mst(numVertices);
    vector<bool> inMst(numVertices);

    vector<int> dist(numVertices);

    //Initalize all edges as infinite and not included yet
    for (int i = 0; i < numVertices; i++) {
        dist[i] = max;
        inMst[i] = false;
    }
    dist[0] = 0;

    // Sequential Execution
    for (int currVertex = 0; currVertex < numVertices; ++currVertex) {
        // Find smallest edge
        int u;
        int minEdgeDist = max;
        for(int i = 0; i < dist.size(); i++){
            if (inMst[i] == false && minEdgeDist > dist[i]) {
                u = i;
                minEdgeDist = dist[i];
            }
        }
        // Put vertex into mst
        inMst[u] = true;
        cout << "Found: " << u << endl;


        for (int v = 0; v < numVertices; v++) {
            int edge = graph[u][v];
            if (edge != 0 && edge < dist[v]) {
                cout << "uv: " << u << v << endl;

                mst[v] = u;
                dist[v] = edge;
            }
        }            
        printf("Edge \tWeight\n"); 
        for (int i = 1; i < graph.size(); i++) {
            printf("%d - %d \t%d \n", mst[i], i, graph[i][mst[i]]); 
        }
    }
    return mst;
}

vector<int> parallel_primsMST(vector<vector<int>> &graph, int numThreads) {
  /* Initalize values */

  int numVertices = graph.size();
  vector<int> mst(numVertices);
  vector<bool> inMst(numVertices);
  vector<int> dist(numVertices);
  pthread_t thread[numThreads];
  int uMinThreads[numThreads];

  // Create barrier for sync. on smallest edge
  pthread_barrier_t smallestEdgeBarrier;
  pthread_barrier_init(&smallestEdgeBarrier, NULL, numThreads + 1);

  // Create barrier for updating edges after sync
  pthread_barrier_t updateEdgeBarrier;
  pthread_barrier_init(&updateEdgeBarrier, NULL, numThreads + 1);

  /* Initalize all edges */
  for (int i = 0; i < numVertices; i++) {
      dist[i] = max;
      inMst[i] = false;
  }

  dist[0] = 0;
  /* Spawn Pthreads */

  for (int i = 0; i < numThreads; i++){

    // Initalize all pthread variables
    struct params* args = new struct params;
    args->graph = &graph;
    args->dist = &dist;
    args->inMst = &inMst;
    args->currThread = i;
    args->uMinThreads = uMinThreads;
    args->numThreads = numThreads;
    args->smallestEdgeBarrier = &smallestEdgeBarrier;
    args->updateEdgeBarrier = &updateEdgeBarrier;


    pthread_create( &thread[i], NULL, parallel_primsSolve, (void*) args);
  }


  /* Start Parallel Prims */

  // Loop for number of vertices
  for (int currVertex = 0; currVertex < numVertices; ++currVertex) {

    // Wait for all threads to find their local min
    pthread_barrier_wait(&smallestEdgeBarrier);

    /* Find minimum length vertex from all threads */
    int u;
    for (int j = 0; j < numThreads; j++) {
      cout << uMinThreads[j] << endl;
      if (inMst[uMinThreads[j]] == false) {
        u = uMinThreads[j];
      }
    }
    cout << "V not in mst: " << u << endl;
    for( int i = 0; i < numThreads; i++) {
      // Check if the current stored distance is larger than other threads
      if (inMst[i] == false && dist[u] > dist[uMinThreads[i]]) {
        u = i;
      }
    }

    cout << "Main thread chose: " << u << endl;
    // Put smallest vertex into mst
    inMst[u] = true;

    /* Update all of the new edges */
    for (int v = 0; v < numVertices; v++) {
      int edge = graph[u][v];
      if (edge != 0 && edge < dist[v]) {
        cout << "uv: " << u << v << endl;
        mst[v] = u;
        dist[v] = edge;
      }
    }

    // All threads are blocked until the edge values are updated
    pthread_barrier_wait(&updateEdgeBarrier);
  }


  for (int i = 0; i < numThreads; i++){
    pthread_join( thread[i], NULL);
  }

  return mst;
}

void* parallel_primsSolve(void* args) {
  struct params *params = (struct params*) args;

  /* Get all parameters */
  vector<vector<int>> *graph = params->graph;
  vector <int>* dist = params->dist;
  vector <bool>* inMst = params->inMst;
  int currThread = params->currThread;
  int numThreads = params->numThreads;
  int* uMinThreads = params->uMinThreads;
  pthread_barrier_t* smallestEdgeBarrier = params->smallestEdgeBarrier;
  pthread_barrier_t* updateEdgeBarrier = params->updateEdgeBarrier;

  int numVertices = (*graph).size();

  int start = currThread * (numVertices / numThreads);

  int end = start + (numVertices / numThreads);

  /* Loop for number of vertices on all threads */
  for (int currVertex = 0; currVertex < numVertices; ++currVertex) {

    /* Find smallest local edge */
    int u = start;
    int minEdgeDist = max;
    for(int i = start; i <= end; i++){
      if ((*inMst)[i] == false && minEdgeDist > (*dist)[i]) {
        u = i;
        minEdgeDist = (*dist)[i];
      }
    }
    //cout << "Thread: " << currThread << "\t found Vertex: " << u << endl;
    uMinThreads[currThread] = u;

    // Signal that local edge was found
    pthread_barrier_wait(smallestEdgeBarrier);

    // Wait for edges to be updated
    pthread_barrier_wait(updateEdgeBarrier);
  }
}

void printGraph(vector<vector<int>> const &input) {
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            cout << input[i][j] << ' ';
        }
        cout << endl;
    }
}

vector<vector<int>> init_AdjMatrix(int numVertices) {
    vector<vector<int>> graph(numVertices, vector<int>(numVertices));

    for (int i = 0; i < numVertices; i++){
        // Traversing through upper triangular
        for (int j = i; j < numVertices; j++) {
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
    return graph;
}