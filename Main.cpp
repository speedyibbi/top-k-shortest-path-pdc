/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                         MAIN FILE                        
    PDC PROJECT
    Section: CS-Z
    
    Group Members:
    -> i21-0601 Ibbrahim Khan
    -> i21-2487 Kalsoom Tariq
    -> i21-0572 Kissa Zahra
______________________________________________________________________________________________
______________________________________________________________________________________________
*/


#include "Header.h"

int main(){

    // Load Data From File
    //string fileName = "./Datasets/Email-Enron.txt";
    //string fileName = "./Datasets/Email-EuAll.txt";
    //string fileName = "./Datasets/classic-who.csv";
    //string fileName = "./Datasets/doctorwho.csv";
    //string fileName = "./Datasets/new-who.csv";

    Graph* graph =  loaddataInMatrix(fileName);
    graph->printLastList();

}