/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: kelvinc
 *
 * Created on 22 de mayo de 2024, 10:29
 */
//
//  main.cpp
//  Homework
//
//  Created by kelvin cervan ruiz on 5/06/24.
//

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <fstream>

using namespace std;

// Node class
class Node {
public:
    // Constructor
    Node(std::string name): name{name} {
    }
    
    // Get node identifier
    std::string get_name() const {
        return name;
    }
    
    // Set node identifier
    void set_name(std::string name) {
        this->name = name;
    }
    
    bool operator==(Node other) const {
        return other.name == name;
    }
    
    bool operator!=(Node other) const {
        return !(name == other.name) ;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << node.get_name();
        return os;
    }
    
    bool operator<(const Node& node) const {
        return name < node.get_name();
    }
    
private:
    string name;
};

// Edge Class
class Edge {
public:
    
    // Constructor
    Edge(Node start, Node end, int cost, int value) : start{start}, end{end}, cost{cost}, value{value}
    {
    }
    
    // Get edge start node
    Node get_start() const {
        return start;
    }
    
    // Get edge end node
    Node get_end() const {
        return end;
    }
    
    // Get cost of edge
    int get_cost() const {
        return cost;
    }
    
    // Set cost of edge
    void set_cost(int cost) {
        this->cost = cost;
    }
    
    // Reverse edge
    Edge reverse() {
        return Edge(this->end, this->start, cost, value + 600);
    }
    
    bool operator==(const Edge & edge) const{
        return start == edge.get_start() && end == edge.get_end();
    }
    
    bool operator<(const Edge & edge) const {
        return value < edge.value;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Edge & edge) {
        os << " " << edge.get_start() << " -- " << edge.get_end();
        return os;
    }
    
private:
    Node start;
    Node end;
    int cost;
    int value;
};

// Graph Class
class Graph {
public:
    
    // Constructor
    Graph(std::set<Node> nodes, std::vector<Edge> edges) : nodes{nodes}, edges{edges} {
        
    }
    
    // Get nodes size
    size_t v() {
        return nodes.size();
    }
    
    // Get edges size
    size_t e() {
        return edges.size();
    }
    
    // Validate if node x and node y are adjacents
    bool adjacent(Node x, Node y) {
        for (const auto & e : edges) {
            if (e.get_start() == x && e.get_end() == y) {
                return true;
            }
        }
        return false;
    }
    
    // Get neighbors of node x
    std::vector<Node> neighbors(Node x) {
        
        std::vector<Node> neighbors;
        
        for (const auto & e : edges) {
            if (x == e.get_start()) {
                neighbors.push_back(e.get_end());
            }
        }
        return neighbors;
    }
    
    // Delete an edge
    void delete_edge(Node x, Node y) {
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (x == it->get_start() && y == it->get_end()) {
                edges.erase(it);
            }
        }
    }
    
    // Get node identifier
    std::string get_node_value(Node x) {
        return x.get_name();
    }
    
    // Set node identifier
    void set_node_value(Node x, std::string a) {
        x.set_name(a);
    }
    
    // Get edge cost
    int get_edge_value(Node x, Node y) {
        for (auto & e : edges) {
            if (e.get_start() == x && e.get_end() == y) {
                return e.get_cost();
            }
        }
        return 0;
    }
    
    // Set edge cost
    void set_edge_value(Node x, Node y, float v) {
        for (auto & e : edges) {
            if (e.get_start() == x && e.get_end() == y) {
                return e.set_cost(v);
            }
        }
    }
    
    // Get nodes
    std::set<Node> get_nodes() {
        return nodes;
    }
    
    // Get adjacents edges of node
    std::vector<Edge> get_adjacent_edges(Node node) {
        std::vector<Edge> edges;
        
        for (const auto & e : this->edges) {
            if (node == e.get_start()) {
                edges.push_back(e);
            }
        }
        return edges;
    }
    
    std::vector<Edge> get_edges() {
        return edges;
    }
    
private:
    set<Node> nodes; // to keep graph nodes
    vector<Edge> edges; // to keep graph edges
};

class MinimumSpanningTree {
public:
    MinimumSpanningTree(Graph graph) : graph{graph} {};
    
    // calculate the minimum span tree and the cost
    void kruskal() {
        
        // Sorted edges
        auto edges = graph.get_edges();
        sort(edges.begin(), edges.end(), [](Edge e1, Edge e2) { return e1.get_cost() < e2.get_cost(); } );
        
        int total_cost = 0;
        
        map<string, set<Edge>> connections;
        list<Edge> valid_edges;
        
        for (Edge edge: edges) {
            
            // Calculate if it is a valid edge
            auto valid_edge = is_valid_edge(connections, edge);
            
            // Add edge to the map
            add_edge_map(connections, valid_edge.first);
            
            // Add edge to the minimum span tree
            if(valid_edge.second) {
                valid_edges.push_back(edge);
            }
        }
        
        // Print the minimum span tree
        cout << endl << "Result: " << endl;
        for (auto v : valid_edges ) {
            cout << endl << v.get_start() << " -- " << v.get_end() << " : " << v.get_cost() << endl;
            total_cost += v.get_cost();
        }
        
        // Print minimum span tree cost
        cout << endl << "Cost: " << total_cost << endl;
    }
    
private:
    Graph graph;
    
    // Build the graph
    void add_edge_map(map<string, set<Edge>> & connections, Edge edge) {
        
        connections[edge.get_start().get_name()].insert(edge);
        connections[edge.get_end().get_name()].insert(edge.reverse());
    }
    
    // Return a pair with the edge and boolean (it is valid edge or not)
    pair<Edge, bool> is_valid_edge(map<string, set<Edge>> &  connections, Edge new_edge) {
        
        if(connections.empty()){
            return make_pair(new_edge, true);
        }
        
        // Copy of connections
        auto connections_copy = connections;
        
        list<Node> unvisited_nodes;
        
        auto start_node = new_edge.get_end();
        auto start_node_name = start_node.get_name();
        
        // Add neighbors to list of the start node
        for (Edge e : connections[start_node_name]) {
            unvisited_nodes.push_back(e.get_end());
        }
        
        // Delete start node from the connection copy
        connections_copy.erase(start_node_name);
        
        // Find if there is a cycle if we add new edge
        while(!unvisited_nodes.empty()) {
            
            // Get unvisited node
            Node unvisited = unvisited_nodes.front();
            unvisited_nodes.pop_front();
            
            // Check if it is equal to new edge start node
            if(new_edge.get_start() == unvisited) {
                return make_pair(new_edge, false);
            }
            
            // Add neighbors of unvisited node
            for(Edge e: connections_copy[unvisited.get_name()]) {
                unvisited_nodes.push_back(e.get_end());
            }
            
            // Erase unvisited node of connections copy
            connections_copy.erase(unvisited.get_name());
        }
        
        return make_pair(new_edge, true);
    }
};

int main() {
    
    // Read file
    std::ifstream file("cplusplus4c_homeworks_Homework3_SampleTestData_mst_data.txt");

    int max_nodes = 0;;

    // Calculate number of lines
    const auto lines_count = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');

    size_t size = lines_count - 1;

    int start[size];
    int end[size];
    int cost[size];

    file.clear();
    file.seekg(0);

    // Read file
    if (file.is_open()) {

        file >> max_nodes;
        int i = 0;
        while (!file.eof()) {
            file >> start[i];
            file >> end[i];
            file >> cost[i];
            i++;
        }

        file.close();
    }
    
    // Create nodes
    set<Node> nodes;
    for (int i = 0; i < max_nodes; i++) {
        Node node = Node("N" + to_string(i));
        nodes.insert(node);
    }
    
    // Create vector with file information
    vector<Edge> edges;
    for (int i = 0; i < size; i++) {
        edges.push_back(Edge(Node("N" + to_string(start[i])), Node("N" + to_string(end[i])), cost[i], i + 1));
    }
    
    // Compute minimum spanning tree
    Graph graph = Graph(nodes, edges);
    MinimumSpanningTree mst = MinimumSpanningTree(graph);
    mst.kruskal();
    
    return 0;
}
