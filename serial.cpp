#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm> // for sort
#include <limits>

using namespace std;

struct Edge {
    int dest;
    int weight;
    Edge(int d, int w) : dest(d), weight(w) {}
};

void readGraphFromFile(const string& filename, vector<vector<Edge>>& graph, int& numNodes) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    int from, to;
    while (file >> from >> to) {
        int maxNode = max(from, to);
        numNodes = max(numNodes, maxNode + 1);
        if (numNodes > graph.size()) {
            graph.resize(numNodes);
        }
        graph[from].emplace_back(to, 1); // Assuming unweighted graph
        graph[to].emplace_back(from, 1); // For undirected graph
    }

    file.close();
}

void findKShortestPaths(const vector<vector<Edge>>& graph, int source, int destination, int k) {
    int numNodes = graph.size();
    vector<vector<int>> distances(numNodes, vector<int>(k, numeric_limits<int>::max()));
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, source });
    distances[source][0] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (distances[u][k - 1] < d) continue;
        for (const Edge& edge : graph[u]) {
            int v = edge.dest;
            int w = edge.weight;
            if (d + w < distances[v][k - 1]) {
                distances[v][k - 1] = d + w;
                sort(distances[v].begin(), distances[v].end());
                pq.push({ d + w, v });
            }
        }
    }

    cout << "K shortest paths from " << source << " to " << destination << ":" << endl;
    for (int i = 0; i < k; ++i) {
        cout << distances[destination][i] << " ";
    }
    cout << endl;
}

int main() {
    vector<vector<Edge>> graph;
    int numNodes = 0;
    readGraphFromFile("Email-Enron.txt", graph, numNodes); // Provide the correct file name
    if (numNodes == 0) {
        cerr << "Error: No nodes found in the graph." << endl;
        return 1;
    }

    // Example: Finding top 3 shortest paths from node 0 to node 1
    int source = 0;
    int destination = 70;
    int k = 3;
    findKShortestPaths(graph, source, destination, k);

    return 0;
}
