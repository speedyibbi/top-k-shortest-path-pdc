<<<<<<< HEAD
// this is just overall shortest path  
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm> 
#include <limits>
#include <unordered_set>

using namespace std;

struct Edge {
    int dest;
    int weight;
    Edge(int d, int w) : dest(d), weight(w) {}
};

// Function to read the graph from text file
void readGraphFromTextFile(const string& filename, vector<vector<Edge>>& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    // Parse the data
    int from, to;
    while (file >> from >> to) {
        int maxNode = max(from, to);
        if (maxNode >= graph.size()) {
            graph.resize(maxNode + 1);
        }
        // Assuming unweighted graph, add edges with weight 1
        graph[from].emplace_back(to, 1);
        graph[to].emplace_back(from, 1); // For undirected graph
    }

    file.close();
}

// Function to find the shortest path using Dijkstra's algorithm
vector<int> findShortestPath(const vector<vector<Edge>>& graph, int source, int destination) {
    int numNodes = graph.size();
    vector<int> distances(numNodes, numeric_limits<int>::max());
    vector<int> prev(numNodes, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, source });
    distances[source] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (u == destination) {
            break; // Stop when destination is reached
        }

        for (const Edge& edge : graph[u]) {
            int v = edge.dest;
            int w = edge.weight;
            if (d + w < distances[v]) {
                distances[v] = d + w;
                prev[v] = u;
                pq.push({ distances[v], v });
            }
        }
    }

    // Reconstruct the shortest path
    vector<int> shortestPath;
    for (int v = destination; v != -1; v = prev[v]) {
        shortestPath.push_back(v);
    }
    reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;
}

int main() {
    // Read the graph from text file
    vector<vector<Edge>> graph;
    readGraphFromTextFile("Email-Enron.txt", graph); // Replace "your_text_file.txt" with the actual file path

    // Get user input for starting and ending nodes
    int startNode, endNode;
    cout << "Enter the starting node: ";
    cin >> startNode;
    cout << "Enter the ending node: ";
    cin >> endNode;

    // Find the shortest path
    vector<int> shortestPath = findShortestPath(graph, startNode, endNode);

    // Output the shortest path
    cout << "Shortest path from " << startNode << " to " << endNode << ": ";
    cout << startNode;
    for (size_t i = 1; i < shortestPath.size(); ++i) {
        cout << " -> " << shortestPath[i];
    }
    cout << endl;

    return 0;
} 

=======
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
>>>>>>> 517cb063ce0f8f29b2df390f17a25b036eb90f87
