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

#include "header.h"

int main() {
    // Load Data From File
    string fileName = "./Datasets/Email-Enron.txt";
    // string fileName = "./Datasets/Email-EuAll.txt";
    // string fileName = "./Datasets/classic-who.csv";
    // string fileName = "./Datasets/doctorwho.csv";
    // string fileName = "./Datasets/new-who.csv";

    auto adjList = ReadFile(fileName, false);
    // auto adjList = ReadFile(fileName, true);
    PrintList(adjList);

    return 0;
}
