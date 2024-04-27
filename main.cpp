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
#include <cstring>

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

    vector<pair<string, string>> randomPairs;

    // Initialize in Master
    char* srcAll[10];
    char* dstAll[10];
    int srCount[10];
    int dsCount[10];

    double sAvg;


    // Generate 10 Random Pair of Nodes
    if (rank == 0){

        cout <<"\n\n**************** SERIAL EXECUTION *****************\n";
        cout <<"**************** Master Chooses 10 Random Nodes *****************\n";
        // Send these pairs using OpenMp to remaining processes later
        randomPairs = GenerateRandomPairs(adjacencyList, 10);
        for (int i = 0; i < 10; ++i) {
            srcAll[i] = new char[randomPairs[i].first.size() + 1];
            strcpy(srcAll[i], randomPairs[i].first.c_str());
            srCount[i] = randomPairs[i].first.size() + 1;

            dstAll[i] = new char[randomPairs[i].second.size() + 1];
            strcpy(dstAll[i], randomPairs[i].second.c_str());
            dsCount[i] = randomPairs[i].second.size() + 1;
        }

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
        sAvg = sum / iterationTime.size();
        cout<<"\n__________________________________________________\n";
        cout<<"__________________________________________________\n";
        cout << "\nAverage Serial Time (10 random nodes) for K-Shortest Path: " << sAvg << " milliseconds\n";
        cout<<"__________________________________________________\n";
        cout<<"__________________________________________________\n";
        
    }

    // All Pocesses wait at barrier
    MPI_Barrier(MPI_COMM_WORLD);


    // Local Random Pairs of each Process
    char* source = new char[100]; 
    char* destination = new char[100];
    if(rank == 0){
        source = srcAll[0];
        destination = dstAll[0];
        for(int i = 1; i<10;i++){
            MPI_Send(srcAll[i], srCount[i] , MPI_CHAR, i, 5, MPI_COMM_WORLD);
            MPI_Send(dstAll[i], dsCount[i] , MPI_CHAR, i, 4, MPI_COMM_WORLD);
        }

        cout <<"\n\n**************** PARALLEL EXECUTION *****************\n";
    }
    else{
        MPI_Recv(source, 100, MPI_CHAR, 0, 5, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(destination, 100, MPI_CHAR, 0, 4, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    
/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                                PARALLEL EXECUTION 
______________________________________________________________________________________________
______________________________________________________________________________________________
*/

    
    auto startTime = chrono::high_resolution_clock::now();
    vector<vector<string>> kPaths = YenAlgorithm(adjacencyList, source, destination, k, true);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    cout << "Paths Found: \n";
    for (auto& path : kPaths) {
        for (string& node : path) {
            cout << node << " -> ";
        }
        cout << "end" << endl;
    }
    cout << "Time taken by Process "<< rank <<": " << duration.count() << " milliseconds\n";
    cout<<"__________________________________________________\n";

    // Collect all the time in master for computation
    long long pTime[10];
    double pAvg;
    if(rank == 0){
        pTime[0] = duration.count();
        for(int i = 1; i <numProcesses; i++){

            MPI_Recv(&pTime[i], 1, MPI_LONG_LONG, i, 3, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        }
    }
    else{
        long long t = duration.count();
        MPI_Send(&t, 1, MPI_LONG_LONG, 0, 3, MPI_COMM_WORLD);
    }

    // All Pocesses wait at barrier
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){

        double sum = 0;
        for (int i = 0; i<10; i++) {
            sum += pTime[i];
        }
        pAvg = sum / 10;
        cout<<"\n__________________________________________________\n";
        cout<<"__________________________________________________\n";
        cout << "\nAverage Parallel Time (10 random nodes) for K-Shortest Path: " << pAvg << " milliseconds\n";
        cout<<"__________________________________________________\n";
        cout<<"__________________________________________________\n";

    }


/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                                TIME AND SPEEDUP ESTIMATION
______________________________________________________________________________________________
______________________________________________________________________________________________
*/


    if(rank == 0){
        double speedup = sAvg/pAvg;
        cout<<"**************************************************\n";
        cout<<"**************************************************\n";
        cout << "\nSPEED-UP ACHIEVED: " << speedup << "\n";
        cout<<"**************************************************\n";
        cout<<"**************************************************\n";

    for (int i = 0; i < 10; ++i) {
            delete srcAll[i];
            delete dstAll[i];
        }
    }
    

    MPI_Finalize();

    return 0;
}
