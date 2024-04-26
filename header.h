/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                               HEADER FILE
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
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>

using namespace std;

struct Edge {
    string target;
    int weight;
};

void PrintList(map<string, vector<Edge>>);
map<string, vector<Edge>> ReadFile(const string&, bool);
map<string, vector<Edge>> ReadWeightedFile(const string&);
map<string, vector<Edge>> ReadUnweightedFile(const string&);

void PrintList(map<string, vector<Edge>> adjList) {
    for (const auto& pair : adjList) {
        cout << pair.first << " -> ";
        for (const auto& edge : pair.second) {
            cout << "(" << edge.target << ", " << edge.weight << ") ";
        }
        cout << endl;
    }
}

map<string, vector<Edge>> ReadFile(const string& filename, bool weighted) {
    if (weighted) return ReadWeightedFile(filename);
    else return ReadUnweightedFile(filename);
}

map<string, vector<Edge>> ReadWeightedFile(const string& filename) {
    ifstream file(filename);
    string line;
    map<string, vector<Edge>> adjList;

    // Skip the first two lines (header lines)
    getline(file, line);
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string source, target, weightStr, type;
        getline(iss, source, ',');
        getline(iss, target, ',');
        getline(iss, weightStr, ',');
        getline(iss, type, ',');

        int weight = stoi(weightStr);
        adjList[source].push_back({target, weight});
        adjList[target].push_back({source, weight});
    }
    return adjList;
}

map<string, vector<Edge>> ReadUnweightedFile(const string& filename) {
    ifstream file(filename);
    string line;
    map<string, vector<Edge>> adjList;

    // Skip the first line (header line)
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string source, target;
        if (!(iss >> source >> target)) {
            continue; // Skip any malformed lines
        }
        adjList[source].push_back({target, 1});
        adjList[target].push_back({source, 1});
    }
    return adjList;
}
