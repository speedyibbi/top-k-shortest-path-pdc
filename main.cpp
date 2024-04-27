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
#include <mpi.h>

#include "header.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Initialize Test File Name Variables
    string fileNames[] = {"./Datasets/Email-Enron.txt","./Datasets/Email-EuAll.txt",
                          "./Datasets/classic-who.csv", "./Datasets/doctorwho.csv",
                          "./Datasets/new-who.csv"};

    // Innitialize MPI Environment
    MPI_Init(&argc, &argv);

    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses); 

    if(numProcesses != 10){
        cout<<"Please Run the Code with 10 Processes...\n";
        MPI_Finalize();
        return 0;
    }

    // Declare Adjacency List  for each
    map<string, vector<Edge>> adjacencyList;

/*
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________

                                   MAIN EXECUTION 
    ______________________________________________________________________________________________
    ______________________________________________________________________________________________
*/
    int choice;
    int k = -1;
    if(rank == 0){
          // Ask user to select a test file
        cout<<"__________________________________________________\n";
        cout<<"__________________________________________________\n";
        cout <<"\nChoose a File: \n\n";
        for (int i = 0; i < 5;i++){
            cout << i+1 <<": " << fileNames[i] <<endl;
        }
        cout<<"\n__________________________________________________\n";
        cout<<"\nYour Choice: ";
        cin >> choice;

        // Send Index over MPI
        for(int i = 1; i <numProcesses; i++){
            MPI_Send(&choice, 1, MPI_INT, i, 20, MPI_COMM_WORLD);
        }

        // Get K value from user in Master
        cout << "\nChoose Value of K (for k shortest path): ";
        cin >> k;
        cout<<"\n__________________________________________________\n";
        if (k <= 0){
            cout <<"\nBad Value :(\nExiting...\n";
            MPI_Finalize();
            return 0;
        }

        // Send Value of K over MPI
        for(int i = 1; i <numProcesses; i++){
            MPI_Send(&k, 1, MPI_INT, i, 10, MPI_COMM_WORLD);
        }

    }
    else
    {
        MPI_Recv(&choice, 1, MPI_INT, 0, 20, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&k, 1, MPI_INT, 0, 10, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
  
    // All Processes Load Data From File
    switch(choice){
        // Reading Data From File in Adjacency List
        case 1 ... 5:
            if (rank == 0){
                 cout <<"\n\n**************** Fetching Data From File *****************\n";
            }
           
            if(choice == 1 || choice == 2){
                adjacencyList = ReadFile(fileNames[choice-1], false);
            }
            else{
                adjacencyList = ReadFile(fileNames[choice-1], true);
            }
            
        break;
        default:
            if (rank == 0){
                cout <<"Wrong Option Chosen :(\n I see you want to Exit Code... Bye then"<<endl;
            }
            MPI_Finalize();
            return 0;
    }

    // Generate 10 Random Pair of Nodes
    if (rank == 0){

        cout <<"\n\n**************** SERIAL EXECUTION *****************\n";
        cout <<"**************** Master Chooses 10 Random Nodes *****************\n";
        vector<pair<string, string>> randomPairs = GenerateRandomPairs(adjacencyList, 10);

        // Output the random pair of Nodes generated
    
        cout << "\nSelected Pair of Nodes: \n\n" << endl;
        for (const auto& pair : randomPairs) {
            cout << pair.first << " -> " << pair.second << endl;
        }
        cout<<"\n__________________________________________________\n";

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
        
    }
    // All Pocesses wait at barrier
    MPI_Barrier(MPI_COMM_WORLD);
    cout<< "Process "<<rank<<  " moving from Barrier\n";
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

    MPI_Finalize();

    return 0;
}
