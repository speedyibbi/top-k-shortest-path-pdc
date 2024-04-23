/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                               HEADER FILE
    PDC PROJECT
    Section: CS-Z
    
    Group Members:
    -> i21-0601 Ibbrahim Khan
    -> i21-2487 Kalsoom Tariq
    -> i21-0572 Kissa Zahra
______________________________________________________________________________________________
______________________________________________________________________________________________
*/
#include <fstream>
#include <vector>
#include <sstream>

void load_data_in_matrix(int** matrix, string fileName){
     // Open the file
    ifstream infile(fileName);
    if (!infile) {
        cerr << "Error opening file." << endl;
        return;
    }
}

