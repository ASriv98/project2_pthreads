#ifndef parallel_prims_h
#define parallel_prims_h

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <functional>
#include <string>
#include <vector>

#include <math.h>
#include <signal.h>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define max 2147483647

vector<int> primsMST(vector<vector<int>> graph);

vector<vector<int>> init_AdjMatrix(int numVertices);

#endif