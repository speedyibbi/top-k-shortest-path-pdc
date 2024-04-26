#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <sstream>
#include <limits>

using namespace std;

// Structure to represent an edge
struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

// Function to read dataset 1 and create a distance matrix
void readDataset1(const string& filename, vector<vector<Edge>>& graph, unordered_map<string, int>& nodeIndices) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string source, target, type;
        int weight;
        ss >> source >> target >> weight >> type;

        if (nodeIndices.find(source) == nodeIndices.end()) {
            nodeIndices[source] = nodeIndices.size();
            graph.push_back(vector<Edge>());
        }
        if (nodeIndices.find(target) == nodeIndices.end()) {
            nodeIndices[target] = nodeIndices.size();
            graph.push_back(vector<Edge>());
        }

        int sourceIdx = nodeIndices[source];
        int targetIdx = nodeIndices[target];

        graph[sourceIdx].push_back(Edge(targetIdx, weight));
        graph[targetIdx].push_back(Edge(sourceIdx, weight)); // For undirected graph
    }

    file.close();
}

// Function to read dataset 2 and create a distance matrix
void readDataset2(const string& filename, vector<vector<Edge>>& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int from, to;
        ss >> from >> to;

        if (from >= graph.size()) graph.resize(from + 1);
        if (to >= graph.size()) graph.resize(to + 1);

        graph[from].push_back(Edge(to, 1));
        graph[to].push_back(Edge(from, 1)); // For undirected graph
    }

    file.close();
}

// Dijkstra's algorithm to find the k shortest paths
vector<vector<int>> kShortestPaths(const vector<vector<Edge>>& graph, int source, int k) {
    int n = graph.size();
    vector<vector<int>> shortestPaths;

    priority_queue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, greater<pair<int, vector<int>>>> pq;
    pq.push({0, {source}});

    while (!pq.empty() && shortestPaths.size() < k) {
        auto [cost, path] = pq.top();
        pq.pop();
        int u = path.back();

        if (shortestPaths.empty() || cost != shortestPaths.back().back()) {
            shortestPaths.push_back(path);
        }

        if (shortestPaths.size() == k) break;

        for (const auto& edge : graph[u]) {
            int v = edge.to;
            vector<int> newPath = path;
            newPath.push_back(v);
            pq.push({cost + edge.weight, newPath});
        }
    }

    return shortestPaths;
}

int main() {
    // Dataset 1
    // vector<vector<Edge>> graph1;
    // unordered_map<string, int> nodeIndices1;
    // readDataset1("./Datasets/doctorwho.csv", graph1, nodeIndices1);

    // Dataset 2
    vector<vector<Edge>> graph2;
    readDataset2("./Datasets/Email-Enron.txt", graph2);

    // Example usage: Find the 3 shortest paths from node 0
    int sourceNode = 0;
    int k = 3;
    vector<vector<int>> shortestPaths = kShortestPaths(graph2, sourceNode, k);

    // Output shortest paths
    cout << "Shortest Paths from node " << sourceNode << ":" << endl;
    for (int i = 0; i < shortestPaths.size(); ++i) {
        cout << "Path " << i+1 << ": ";
        for (int j = 0; j < shortestPaths[i].size(); ++j) {
            cout << shortestPaths[i][j] << (j == shortestPaths[i].size()-1 ? "\n" : " -> ");
        }
    }

    return 0;
}
