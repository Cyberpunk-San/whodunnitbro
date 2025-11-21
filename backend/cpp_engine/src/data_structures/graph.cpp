#include "graph.h"
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>
#include <set>
#include <iostream>
#include <unordered_map>

// Constructor
Graph::Graph() {}

// Destructor
Graph::~Graph() {}

// Add a node
void Graph::addNode(const std::string& node) {
    if (adjList.find(node) == adjList.end()) {
        adjList[node] = std::vector<std::string>();
    }
}

// Add a directed edge with optional weight
void Graph::addEdge(const std::string& from, const std::string& to, int weight) {
    addNode(from);
    addNode(to);
    
    // Add to adjacency list if not already present
    if (std::find(adjList[from].begin(), adjList[from].end(), to) == adjList[from].end()) {
        adjList[from].push_back(to);
    }
    
    // Set weight
    weights[from][to] = weight;
}

// Remove an edge
void Graph::removeEdge(const std::string& from, const std::string& to) {
    if (adjList.find(from) != adjList.end()) {
        auto& neighbors = adjList[from];
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), to), neighbors.end());
        
        // Remove weight
        if (weights.find(from) != weights.end() && weights[from].find(to) != weights[from].end()) {
            weights[from].erase(to);
        }
    }
}

// Remove a node and all its edges
void Graph::removeNode(const std::string& node) {
    // Remove node from adjacency list
    adjList.erase(node);
    
    // Remove node from weights
    weights.erase(node);
    
    // Remove all edges pointing to this node
    for (auto& pair : adjList) {
        auto& neighbors = pair.second;
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), node), neighbors.end());
        
        // Remove weights for edges to this node
        if (weights.find(pair.first) != weights.end()) {
            weights[pair.first].erase(node);
        }
    }
}

// Clear 
void Graph::clear() {
    adjList.clear();
    weights.clear();
}

// checking if node exists
bool Graph::hasNode(const std::string& node) const {
    return adjList.find(node) != adjList.end();
}

// checking if edge exists
bool Graph::hasEdge(const std::string& from, const std::string& to) const {
    if (adjList.find(from) == adjList.end()) return false;
    const auto& neighbors = adjList.at(from);
    return std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
}

// edge weight
int Graph::getEdgeWeight(const std::string& from, const std::string& to) const {
    if (weights.find(from) != weights.end() && 
        weights.at(from).find(to) != weights.at(from).end()) {
        return weights.at(from).at(to);
    }
    return 1; 
}

// edge weight
void Graph::setEdgeWeight(const std::string& from, const std::string& to, int weight) {
    if (hasEdge(from, to)) {
        weights[from][to] = weight;
    }
}

// neighbors of a node
std::vector<std::string> Graph::getNeighbors(const std::string& node) const {
    if (adjList.find(node) == adjList.end()) return {};
    return adjList.at(node);
}

// all nodes
std::vector<std::string> Graph::getAllNodes() const {
    std::vector<std::string> nodes;
    for (const auto& pair : adjList) {
        nodes.push_back(pair.first);
    }
    return nodes;
}

// all edges
std::vector<std::pair<std::string, std::string>> Graph::getAllEdges() const {
    std::vector<std::pair<std::string, std::string>> edges;
    for (const auto& pair : adjList) {
        for (const auto& neighbor : pair.second) {
            edges.push_back({pair.first, neighbor});
        }
    }
    return edges;
}

// node degree
int Graph::getNodeDegree(const std::string& node) const {
    return getOutDegree(node) + getInDegree(node);
}

// out-degree
int Graph::getOutDegree(const std::string& node) const {
    if (adjList.find(node) == adjList.end()) return 0;
    return adjList.at(node).size();
}

// in-degree
int Graph::getInDegree(const std::string& node) const {
    int count = 0;
    for (const auto& pair : adjList) {
        for (const auto& neighbor : pair.second) {
            if (neighbor == node) count++;
        }
    }
    return count;
}

// node count
int Graph::getNodeCount() const {
    return adjList.size();
}

// edge count
int Graph::getEdgeCount() const {
    int count = 0;
    for (const auto& pair : adjList) {
        count += pair.second.size();
    }
    return count;
}

// graph is empty
bool Graph::isEmpty() const {
    return adjList.empty();
}

// bfs
void Graph::bfs(const std::string& start, std::function<void(const std::string&)> visit) const {
    if (!hasNode(start)) return;
    
    std::unordered_map<std::string, bool> visited;
    std::queue<std::string> q;
    
    visited[start] = true;
    q.push(start);
    
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        
        visit(current);
        
        for (const auto& neighbor : getNeighbors(current)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// DFS
void Graph::dfs(const std::string& start, std::function<void(const std::string&)> visit) const {
    if (!hasNode(start)) return;
    
    std::unordered_map<std::string, bool> visited;
    dfsHelper(start, visited, visit);
}

void Graph::dfsHelper(const std::string& node, std::unordered_map<std::string, bool>& visited, 
                    std::function<void(const std::string&)> visit) const {
    visited[node] = true;
    visit(node);
    
    for (const auto& neighbor : getNeighbors(node)) {
        if (!visited[neighbor]) {
            dfsHelper(neighbor, visited, visit);
        }
    }
}

// shortest path (BFS )
std::vector<std::string> Graph::shortestPath(const std::string& start, const std::string& end) const {
    if (!hasNode(start) || !hasNode(end)) return {};
    
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, std::string> parent;
    std::queue<std::string> q;
    
    visited[start] = true;
    q.push(start);
    parent[start] = "";
    
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        
        if (current == end) {
            // reconstruct path
            std::vector<std::string> path;
            std::string node = end;
            while (!node.empty()) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        for (const auto& neighbor : getNeighbors(current)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
    
    return {}; 
}

// all paths between two nodes
std::vector<std::vector<std::string>> Graph::findAllPaths(const std::string& start, const std::string& end) const {
    std::vector<std::vector<std::string>> allPaths;
    if (!hasNode(start) || !hasNode(end)) return allPaths;
    
    std::unordered_map<std::string, bool> visited;
    std::vector<std::string> path;
    findPathsHelper(start, end, visited, path, allPaths);
    return allPaths;
}

void Graph::findPathsHelper(const std::string& current, const std::string& end,
                        std::unordered_map<std::string, bool>& visited,
                        std::vector<std::string>& path, std::vector<std::vector<std::string>>& allPaths) const {
    visited[current] = true;
    path.push_back(current);
    
    if (current == end) {
        allPaths.push_back(path);
    } else {
        for (const auto& neighbor : getNeighbors(current)) {
            if (!visited[neighbor]) {
                findPathsHelper(neighbor, end, visited, path, allPaths);
            }
        }
    }
    
    path.pop_back();
    visited[current] = false;
}

// shortest path length
int Graph::shortestPathLength(const std::string& start, const std::string& end) const {
    auto path = shortestPath(start, end);
    return path.empty() ? -1 : path.size() - 1;
}

// checking if graph is connected
bool Graph::isConnected() const {
    if (isEmpty()) return true;
    
    auto nodes = getAllNodes();
    if (nodes.empty()) return true;
    
    std::unordered_map<std::string, bool> visited;
    dfsHelper(nodes[0], visited, [](const std::string&) {});
    
    return visited.size() == nodes.size();
}

// checking if graph has cycle
bool Graph::hasCycle() const {
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, bool> recStack;
    
    for (const auto& node : getAllNodes()) {
        if (!visited[node]) {
            if (hasCycleHelper(node, visited, recStack)) {
                return true;
            }
        }
    }
    return false;
}

bool Graph::hasCycleHelper(const std::string& node, std::unordered_map<std::string, bool>& visited,
                        std::unordered_map<std::string, bool>& recStack) const {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;
        
        for (const auto& neighbor : getNeighbors(node)) {
            if (!visited[neighbor] && hasCycleHelper(neighbor, visited, recStack)) {
                return true;
            } else if (recStack[neighbor]) {
                return true;
            }
        }
    }
    
    recStack[node] = false;
    return false;
}

// Topological sort
std::vector<std::string> Graph::topologicalSort() const {
    std::vector<std::string> result;
    if (hasCycle()) return result; // Only for DAGs
    
    std::unordered_map<std::string, bool> visited;
    std::stack<std::string> stack;
    
    for (const auto& node : getAllNodes()) {
        if (!visited[node]) {
            // DFS for topological sort
            std::function<void(const std::string&)> dfsTopo = [&](const std::string& n) {
                visited[n] = true;
                for (const auto& neighbor : getNeighbors(n)) {
                    if (!visited[neighbor]) {
                        dfsTopo(neighbor);
                    }
                }
                stack.push(n);
            };
            dfsTopo(node);
        }
    }
    
    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }
    
    return result;
}

// connected components
std::vector<std::vector<std::string>> Graph::findConnectedComponents() const {
    std::vector<std::vector<std::string>> components;
    std::unordered_map<std::string, bool> visited;
    
    for (const auto& node : getAllNodes()) {
        if (!visited[node]) {
            std::vector<std::string> component;
            dfsHelper(node, visited, [&](const std::string& n) {
                component.push_back(n);
            });
            components.push_back(component);
        }
    }
    
    return components;
}

// articulation points
std::vector<std::string> Graph::findArticulationPoints() const {
    std::vector<std::string> ap;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, int> disc;
    std::unordered_map<std::string, int> low;
    std::unordered_map<std::string, std::string> parent;
    int time = 0;
    
    for (const auto& node : getAllNodes()) {
        if (!visited[node]) {
            articulationPointsHelper(node, visited, disc, low, parent, ap, time);
        }
    }
    
    return ap;
}

void Graph::articulationPointsHelper(const std::string& u, std::unordered_map<std::string, bool>& visited,
                                    std::unordered_map<std::string, int>& disc,
                                    std::unordered_map<std::string, int>& low,
                                    std::unordered_map<std::string, std::string>& parent,
                                    std::vector<std::string>& ap, int& time) const {
    int children = 0;
    
    visited[u] = true;
    disc[u] = low[u] = ++time;
    
    for (const auto& v : getNeighbors(u)) {
        if (!visited[v]) {
            children++;
            parent[v] = u;
            articulationPointsHelper(v, visited, disc, low, parent, ap, time);
            
            low[u] = std::min(low[u], low[v]);
            
            // u is articulation point if:
            // 1. It's root and has 2+ children
            // 2. It's not root and low[v] >= disc[u]
            if (parent[u].empty() && children > 1) {
                if (std::find(ap.begin(), ap.end(), u) == ap.end()) {
                    ap.push_back(u);
                }
            }
            if (!parent[u].empty() && low[v] >= disc[u]) {
                if (std::find(ap.begin(), ap.end(), u) == ap.end()) {
                    ap.push_back(u);
                }
            }
        } else if (v != parent[u]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

// bridges
std::vector<std::pair<std::string, std::string>> Graph::findBridges() const {
    std::vector<std::pair<std::string, std::string>> bridges;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, int> disc;
    std::unordered_map<std::string, int> low;
    std::unordered_map<std::string, std::string> parent;
    int time = 0;
    
    for (const auto& node : getAllNodes()) {
        if (!visited[node]) {
            bridgesHelper(node, visited, disc, low, parent, bridges, time);
        }
    }
    
    return bridges;
}

void Graph::bridgesHelper(const std::string& u, std::unordered_map<std::string, bool>& visited,
                        std::unordered_map<std::string, int>& disc,
                        std::unordered_map<std::string, int>& low,
                        std::unordered_map<std::string, std::string>& parent,
                        std::vector<std::pair<std::string, std::string>>& bridges, int& time) const {
    
    visited[u] = true;
    disc[u] = low[u] = ++time;
    
    for (const auto& v : getNeighbors(u)) {
        if (!visited[v]) {
            parent[v] = u;
            bridgesHelper(v, visited, disc, low, parent, bridges, time);
            
            low[u] = std::min(low[u], low[v]);
            
            // If the lowest vertex reachable from subtree under v is below u in DFS tree,
            // then u-v is a bridge
            if (low[v] > disc[u]) {
                bridges.push_back({u, v});
            }
        } else if (v != parent[u]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

// most connected nodes
std::vector<std::string> Graph::getMostConnectedNodes(int count) const {
    std::vector<std::pair<std::string, int>> nodeDegrees;
    
    for (const auto& node : getAllNodes()) {
        nodeDegrees.push_back({node, getNodeDegree(node)});
    }
    
    std::sort(nodeDegrees.begin(), nodeDegrees.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> result;
    for (int i = 0; i < std::min(count, static_cast<int>(nodeDegrees.size())); i++) {
        result.push_back(nodeDegrees[i].first);
    }
    
    return result;
}

// Get most central node 
std::string Graph::getMostCentralNode() const {
    auto mostConnected = getMostConnectedNodes(1);
    return mostConnected.empty() ? "" : mostConnected[0];
}

// Calculate betweenness centrality 
double Graph::calculateBetweennessCentrality(const std::string& node) const {
    int totalPaths = 0;
    int pathsThroughNode = 0;
    
    auto nodes = getAllNodes();
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            auto path = shortestPath(nodes[i], nodes[j]);
            if (!path.empty()) {
                totalPaths++;
                if (std::find(path.begin(), path.end(), node) != path.end()) {
                    pathsThroughNode++;
                }
            }
        }
    }
    
    return totalPaths == 0 ? 0 : static_cast<double>(pathsThroughNode) / totalPaths;
}

// Get subgraph
Graph Graph::getSubgraph(const std::vector<std::string>& nodes) const {
    Graph subgraph;
    
    for (const auto& node : nodes) {
        if (hasNode(node)) {
            subgraph.addNode(node);
            for (const auto& neighbor : getNeighbors(node)) {
                if (std::find(nodes.begin(), nodes.end(), neighbor) != nodes.end()) {
                    subgraph.addEdge(node, neighbor, getEdgeWeight(node, neighbor));
                }
            }
        }
    }
    
    return subgraph;
}

// Get transpose (reverse all edges)
Graph Graph::getTranspose() const {
    Graph transpose;
    
    for (const auto& pair : adjList) {
        for (const auto& neighbor : pair.second) {
            transpose.addEdge(neighbor, pair.first, getEdgeWeight(pair.first, neighbor));
        }
    }
    
    return transpose;
}

// Display graph
void Graph::displayGraph() const {
    std::cout << "Graph:\n";
    for (const auto& pair : adjList) {
        std::cout << pair.first << " -> ";
        for (const auto& neighbor : pair.second) {
            std::cout << neighbor;
            if (weights.find(pair.first) != weights.end() && 
                weights.at(pair.first).find(neighbor) != weights.at(pair.first).end()) {
                std::cout << "(" << weights.at(pair.first).at(neighbor) << ")";
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}

// Display statistics
void Graph::displayStats() const {
    std::cout << "Graph Statistics:\n";
    std::cout << "Nodes: " << getNodeCount() << "\n";
    std::cout << "Edges: " << getEdgeCount() << "\n";
    std::cout << "Density: " << (getNodeCount() > 1 ? 
        (2.0 * getEdgeCount()) / (getNodeCount() * (getNodeCount() - 1)) : 0) << "\n";
    std::cout << "Connected: " << (isConnected() ? "Yes" : "No") << "\n";
    std::cout << "Has Cycle: " << (hasCycle() ? "Yes" : "No") << "\n";
    
    auto mostConnected = getMostConnectedNodes(1);
    if (!mostConnected.empty()) {
        std::cout << "Most Connected Node: " << mostConnected[0] 
                << " (degree: " << getNodeDegree(mostConnected[0]) << ")\n";
    }
}

//adjacency matrix
void Graph::printAdjacencyMatrix() const {
    auto nodes = getAllNodes();
    std::sort(nodes.begin(), nodes.end());
    
    std::cout << "Adjacency Matrix:\n  ";
    for (const auto& node : nodes) {
        std::cout << node << " ";
    }
    std::cout << "\n";
    
    for (const auto& rowNode : nodes) {
        std::cout << rowNode << " ";
        for (const auto& colNode : nodes) {
            std::cout << (hasEdge(rowNode, colNode) ? "1 " : "0 ");
        }
        std::cout << "\n";
    }
}

// checking if graph is bipartite
bool Graph::isBipartite() const {
    if (isEmpty()) return true;
    
    std::unordered_map<std::string, int> color;
    for (const auto& node : getAllNodes()) {
        color[node] = -1;
    }
    
    for (const auto& node : getAllNodes()) {
        if (color[node] == -1) {
            std::queue<std::string> q;
            q.push(node);
            color[node] = 0;
            
            while (!q.empty()) {
                std::string current = q.front();
                q.pop();
                
                for (const auto& neighbor : getNeighbors(current)) {
                    if (color[neighbor] == -1) {
                        color[neighbor] = 1 - color[current];
                        q.push(neighbor);
                    } else if (color[neighbor] == color[current]) {
                        return false;
                    }
                }
            }
        }
    }
    
    return true;
}

// minimum spanning tree (Prim's algorithm)
Graph Graph::getMinimumSpanningTree() const {
    Graph mst;
    if (isEmpty()) return mst;
    
    auto nodes = getAllNodes();
    std::unordered_map<std::string, bool> inMST;
    std::unordered_map<std::string, int> key;
    std::unordered_map<std::string, std::string> parent;
    
    for (const auto& node : nodes) {
        key[node] = std::numeric_limits<int>::max();
        inMST[node] = false;
    }
    
    key[nodes[0]] = 0;
    parent[nodes[0]] = "";
    
    for (size_t i = 0; i < nodes.size() - 1; i++) {
        // min key vertex not in MST
        std::string u;
        int minKey = std::numeric_limits<int>::max();
        for (const auto& node : nodes) {
            if (!inMST[node] && key[node] < minKey) {
                minKey = key[node];
                u = node;
            }
        }
        
        inMST[u] = true;
        
        // adding edge to MST
        if (!parent[u].empty()) {
            mst.addEdge(parent[u], u, getEdgeWeight(parent[u], u));
            mst.addEdge(u, parent[u], getEdgeWeight(u, parent[u]));
        }
        
        
        for (const auto& v : getNeighbors(u)) {
            int weight = getEdgeWeight(u, v);
            if (!inMST[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
        }
    }
    
    return mst;
}

//Eulerian path 
std::vector<std::string> Graph::findEulerianPath() const {
    std::vector<std::string> path;
    if (isEmpty()) return path;
    
    // Eulerian path exists 
    int oddDegreeCount = 0;
    for (const auto& node : getAllNodes()) {
        if (getNodeDegree(node) % 2 != 0) {
            oddDegreeCount++;
        }
    }
    
    if (oddDegreeCount != 0 && oddDegreeCount != 2) {
        return path; // No Eulerian path
    }
    
    // DFS-based approach 
    auto nodes = getAllNodes();
    path.push_back(nodes[0]);
    
    
    return path;
}

// graph has Eulerian circuit
bool Graph::hasEulerianCircuit() const {
    if (!isConnected()) return false;
    
    // vertices must have even degree
    for (const auto& node : getAllNodes()) {
        if (getNodeDegree(node) % 2 != 0) {
            return false;
        }
    }
    
    return true;
}

// Main function to test Graph
/*
int main() {
    std::cout << "=== Testing Graph Data Structure ===\n";
    
    Graph g;
    
    // Test adding nodes and edges
    g.addEdge("A", "B", 5);
    g.addEdge("A", "C", 3);
    g.addEdge("B", "C", 2);
    g.addEdge("B", "D", 4);
    g.addEdge("C", "D", 1);
    g.addEdge("D", "E", 6);
    
    // Display graph
    g.displayGraph();
    std::cout << "\n";
    
    // Display statistics
    g.displayStats();
    std::cout << "\n";
    
    // Test BFS
    std::cout << "BFS starting from A: ";
    g.bfs("A", [](const std::string& node) {
        std::cout << node << " ";
    });
    std::cout << "\n\n";
    
    // Test DFS
    std::cout << "DFS starting from A: ";
    g.dfs("A", [](const std::string& node) {
        std::cout << node << " ";
    });
    std::cout << "\n\n";
    
    // Test shortest path
    auto path = g.shortestPath("A", "E");
    std::cout << "Shortest path from A to E: ";
    for (const auto& node : path) {
        std::cout << node << " ";
    }
    std::cout << "\n\n";
    
    // Test connected components
    auto components = g.findConnectedComponents();
    std::cout << "Connected components: " << components.size() << "\n";
    for (size_t i = 0; i < components.size(); ++i) {
        std::cout << "Component " << i + 1 << ": ";
        for (const auto& node : components[i]) {
            std::cout << node << " ";
        }
        std::cout << "\n";
    }
    
    return 0;
}
    */