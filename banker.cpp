//Yusairah Haque 
//banker.cpp 
//11/22/2025
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int main() {
    //reads in from input file. prints error if fail
    ifstream fin("input.txt");
    if (!fin) {
        cerr << "Error: could not open input.txt\n";
        return 1;
    }
    int n, m;
    fin >> n >> m;  //number of process resource types 

    //vector... allocates space for the below. then fills them from input.txt:
    // 1. n lines of allocation 
    // 2. n lines of maximum 
    // 3. 1 line of available 
    //read allocation matrix 
    vector<vector<int>> allocation (n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fin >> allocation[i][j];
        }
    }
    //read maximum matrix
    vector<vector<int>> maximum(n, vector<int>(m));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++) {
            fin >> maximum[i][j];
        }
    }
    //read available vector
    vector<int> available(m);
    for (int j = 0; j < m; j++) {
        fin >> available[j];
    }

    //compute need matrix: shows how many more resources each process still require
    //to finish execution. 
    vector<vector<int>> need(n, vector<int>(m)); 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            //need: how many of resource j process i still needs to finish
            //maximum: max number of resource j process i is currently holding
            //allocation: number of units of resource j process i is currently holding
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    //safety algorithms 
    vector<bool> finished(n, false);    //has process i been simulated to completion
    vector<int> safeSequence;           //stores the order of processes that make system safe
    vector<int> work = available;       //work is a temp copy of available that updates if process finish and release resources.

    int finishedCount = 0;
    //keeps going until all processes are marked finished.
    while (finishedCount < n) {
        //found is set to true if a new process is run in a round/loop
        bool found = false; 

        for (int p = 0; p < n; p++) {
            if (!finished[p]) {
                bool canRun = true; 

            for (int j = 0; j < m; j++){
                //checks if it can run with current work
                if (need[p][j] > work[j]) {
                    canRun = false;
                    break;
                }
            }

                if (canRun) {
                    //process can finish
                    for (int j = 0; j < m; j++)
                    //releases allocated resources after it is run
                    work[j] += allocation[p][j];

                    safeSequence.push_back(p);
                    finished[p] = true;
                    finishedCount++;
                    found = true;
                }
            }
        }
    
        if (!found) {
            cout << "System is not in a safe state.\n"; 
            return 0; 
        }
    }

    cout << "System is in a safe state.\n";
    cout << "Safe sequence: "; 
    for (int p : safeSequence)
        cout << "P" << p << " "; 
    cout << endl;

    return 0; 
}