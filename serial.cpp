#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>

struct Edge {
    std::string target;
    int weight;  // Always 1 for unweighted graphs
};

int main() {
    std::ifstream file("./Datasets/Email-Enron.txt");
    std::string line;
    std::map<std::string, std::vector<Edge>> adjList;

    // Skip the first line (header line)
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string source, target;
        // Read each line's contents
        if (!(iss >> source >> target)) {
            continue; // Skip any malformed lines
        }

        // Add edge to adjacency list with weight 1
        adjList[source].push_back({target, 1});
        // Optionally add the reverse edge
        // adjList[target].push_back({source, 1});
    }

    // Output the adjacency list
    for (const auto& pair : adjList) {
        std::cout << pair.first << " -> ";
        for (const auto& edge : pair.second) {
            std::cout << "(" << edge.target << ", " << edge.weight << ") ";
        }
        std::cout << std::endl;
    }

    return 0;
}



// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <map>
// #include <vector>
// #include <utility>

// // Define a struct to store edges
// struct Edge {
//     std::string target;
//     int weight;
// };

// int main() {
//     std::ifstream file("./Datasets/classic-who.csv");
//     std::string line;
//     // Map to store the adjacency list
//     std::map<std::string, std::vector<Edge>> adjList;

//     // Skip the first two lines (header lines)
//     std::getline(file, line);
//     std::getline(file, line);

//     while (std::getline(file, line)) {
//         std::istringstream iss(line);
//         std::string source, target, weightStr, type;
//         // Read each line's contents
//         std::getline(iss, source, ',');
//         std::getline(iss, target, ',');
//         std::getline(iss, weightStr, ',');
//         std::getline(iss, type, ',');

//         // Convert weight to integer
//         int weight = std::stoi(weightStr);

//         // Add edge to adjacency list
//         adjList[source].push_back({target, weight});
//         // Since the graph is undirected, add the reverse edge as well
//         adjList[target].push_back({source, weight});
//     }

//     // Output the adjacency list
//     for (const auto& pair : adjList) {
//         std::cout << pair.first << " -> ";
//         for (const auto& edge : pair.second) {
//             std::cout << "(" << edge.target << ", " << edge.weight << ") ";
//         }
//         std::cout << std::endl;
//     }

//     return 0;
// }
