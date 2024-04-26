#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <sstream>
#include <algorithm>
#include <set>
#include<omp.h>

#include "header.h"
using namespace std;

// Comparator for min heap based on edge weight
struct CompareEdges {
    bool operator()(const pair<int, string>& a, const pair<int, string>& b) {
        return a.first > b.first;
    }
};


/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                                SERIAL 
______________________________________________________________________________________________
______________________________________________________________________________________________
*/


// Dijkstra's algorithm to find shortest paths from source to all nodes
map<string, int> dijkstra(const map<string, vector<Edge>>& adjList, const string& source) {
    map<string, int> distance;
    priority_queue<pair<int, string>, vector<pair<int, string>>, CompareEdges> pq;

    // Initialize distances
    for (const auto& pair : adjList) {
        distance[pair.first] = numeric_limits<int>::max();
    }
    distance[source] = 0;

    pq.push({0, source});

    while (!pq.empty()) {
        auto [dist, node] = pq.top();
        pq.pop();

        if (dist > distance[node]) continue;

        for (const auto& edge : adjList.at(node)) {
            int new_dist = dist + edge.weight;
            if (new_dist < distance[edge.target]) {
                distance[edge.target] = new_dist;
                pq.push({new_dist, edge.target});
            }
        }
    }

    return distance;
}


// Function to find k shortest paths using Dijkstra's algorithm
vector<vector<string>> kShortestPaths(const map<string, vector<Edge>>& adjList, const string& source, const string& target, int k) {
    map<string, vector<vector<string>>> paths;
    vector<vector<string>> result;

    // Find shortest path from source to all nodes
    map<string, int> distances = dijkstra(adjList, source);

    // Initialize visited set to keep track of visited nodes
    set<string> visited;

    // Initialize the heap with source node and its distance
    priority_queue<pair<int, vector<string>>, vector<pair<int, vector<string>>>, greater<pair<int, vector<string>>>> pq;
    pq.push({distances[target], {target}});

    // Loop until k shortest paths are found
    while (!pq.empty() && result.size() < k) {
        auto [dist, path] = pq.top();
        pq.pop();

        string current_node = path.back();

        // Skip if the current node has been visited earlier
        if (visited.count(current_node)) {
            continue;
        }

        // Mark the current node as visited
        visited.insert(current_node);

        // If current node is same as source node, it means we have found a path
        // In such case, we add it to the result and continue to find next path
        if (current_node == source) {
            reverse(path.begin(), path.end());
            result.push_back(path);
            continue;
        }

        // Expand the current node and add its neighbors to the priority queue
        for (const auto& edge : adjList.at(current_node)) {
            // Skip the edge if the target node has been visited earlier
            if (visited.count(edge.target)) {
                continue;
            }
            vector<string> new_path = path;
            new_path.push_back(edge.target);
            pq.push({distances[edge.target], new_path});
        }
    }

    return result;
}





int main() {
    // Read dataset into adjacency list
    // Load Data From File
    //string fileName = "./Datasets/classic-who.csv";
    string fileName = "./Datasets/Email-EuAll.txt";
    // string fileName = "./Datasets/classic-who.csv";
    // string fileName = "./Datasets/doctorwho.csv";
    // string fileName = "./Datasets/new-who.csv";

    //auto adjList = ReadFile(fileName, true);
    auto adjList = ReadFile(fileName, false);
    //PrintList(adjList);
    // // Example usage: Find the 3 shortest paths from "A" to "D"
    string source = "1";
    string target = "255750";
    int k = 3;
    vector<vector<string>> shortestPaths = kShortestPaths(adjList, source, target, k);

    // Output shortest paths
    cout << "Shortest Paths from " << source << " to " << target << ":" << endl;
    for (int i = 0; i < shortestPaths.size(); ++i) {
        cout << "Path " << i+1 << ": ";
        for (int j = 0; j < shortestPaths[i].size(); ++j) {
            cout << shortestPaths[i][j] << (j == shortestPaths[i].size()-1 ? "\n" : " -> ");
        }
    }

    return 0;
}

/*
______________________________________________________________________________________________
______________________________________________________________________________________________

Try 1

______________________________________________________________________________________________
______________________________________________________________________________________________
*/

// // Function to find k shortest paths using Dijkstra's algorithm
// vector<vector<string>> kShortestPaths(const map<string, vector<Edge>>& adjList, const string& source, const string& target, int k) {
//     map<string, vector<vector<string>>> paths;
//     vector<vector<string>> result;

//     // Find shortest path from source to all nodes
//     map<string, int> distances = dijkstra(adjList, source);

//     // Initialize the heap with source node and its distance
//     priority_queue<pair<int, vector<string>>, vector<pair<int, vector<string>>>, greater<pair<int, vector<string>>>> pq;
//     pq.push({distances[target], {target}});

//     // Loop until k shortest paths are found
//     while (!pq.empty() && result.size() < k) {
//         auto [dist, path] = pq.top();
//         pq.pop();

//         string current_node = path.back();
//         if (current_node == source) {
//             reverse(path.begin(), path.end());
//             result.push_back(path);
//         }

//         for (const auto& edge : adjList.at(current_node)) {
//             if (distances[edge.target] < distances[current_node]) {
//                 vector<string> new_path = path;
//                 new_path.push_back(edge.target);
//                 pq.push({distances[edge.target], new_path});
//             }
//         }
//     }

//     return result;
// }








/*
______________________________________________________________________________________________
______________________________________________________________________________________________

                                PARALLEL
______________________________________________________________________________________________
______________________________________________________________________________________________
*/


// //Dijkstra's algorithm to find shortest paths from source to all nodes (with OpenMP parallelization)
// map<string, int> dijkstra(const map<string, vector<Edge>>& adjList, const string& source) {
//     map<string, int> distance;
//     priority_queue<pair<int, string>, vector<pair<int, string>>, CompareEdges> pq;

//     // Initialize distances
//     for (const auto& pair : adjList) {
//         distance[pair.first] = numeric_limits<int>::max();
//     }
//     distance[source] = 0;

//     pq.push({0, source});

//     while (!pq.empty()) {
//         auto [dist, node] = pq.top();
//         pq.pop();

//         if (dist > distance[node]) continue;

//         // Parallelize this loop
//         #pragma omp parallel for num_threads(4)
//         for (size_t i = 0; i < adjList.at(node).size(); ++i) {
//             const auto& edge = adjList.at(node)[i];
//             int new_dist = dist + edge.weight;
//             if (new_dist < distance[edge.target]) {
//                 // Ensure atomic update to distance map and push to priority queue
//                 #pragma omp critical
//                 {
//                     if (new_dist < distance[edge.target]) {
//                         distance[edge.target] = new_dist;
//                         pq.push({new_dist, edge.target});
//                     }
//                 }
//             }
//         }
//     }

//     return distance;
// }

// // Function to find k shortest paths using Dijkstra's algorithm (with OpenMP parallelization)
// vector<vector<string>> kShortestPaths(const map<string, vector<Edge>>& adjList, const string& source, const string& target, int k) {
//     map<string, vector<vector<string>>> paths;
//     vector<vector<string>> result;

//     // Find shortest path from source to all nodes
//     map<string, int> distances = dijkstra(adjList, source);

//     // Initialize the heap with source node and its distance
//     priority_queue<pair<int, vector<string>>, vector<pair<int, vector<string>>>, greater<pair<int, vector<string>>>> pq;
//     pq.push({distances[target], {target}});

//     // Initialize visited set to keep track of visited nodes
//     set<string> visited;

//     // Loop until k shortest paths are found
//     #pragma omp parallel
//     {
//         vector<vector<string>> local_result;
//         bool should_continue = false; // Local flag to control loop execution

//         #pragma omp single
//         {
//             while (!pq.empty() && result.size() < k) {
//                 auto [dist, path] = pq.top();
//                 pq.pop();

//                 string current_node = path.back();

//                 // Skip if the current node has been visited earlier
//                 if (visited.count(current_node)) {
//                     should_continue = true;
//                 }

//                 // Mark the current node as visited
//                 #pragma omp critical
//                 visited.insert(current_node);

//                 // If current node is same as source node, it means we have found a path
//                 // In such case, we add it to the result and continue to find next path
//                 if (current_node == source) {
//                     reverse(path.begin(), path.end());
//                     local_result.push_back(path);
//                     should_continue = true;
//                 }

//                 // If should_continue is false, execute loop body
//                 if (!should_continue) {
//                     // Expand the current node and add its neighbors to the priority queue
//                     for (const auto& edge : adjList.at(current_node)) {
//                         // Skip the edge if the target node has been visited earlier
//                         if (!visited.count(edge.target)) {
//                             // Create a new path and add it to the priority queue
//                             vector<string> new_path = path;
//                             new_path.push_back(edge.target);

//                             // Ensure atomic update to priority queue
//                             #pragma omp critical
//                             pq.push({distances[edge.target], new_path});
//                         }
//                     }
//                 }
//             }
//         }

//         // Combine local results
//         #pragma omp critical
//         result.insert(result.end(), local_result.begin(), local_result.end());
//     }

//     return result;
// }