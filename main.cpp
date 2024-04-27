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
    // Initialize Test File Name Variables
    string fileNames[] = {"./Datasets/Email-Enron.txt","./Datasets/Email-EuAll.txt",
                          "./Datasets/classic-who.csv", "./Datasets/doctorwho.csv",
                          "./Datasets/new-who.csv"};
/*
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________

                                   MAIN EXECUTION LOOP
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________
*/
    while(true){

        // Ask user to select a test file
        int choice;
        cout<<"__________________________________________________\n";
        cout<<"__________________________________________________\n";
        cout <<"\nChoose a File: \n\n";
        for (int i = 0; i < 5;i++){
            cout << i+1 <<": " << fileNames[i] <<endl;
        }
        cout<<"\n__________________________________________________\n";
        cout<<"\nYour Choice: ";
        cin >> choice;

        // Declare Adjacency List Variable
        map<string, vector<Edge>> adjacencyList;

        switch(choice){
            // Reading Data From File in Adjacency List
            case 1 ... 5:
                cout <<"\n\n**************** Fetching Data From File *****************\n";
                if(choice == 1 || choice == 2){
                    adjacencyList = ReadFile(fileNames[choice-1], false);
                }
                else{
                     adjacencyList = ReadFile(fileNames[choice-1], true);
                }
                
            break;
            default:
                cout <<"Wrong Option Chosen :(\n I see you want to Exit Code... Bye then"<<endl;
                return 0;
        }

        // Generate 10 Random Pair of Nodes
        cout <<"\n\n**************** Choosing 10 Random Nodes *****************\n";
        vector<pair<string, string>> randomPairs = GenerateRandomPairs(adjacencyList, 10);

        // Output the random pair of Nodes generated
       
        cout << "\nSelected Pair of Nodes: \n\n" << endl;
        for (const auto& pair : randomPairs) {
            cout << pair.first << " -> " << pair.second << endl;
        }
        cout<<"\n__________________________________________________\n";

        int k = -1;
        cout << "\nChoose Value of K (for k shortest path): ";
        cin >> k;
        cout<<"\n\n__________________________________________________\n";
        if (k <= 0){
            cout <<"\nBad Value :(\nExiting...\n";
            return 0;
        }
/*
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________

                                   SERIAL EXECUTION FOR BENCHMARK
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________
*/
        int idx = 1;
        vector<double> iterationTime;
        for (const auto& pair : randomPairs) {
            cout<<"__________________________________________________\n";
            cout << "*********** Running for Node "<< idx++ << " *************\n";
            auto startTime = chrono::high_resolution_clock::now();
            vector<vector<string>> kPaths = YenAlgorithm(adjacencyList, pair.first, pair.second, k, false);
            auto endTime = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
            iterationTime.push_back(duration.count());
            cout << "Paths Found: \n";
            for (auto& path : kPaths) {
                for (string& node : path) {
                    cout << node << " -> ";
                }
                cout << "end" << endl;
            }
            cout << "Time taken: " << duration.count() << " milliseconds\n";
            cout<<"__________________________________________________\n";

        }

        // Averaging Time for calculation Purposes
        // Calculate and display the average time
        double sum = 0;
        for (double time : iterationTime) {
            sum += time;
        }
        double averageTime = sum / iterationTime.size();
        cout<<"\n__________________________________________________\n";
        cout<<"__________________________________________________\n";
        cout << "\nAverage Serial Time (10 random nodes) for K-Shortest Path: " << averageTime << " milliseconds\n";
        cout<<"__________________________________________________\n";
        cout<<"__________________________________________________\n";

/*
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________

                                   PARALLEL EXECUTION 
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________
*/

        



/*
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________

                                   TIME AND SPEEDUP ESTIMATION
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________
*/

        break;
    }
    
    return 0;
}
