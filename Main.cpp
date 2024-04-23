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

#include <iostream>
#include <string.h>
#include "Header.h"
using namespace std;

// Declare Matrix
int** distanceMatrix;

int main(){

    // Load Data From File
    string fileName = "/Datasets/Email-Enron.txt";
    load_data_in_matrix(distanceMatrix, file_name);
    
}