//
//  main.cpp
//  lab_04
//
//  Created by macbook on 12.11.2022.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "Alg.hpp"
using namespace std;


void PrintMatrix(vector<vector<double>> v)
{
    for(int i = 0; i < (int)v.size(); i++)
    {
        for(int j = 0; j < (int)v[0].size(); j++)
            cout << v[i][j] << " ";
        cout << endl;
    }
}

vector<vector<double>> GenData(int n, int m)
{
    vector<vector<double>> res(n);
    for(int i = 0; i < n ; i++)
        for(int j = 0; j < m ; j++)
            res[i].push_back(0.0 + rand()/((RAND_MAX)/10.0));
    return res;
}



int main(void) {
    
    printf("PID:%d", getpid());
    bool k = true;
    while (k)
    {
        vector<vector<double>> data = GenData(20,20);
        vector<vector<double>> res = FuzzyCMeans(data, 2, 2, 0.00001);
    }
    return 0;
}
