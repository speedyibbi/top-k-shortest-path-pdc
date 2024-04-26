/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                                MAIN FILE                        
    PDC PROJECT
    Section: CS-Z
    
    Group Members:
    -> i21-2487 Kalsoom Tariq
    -> i21-0572 Kissa Zahra
    -> i21-0601 Ibrahim Khan
______________________________________________________________________________________________
______________________________________________________________________________________________
*/

#include <iostream>
#include <chrono>

#include "header.h"

using namespace std;

int main() {
    // Load Data From File
    string fileName = "./Datasets/Email-Enron.txt";
    // string fileName = "./Datasets/Email-EuAll.txt";
    // string fileName = "./Datasets/classic-who.csv";
    // string fileName = "./Datasets/doctorwho.csv";
    // string fileName = "./Datasets/new-who.csv";

    auto adjList = ReadFile(fileName, false);
    // auto adjList = ReadFile(fileName, true);
    // PrintList(adjList);

    string start = "1";
    string end = "36691";
    int K = 5;

    const int threads = 4;

    auto startTime = chrono::high_resolution_clock::now();
    
    // vector<vector<string>> kPaths = YenAlgorithm(adjList, start, end, K, false);
    vector<vector<string>> kPaths = YenAlgorithm(adjList, start, end, K, true, threads);
    
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    
    cout << "Time taken: " << duration.count() << " ms" << endl << endl;

    for (auto& path : kPaths) {
        for (string& node : path) {
            cout << node << " -> ";
        }
        cout << "end" << endl;
    }

    return 0;
}
