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
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <climits>
#include <utility>
#include <algorithm>

#include "header.h"

using namespace std;

typedef map<string, vector<Edge>> AdjList;

vector<string> dijkstra(const AdjList& graph, const string& start, const string& end) {
    map<string, int> distances;
    map<string, string> predecessors;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    set<string> explored;

    for (auto& node : graph) {
        distances[node.first] = INT_MAX;
    }
    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        string u = pq.top().second;
        pq.pop();
        if (explored.find(u) != explored.end()) continue;
        explored.insert(u);

        if (u == end) break;

        for (const auto& e : graph.at(u)) {
            string v = e.target;
            int weight = e.weight;
            int distanceThroughU = distances[u] + weight;
            if (distanceThroughU < distances[v]) {
                distances[v] = distanceThroughU;
                predecessors[v] = u;
                pq.push({distanceThroughU, v});
            }
        }
    }

    // Reconstruct the shortest path from end to start by predecessors
    vector<string> path;
    for (string at = end; at != start; at = predecessors[at]) {
        path.push_back(at);
        if (predecessors.find(at) == predecessors.end()) return {}; // No path
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

vector<vector<string>> yenAlgorithm(const AdjList& graph, const string& start, const string& end, int K) {
    vector<vector<string>> A; // Store the k shortest paths
    if (start == end || K == 0) return A;
    vector<string> path = dijkstra(graph, start, end);
    if (path.empty()) return A; // No path found
    A.push_back(path);

    set<vector<string>> B; // Potential k-shortest paths

    for (int k = 1; k < K; ++k) {
        for (size_t i = 0; i < A[k-1].size() - 1; ++i) {
            string spurNode = A[k-1][i];
            vector<string> rootPath(A[k-1].begin(), A[k-1].begin() + i + 1);

            // Remove the links that are part of the previous shortest paths
            AdjList tempGraph = graph;
            for (auto& p : A) {
                if (p.size() > i && equal(p.begin(), p.begin() + i + 1, rootPath.begin())) {
                    tempGraph[p[i]].erase(remove_if(tempGraph[p[i]].begin(), tempGraph[p[i]].end(),
                        [&](const Edge& e){ return e.target == p[i+1]; }), tempGraph[p[i]].end());
                }
            }

            // Remove all nodes in rootPath except spurNode from tempGraph
            for (string& rpNode : rootPath) {
                if (rpNode != spurNode) {
                    tempGraph.erase(rpNode);
                }
            }

            // Calculate the spur path from the spur node to the target node
            vector<string> spurPath = dijkstra(tempGraph, spurNode, end);
            if (!spurPath.empty() && spurPath[0] == spurNode) {
                vector<string> totalPath = rootPath;
                totalPath.insert(totalPath.end(), spurPath.begin() + 1, spurPath.end());
                B.insert(totalPath);
            }
        }

        if (B.empty()) break;

        // Transfer the smallest path from B to A
        A.push_back(*B.begin());
        B.erase(B.begin());
    }

    return A;
}

int main() {
    // Load Data From File
    // string fileName = "./Datasets/Email-Enron.txt";
    string fileName = "./Datasets/Email-EuAll.txt";
    // string fileName = "./Datasets/classic-who.csv";
    // string fileName = "./Datasets/doctorwho.csv";
    // string fileName = "./Datasets/new-who.csv";

    auto adjList = ReadFile(fileName, false);
    // auto adjList = ReadFile(fileName, true);
    // PrintList(adjList);

    string start = "1";
    string end = "255750";
    int K = 3; // Find the top 3 shortest paths

    vector<vector<string>> kPaths = yenAlgorithm(adjList, start, end, K);
    for (auto& path : kPaths) {
        for (string& node : path) {
            cout << node << " -> ";
        }
        cout << "end" << endl;
    }

    return 0;
}



// int main() {
//     // Load Data From File
//     string fileName = "./Datasets/Email-Enron.txt";
//     // string fileName = "./Datasets/Email-EuAll.txt";
//     // string fileName = "./Datasets/classic-who.csv";
//     // string fileName = "./Datasets/doctorwho.csv";
//     // string fileName = "./Datasets/new-who.csv";

//     auto adjList = ReadFile(fileName, false);
//     // auto adjList = ReadFile(fileName, true);
//     // PrintList(adjList);

//     return 0;
// }
