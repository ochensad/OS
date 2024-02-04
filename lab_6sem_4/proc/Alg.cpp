//
//  Alg.cpp
//  lab_04
//
//  Created by macbook on 12.11.2022.
//

#include <vector>
#include <random>
#include <cinttypes>
#include <iostream>
#include <thread>
using namespace std;

vector<vector<double>> generateRandFill(int n, int c)
{
    vector<vector<double>> res(n);
    double tmp = 0.0 + rand()/((RAND_MAX)/1.0);
    for (int i = 0; i < n; i++)
    {
        tmp = 0.0 + rand()/((RAND_MAX)/1.0);
        double tmp_sum = 0.0;
        for(int j = 0; j < c - 1; j++)
        {
            res[i].push_back(tmp);
            tmp_sum += tmp;
            if (j < c - 2)
                tmp = 0.0 + std::rand()/((RAND_MAX)/tmp);
            else
            {
                tmp = 1.0 - tmp_sum;
                res[i].push_back(tmp);
            }
        }
    }
    double sum_i = 0.0;
    bool k = true;
    int k_counter = 0;
    while(k)
    {
        k_counter = 0;
        for(int j = 0; j < c; j++)
        {
            sum_i = 0.0;
            for(int i = 0; i < n; i++)
            {
                sum_i += res[i][j];
            }
            if (sum_i < 0 || sum_i > n)
            {
                break;
            }
            else
                k_counter++;
        }
        if (k_counter == c)
            k = false;
        else
        {
            for (int i = 0; i < n; i++)
            {
                tmp = 0.0 + rand()/((RAND_MAX)/1.0);
                double tmp_sum = 0.0;
                for(int j = 0; j < c - 1; j++)
                {
                    res[i].push_back(tmp);
                    tmp_sum += tmp;
                    if (j < c - 2)
                        tmp = 0.0 + std::rand()/((RAND_MAX)/tmp);
                    else
                    {
                        tmp = 1.0 - tmp_sum;
                        res[i].push_back(tmp);
                    }
                }
            }
        }
    }
    return res;
}

vector<vector<double>> generateZeroFill(int c, int r)
{
    vector<vector<double>> res(c);
    for(int i = 0; i < c; i++)
    {
        for(int j = 0; j < r; j++)
            res[i].push_back(0.0);
    }
    return res;
}

vector<double> MulNumVec(double num, vector<double> xj)
{
    vector<double> res(xj.size());
    for(int i = 0; i < (int)xj.size(); i++)
        res[i] = num * xj[i];
    return res;
}

vector<double> SumVecVec(vector<double> a, vector<double> b)
{
    vector<double> res(a.size());
    for(int i = 0; i < (int)a.size(); i++)
        res[i] = a[i] + b[i];
    return res;
}

vector<double> DevVecNum(vector<double> v, double n)
{
    vector<double> res(v.size());
    for(int i = 0; i < (int)v.size(); i++)
        res[i] = v[i] / n;
    return res;
}

double distance(vector<double> a, vector<double> b)
{
    vector<double> res(a.size());
    for(int i = 0 ; i < (int)a.size(); i++)
        res[i] = a[i] - b[i];
    double max_diff = res[0];
    for(int i = 0; i < (int)a.size(); i++)
        max_diff = max(res[i], max_diff);
    return max_diff;
}

void KPrintMatrix(vector<vector<double>> v)
{
    for(int i = 0; i < (int)v.size(); i++)
    {
        for(int j = 0; j < (int)v[0].size(); j++)
            cout << v[i][j] << " ";
        cout << endl;
    }
}


vector<vector<double>> FuzzyCMeans (vector<vector<double>> X, int c, int m, double eps)
{
    int n = X.size();
    int r = X[0].size();
    vector<vector<double>> centers = generateZeroFill(c, r);
    vector<vector<double>> Wprev = generateRandFill(n, c); //membership
    vector<vector<double>> W = Wprev;
    while (true){
        // обновляем центры кластеров
        for (int i=0; i<c; i++)
        {
            vector<double> num(X[0].size());
            double den = 0.0;
            for (int j=0; j<n; j++)
            {
                num = SumVecVec(num, MulNumVec(pow(W[j][i], m),X[j]));
                den += pow(W[j][i], m);
            }
            centers[i] = DevVecNum(num, den);
        }
           //обновляем вероятности W и считаем расстояние между матрицами W и Wprev
        double max_diff = 0.0;
        for (int i=0; i<n; i++)
            for (int j=0; j<c; j++)
            {
                double total = 0.0;
                for (int k=0; k<c; k++)
                {
                    total += pow(distance(X[i], centers[j]) / distance(X[i], centers[k]), 2/(m-1));
                }
                W[i][j] = 1.0/total;
                max_diff = max(fabs(W[i][j]-Wprev[i][j]), max_diff);
                Wprev[i][j] = W[i][j];
            }
        if (max_diff < eps)
            break;
    }
    return W;
}

void ParallelMatrix(vector<vector<double>> &m, vector<vector<double>> &mprev, double &max_diff,int i_i, int i_k ,int c, vector<vector<double>> X, vector<vector<double>> centers, int m_k)
{
    for (int i= i_i ; i < i_k; i++)
        for (int j=0; j<c; j++)
        {
            double total = 0.0;
            for (int k=0; k<c; k++)
            {
                total += pow(distance(X[i], centers[j]) / distance(X[i], centers[k]), 2/(m_k-1));
            }
            m[i][j] = 1.0/total;
            max_diff = max(fabs(m[i][j]-mprev[i][j]), max_diff);
            mprev[i][j] = m[i][j];
        }
}

vector<vector<double>> FuzzyCMeansParallel(vector<vector<double>> X, int c, int m, double eps, int threads)
{
    int n = X.size();
    int r = X[0].size();
    vector<vector<double>> centers = generateZeroFill(c, r);
    vector<vector<double>> Wprev = generateRandFill(n, c); //membership
    vector<vector<double>> W = Wprev;
    while (true){
        // обновляем центры кластеров
        for (int i=0; i<c; i++)
        {
            vector<double> num(X[0].size());
            double den = 0.0;
            for (int j=0; j<n; j++)
            {
                num = SumVecVec(num, MulNumVec(pow(W[j][i], m),X[j]));
                den += pow(W[j][i], m);
            }
            centers[i] = DevVecNum(num, den);
        }
           //обновляем вероятности W и считаем расстояние между матрицами W и Wprev
        vector<double> maxs(threads);
        int count = n / threads;
        int k = 0;
        vector<thread> threads_arr;
        for(int i = 0; i < threads; i++)
        {
            for(int j = 1; j < count+1; j++, k++)
                ;
            if (i == threads - 1 && k != n)
            {
                k++;
            }
            threads_arr.push_back(std::thread(ParallelMatrix, std::ref(W), std::ref(Wprev), std::ref(maxs[i]),i*count, k,c, X, centers, m));

        }
        double max_diff = 0.0;
        for(int i = 0; i < threads; i++)
        {
            threads_arr[i].join();
            max_diff = max(maxs[i], max_diff);
            
        }
        if (max_diff < eps)
            break;
    }
    return W;
}
