#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjList;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> weights;

    // Helper functions
    void dfsHelper(const std::string& node, 
                std::unordered_map<std::string, bool>& visited,
                std::function<void(const std::string&)> visit) const;
    
    void findPathsHelper(const std::string& current, const std::string& end,
                        std::unordered_map<std::string, bool>& visited,
                        std::vector<std::string>& path,
                        std::vector<std::vector<std::string>>& allPaths) const;
    
    bool hasCycleHelper(const std::string& node,
                    std::unordered_map<std::string, bool>& visited,
                    std::unordered_map<std::string, bool>& recStack) const;
    
    void articulationPointsHelper(const std::string& u,
                                std::unordered_map<std::string, bool>& visited,
                                std::unordered_map<std::string, int>& disc,
                                std::unordered_map<std::string, int>& low,
                                std::unordered_map<std::string, std::string>& parent,
                                std::vector<std::string>& ap, int& time) const;
    
    void bridgesHelper(const std::string& u,
                    std::unordered_map<std::string, bool>& visited,
                    std::unordered_map<std::string, int>& disc,
                    std::unordered_map<std::string, int>& low,
                    std::unordered_map<std::string, std::string>& parent,
                    std::vector<std::pair<std::string, std::string>>& bridges,
                    int& time) const;

public:
    // Constructor and Destructor
    Graph();
    ~Graph();

    // Basic operations
    void addNode(const std::string& node);
    void addEdge(const std::string& from, const std::string& to, int weight = 1);
    void removeEdge(const std::string& from, const std::string& to);
    void removeNode(const std::string& node);
    void clear();

    // Query operations
    bool hasNode(const std::string& node) const;
    bool hasEdge(const std::string& from, const std::string& to) const;
    int getEdgeWeight(const std::string& from, const std::string& to) const;
    void setEdgeWeight(const std::string& from, const std::string& to, int weight);
    std::vector<std::string> getNeighbors(const std::string& node) const;
    std::vector<std::string> getAllNodes() const;
    std::vector<std::pair<std::string, std::string>> getAllEdges() const;

    // Graph metrics
    int getNodeDegree(const std::string& node) const;
    int getOutDegree(const std::string& node) const;
    int getInDegree(const std::string& node) const;
    int getNodeCount() const;
    int getEdgeCount() const;
    bool isEmpty() const;

    // Graph algorithms
    void bfs(const std::string& start, 
            std::function<void(const std::string&)> visit) const;
    void dfs(const std::string& start, 
            std::function<void(const std::string&)> visit) const;
    std::vector<std::string> shortestPath(const std::string& start, const std::string& end) const;
    std::vector<std::vector<std::string>> findAllPaths(const std::string& start, const std::string& end) const;
    int shortestPathLength(const std::string& start, const std::string& end) const;
    bool isConnected() const;
    bool hasCycle() const;
    std::vector<std::string> topologicalSort() const;
    std::vector<std::vector<std::string>> findConnectedComponents() const;
    std::vector<std::string> findArticulationPoints() const;
    std::vector<std::pair<std::string, std::string>> findBridges() const;
    bool isBipartite() const;
    Graph getMinimumSpanningTree() const;
    std::vector<std::string> findEulerianPath() const;
    bool hasEulerianCircuit() const;

    // Centrality measures
    std::vector<std::string> getMostConnectedNodes(int count = 1) const;
    std::string getMostCentralNode() const;
    double calculateBetweennessCentrality(const std::string& node) const;

    // Graph operations
    Graph getSubgraph(const std::vector<std::string>& nodes) const;
    Graph getTranspose() const;

    // Display functions
    void displayGraph() const;
    void displayStats() const;
    void printAdjacencyMatrix() const;
};

#endif // GRAPH_H