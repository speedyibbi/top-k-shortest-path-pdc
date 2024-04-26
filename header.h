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
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <climits>
#include <utility>
#include <algorithm>
#include <omp.h>

using namespace std;

struct Edge {
    string target;
    int weight;
};

void PrintList(map<string, vector<Edge>>);
map<string, vector<Edge>> ReadFile(const string&, bool);
map<string, vector<Edge>> ReadWeightedFile(const string&);
map<string, vector<Edge>> ReadUnweightedFile(const string&);
vector<string> Ddijkstra(const map<string, vector<Edge>>&, const string&, const string&);
vector<vector<string>> YenAlgorithm(const map<string, vector<Edge>>&, const string&, const string&, int, bool, int);
vector<vector<string>> YenAlgorithmSerial(const map<string, vector<Edge>>&, const string&, const string&, int);
vector<vector<string>> YenAlgorithmParallel(const map<string, vector<Edge>>&, const string&, const string&, int);

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

vector<string> Dijkstra(const map<string, vector<Edge>>& graph, const string& start, const string& end) {
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

vector<vector<string>> YenAlgorithm(const map<string, vector<Edge>>& graph, const string& start, const string& end, int K, bool parallelise, int threads = 1) {
    if (parallelise) {
        omp_set_num_threads(threads);
        return YenAlgorithmParallel(graph, start, end, K);
    } else {
        return YenAlgorithmSerial(graph, start, end, K);
    }
}

vector<vector<string>> YenAlgorithmSerial(const map<string, vector<Edge>>& graph, const string& start, const string& end, int K) {
    vector<vector<string>> A; // Store the k shortest paths
    if (start == end || K == 0) return A;
    vector<string> path = Dijkstra(graph, start, end);
    if (path.empty()) return A; // No path found
    A.push_back(path);

    set<vector<string>> B; // Potential k-shortest paths

    for (int k = 1; k < K; ++k) {
        for (size_t i = 0; i < A[k-1].size() - 1; ++i) {
            string spurNode = A[k-1][i];
            vector<string> rootPath(A[k-1].begin(), A[k-1].begin() + i + 1);

            // Remove the links that are part of the previous shortest paths
            map<string, vector<Edge>> tempGraph = graph;
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
            vector<string> spurPath = Dijkstra(tempGraph, spurNode, end);
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

vector<vector<string>> YenAlgorithmParallel(const map<string, vector<Edge>>& graph, const string& start, const string& end, int K) {
    vector<vector<string>> A; // Store the k shortest paths
    if (start == end || K == 0) return A;
    vector<string> path = Dijkstra(graph, start, end);
    if (path.empty()) return A; // No path found
    A.push_back(path);

    set<vector<string>> B; // Potential k-shortest paths

    for (int k = 1; k < K; ++k) {
        vector<vector<string>> localPaths;
        #pragma omp parallel
        {
            #pragma omp for nowait schedule(dynamic)
            for (int i = 0; i < int(A[k-1].size() - 1); ++i) {
                string spurNode = A[k-1][i];
                vector<string> rootPath(A[k-1].begin(), A[k-1].begin() + i + 1);

                // Create a local copy of the graph for each thread
                map<string, vector<Edge>> tempGraph = graph;
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
                vector<string> spurPath = Dijkstra(tempGraph, spurNode, end);
                if (!spurPath.empty() && spurPath[0] == spurNode) {
                    vector<string> totalPath = rootPath;
                    totalPath.insert(totalPath.end(), spurPath.begin() + 1, spurPath.end());
                    #pragma omp critical
                    B.insert(totalPath);
                }
            }
        }

        if (B.empty()) break;

        // Transfer the smallest path from B to A
        A.push_back(*B.begin());
        B.erase(B.begin());
    }

    return A;
}
