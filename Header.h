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

// Define the two types of 
// graphs to be encountered
#define WEIGHTED "Weighted"
#define UNWEIGHTED "Unweighted"

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;


// Define Node Class
class Node 
{
    public:
    // For Unweighted
    int vertex;

    // For weighted
    string name;
    int weight;
    Node* next;

    Node(int weight = 1){
        this->weight = weight;
    }

};

class Graph 
{
    // To Keep track of type
    char t;
    // Initialize Variables For
    // Unweighted Graph
    private:
    int num_nodes;
    Node** u_adjacency_list;

    // Initialize Variables for 
    // Weighted Graphs
    unordered_map<string, int> name_to_index;
    vector<Node*> w_adjacency_list;

    public:
    // Constructors
    Graph(){
        t = 'w';
    }

    Graph(int nodes)
    {
        t = 'u';
        num_nodes = nodes;
        u_adjacency_list = new Node*[num_nodes]();
        for (int i = 0; i < num_nodes; ++i) {
            u_adjacency_list[i] = nullptr;
        }
    }

    // Destructor
    ~Graph() {
        // Clear Unweighted Graph
        for (int i = 0; i < num_nodes; ++i) {
            Node* current = u_adjacency_list[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] u_adjacency_list;
        // Clear Weighted Graph
        for (Node* node : w_adjacency_list) {
            Node* current = node;
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // Add New Edge in Unweighted Graph
    void addUEdge(int from, int to) {

        // Create a new node
        Node* newNode = new Node;
        newNode->vertex = to;
        newNode->next = nullptr;

        // Add the new node to the
        // list of 'from' vertex
        if (u_adjacency_list[from] == nullptr) {
            u_adjacency_list[from] = newNode;
        } 
        else {
            Node* current = u_adjacency_list[from];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

     // Add New Node and Edge in Weigted Graph
    void addWNode(string name) {

        if (name_to_index.find(name) == name_to_index.end()) {
            int index = w_adjacency_list.size();
            name_to_index[name] = index;
            w_adjacency_list.push_back(nullptr);
        }
    }
   
    void addWEdge(string from, string to, int weight) {
        int from_index = name_to_index[from];
        int to_index = name_to_index[to];

        // Create a new node 
        Node* newNode = new Node;
        newNode->name = to;
        newNode->weight = weight;
        newNode->next = w_adjacency_list[from_index];

        // Add new Node to
        // the list of 'from' vertex
        w_adjacency_list[from_index] = newNode;
    }

    // Print Last List cause
    // the graph is gonna be too big
    // Kinda BVA for unweighted Graph
    void printLastList(){
        if(t=='u')
        {
            cout << "Vertex " << num_nodes-1 << " :";
            Node* current = u_adjacency_list[num_nodes-1];
            while (current != nullptr) {
                cout << " -> " << current->vertex;
                current = current->next;
            }
            cout<< endl;
        }
        else if(t=='w'){
            int last_node_index = w_adjacency_list.size() - 1;
            string last_node_name;
            for (const auto& entry : name_to_index) {
                if (entry.second == last_node_index) {
                    last_node_name = entry.first;
                }
            }
            cout << "Adjacency list of last Node: " << last_node_name << "':" << endl;
            Node* current = w_adjacency_list[last_node_index];
            while (current != nullptr) {
                cout << " -> " << current->name << " (" << current->weight << ")";
                current = current->next;
            }
            cout << endl;
        }
       
    }
    // Print entire Graph
    // Don't call these function!!
    void printUAdjacencyList() {
        for (int i = 0; i < num_nodes; ++i) {
            cout << "Vertex " << i << " :";
            Node* current = u_adjacency_list[i];
            while (current != nullptr) {
                cout << " -> " << current->vertex;
                current = current->next;
            }
            cout << endl;
        }
    }

    void printWAdjacencyList() {
        for (const auto& entry : name_to_index) {
            cout << "Vertex " << entry.first << " :";
            Node* current = w_adjacency_list[entry.second];
            while (current != nullptr) {
                cout << " -> " << current->name << " (" << current->weight << ")";
                current = current->next;
            }
            cout << endl;
        }
    }

};

// Function that loads data from
// two types of files and returns
// a graph object

Graph* loaddataInMatrix(string fileName){

    ifstream infile(fileName);
    if (!infile) {
        cerr << "Error opening file." << endl;
        return nullptr;
    }

    // Read the type of graph
    // and Information
    string type;
    getline(infile, type);
    
    // Based on Type of graph
    // load data differently
    if(type[0] == 'u'){

        string header;
        getline(infile, header);

        cout << "File Type: " << type << endl;
        cout << "Info: "<< header << endl;
        // Extract Meta data
        int nodes, edges;
        istringstream(header) >> ws >> nodes >> ws >> edges;
        cout << "Nodes: %" << nodes << "%" << endl;
        cout << "Edges: %" << edges << "%" << endl;

        Graph* graph = new Graph(nodes);  

        int from, to;
        int counter=0;
        while (infile >> from >> to) {
            graph->addUEdge(from, to);
        }

        // Close the file
        infile.close();
        return graph;

    }
    else if(type[0] == 'w'){

        Graph* graph = new Graph();
        // Skip the Column Names
        string line;
        std::getline(infile, line);

        // Read Data
        while (getline(infile, line)) {
            istringstream iss(line);

            string source, target, type;
            int weight;
            if (getline(iss, source, ',') && 
                getline(iss, target, ',') && 
                (iss >> weight) && 
                getline(iss >> ws,type)) {
                graph->addWNode(source);
                graph->addWNode(target);
                graph->addWEdge(source, target, weight);
                
                // True for our dataset
                // if (type[1] == 'u') {
                //     graph->addWEdge(target, source, weight);
                // }
            }
        }

        // Close the file
        infile.close();
        return graph;
    }else{
        cout<<"TYPE NOT FOUND";
        return nullptr;
    }
        
}

/*
______________________________________________________________________________________________
______________________________________________________________________________________________

           FIRST ATTEMPT: READING DATA IN MATRIX
______________________________________________________________________________________________
______________________________________________________________________________________________
*/

// void load_data_in_matrix(int** matrix, string fileName){
//      // Open the file
//     ifstream infile(fileName);
//     if (!infile) {
//         cerr << "Error opening file." << endl;
//         return;
//     }

//     // Read the type of graph
//     // and Information
//     string type;
//     getline(infile, type);
//     string header;
//     getline(infile, header);

//     cout << "File Type: " << type << endl;
//     cout << "Info: "<< header << endl;
//     // Extract Meta data
//     int nodes, edges;
//     istringstream(header) >> ws >> nodes >> ws >> edges;
//     cout << "Nodes: %" << nodes << "%" << endl;
//     cout << "Edges: %" << edges << "%" << endl;

//     // Create Matrix
//     matrix = (int **)malloc(nodes * sizeof(int *));
//     if (matrix == nullptr) {
//         cerr << "Memory allocation failed." << endl;
//         return;
//     }

//     for (int i = 0; i < nodes; ++i) {
//         matrix[i] = (int *)malloc(nodes * sizeof(int));
//         // Initialize each row to 0
//         memset(matrix[i], 0, nodes * sizeof(int));
//     }

//      // Read from File
//     int from, to;
//     while (infile >> from >> to) 
//     {
//         matrix[from][to] = 1;
//     }
//     // // Close the file
//     infile.close();

//     cout << "Adjacency Matrix:" << endl;
//     for (int i = 0; i < nodes; ++i) {
//         for (int j = 0; j < nodes; ++j) {
//             cout << matrix[i][j] << " ";
//         }
//         cout << endl;
//     }
// }

