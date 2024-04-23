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

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void load_data_in_matrix(int** matrix, string fileName){
     // Open the file
    ifstream infile(fileName);
    if (!infile) {
        cerr << "Error opening file." << endl;
        return;
    }

    // Read the type of graph
    // and Information
    string type;
    getline(infile, type);
    string header;
    getline(infile, header);

    cout << "File Type: " << type << endl;
    cout << "Info: "<< header << endl;
    // Extract Meta data
    int nodes, edges;
    istringstream(header) >> ws >> nodes >> ws >> edges;
    cout << "Nodes: %" << nodes << "%" << endl;
    cout << "Edges: %" << edges << "%" << endl;

    // Create Matrix
    matrix = (int **)malloc(nodes * sizeof(int *));
    // Read From File
    int from, to;
    while (infile >> from >> to) {
        matrix[from][to] = 1;
        // cout << matrix[from][to]<<" ";
    }

    // // Close the file
    infile.close();

    // cout << "Adjacency Matrix:" << endl;
    // for (int i = 0; i < nodes; ++i) {
    //     for (int j = 0; j < nodes; ++j) {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}

