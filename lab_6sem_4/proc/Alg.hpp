#ifndef ALG_T
#define ALG_T
#include <vector>
#include <random>
#include <cinttypes>
#include <iostream>
#include <thread>
using namespace std;

vector<vector<double>> FuzzyCMeans (vector<vector<double>> X, int c, int m, double eps);
vector<vector<double>> FuzzyCMeansParallel(vector<vector<double>> X, int c, int m, double eps, int threads);

#endif