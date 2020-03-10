#include <bits/stdc++.h>
#include <memory>
using namespace std;

void fill_rand(vector<vector<double>> &v, int upto){
    for (auto i_row = v.begin(); i_row != v.end(); i_row++){
        for (auto i_col = i_row->begin(); i_col != i_row->end(); i_col++) {
            *i_col = rand() % upto;
        }
    }
}

double find_mean(vector<double> &v){
    return accumulate(v.begin(), v.end(), 0.0) / v.size();
}

void initPopul(vector<vector<double>> &arr, int n, int m, int k, vector<vector<double>> &popul){
    vector<vector<double>> possInd(50, vector<double>(k, 0));
    fill_rand(possInd, n);
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < k; j++)
        {
            popul[i][j] = arr[possInd[i][j]][m];
        }
    }
}

void find_fitness(vector<vector<double>> arr, int n, int m, int k, vector<vector<double>> &popul, int minFitness, vector<double> minArr, int prevRow){
    double minDiff;
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < n; j++)
        {
            minDiff = 101;
            for (int l = 0; l < k; l++)
            {
                if (abs(arr[j][m] - popul[i][l]) < minDiff)
                {
                    minDiff = abs(arr[j][m] - popul[i][l]);
                }
            }
            popul[i][k] += minDiff;
            if (minFitness > popul[i][k])
            {
                minFitness = popul[i][k];
                minArr.assign(popul[i].begin(), popul[i].end() - 1);
                prevRow = i;
            }
        }
    }
}

void selection(vector<vector<double>> arr, int k, vector<vector<double>> &selected, vector<vector<double>> popul){
    vector<vector<double>> possInd(50, vector<double>(10, 0));
    fill_rand(possInd, 50);
    double maxFitness;
    int row;
    for (int i = 0; i < 50; i++)
    {
        maxFitness = -1;
        for (int j = 0; j < 10; j++)
        {
            if (maxFitness < popul[possInd[i][j]][k])
            {
                maxFitness = popul[possInd[i][j]][k];
                row = possInd[i][j];
            }
        }
        selected[i].assign(popul[row].begin(), popul[row].end() - 1);
    }
}

void crossover(vector<vector<double>> &selected, int k){
    int rNum;
    vector<double> temp;
    for (int i = 0; i < 25; i++)
    {
        rNum = rand() % k;
        temp.assign(selected[i].begin() + rNum, selected[i].end() - 1);
        for (int j = rNum; j < selected[i].size() - 1; j++)
        {
            swap(selected[i][j], selected[49 - i][j]);
        }
        int k = 0;
        for (int j = rNum; j < selected[i].size() - 1; j++)
        {
            selected[49 - i][j] = temp[k++];
        }
    }
}

void mutation(vector<vector<double>> arr, vector<vector<double>> &selected, int n, int m, int k){
    int rnd1, rnd2;
    for (int i = 0; i < 50; i++)
    {
        rnd1 = rand() % k;
        rnd2 = rand() % k;
        selected[i][rnd1] = arr[rnd2][m];
    }
}

int main(){
    srand(time(NULL));
    int n, k, m, rows, cols, cnt = 0, prevRow = 0, currRow = 0;
    int minFitness = INT_MAX;
    int minFit = INT_MAX;
    vector<double> minArr(k);
    vector<double> minA(k);

    cout << "Enter the number of students : ";
    cin >> n;
    cout << "Enter the number of groups they are to be dividen into : ";
    cin >> k;
    cout << "Enter the number of subjects : ";
    cin >> m;
    rows = n;
    cols = m;

    vector<vector<double>> arr(rows, vector<double>(cols, 0));
    fill_rand(arr, 101);
    for (int i = 0; i < rows; i++)
    {
        arr[i].push_back(find_mean(arr[i]));
    }
    
    vector<vector<double>> popul((int)50, vector<double>(k + 1, 0));
    initPopul(arr, rows, cols, k, popul);
    find_fitness(arr, rows, cols, k, popul, minFitness, minArr, prevRow);
   
    while (1)
    {
        cnt++;
        vector<vector<double>> selected((int)50, vector<double>(k, 0));
        
        selection(arr, k, selected, popul);
        crossover(selected, k);
        mutation(arr, selected, n, m, k);
        for (int i = 0; i < 50; i++)
        {
            selected[i].push_back(0);
        }
        find_fitness(arr, rows, cols, k, selected, minFit, minA, currRow);
        double minDiff;
        int col;
        if (cnt > 2000 || minFit >= minFitness)
        {
            for (int i = 0; i < n; i++)
            {
                minDiff = 101;
                for (int j = 0; j < k; j++)
                {
                    if (abs(arr[i][m] - popul[prevRow][j]) < minDiff)
                    {
                        minDiff = abs(arr[i][m] - popul[prevRow][j]);
                        col = j;
                    }
                }
                cout << "Student:" << i + 1 << " -> Group:" << col + 1 << " -> Group Representative:" << popul[prevRow][col] << " -> Avg:" << arr[i][m] << " -> Marks:";
                for (int a = 0; a < cols - 1; a++)
                    cout << arr[i][a] << " ";
                cout << "\n";
            }
            break;
        }
        else
        {
            for (int i = 0; i < 50; i++)
            {
                popul[i].assign(selected[i].begin(), selected[i].end());
                minFitness = minFit;
                prevRow = currRow;
            }
        }
    }
    return 0;
}