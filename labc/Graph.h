#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <cmath>
#include <random>
#include <cctype>
#include <iomanip>

class Graph {
public:
    Graph() = default;
    ~Graph() = default;

    void generateGraph(const std::string& filePath);
    void showDirectedGraph() const;
    std::vector<std::string> queryBridgeWords(const std::string& word1, const std::string& word2) const;
    std::string generateNewText(const std::string& inputText) const;
    std::string calcShortestPath(const std::string& word1, const std::string& word2) const;
    void calculatePageRank(double d = 0.85);
    double calcPageRank(const std::string& word) const;
    std::string randomWalk() const;
    void exportGraphvizCode(const std::string& outputFilePath) const;

private:
    struct Node {
        std::unordered_map<std::string, int> edges;
        double pageRank = 1.0;
    };

    std::unordered_map<std::string, Node> nodes;

    void addEdge(const std::string& from, const std::string& to);
    std::string removePunctuation(const std::string& str) const;
};

#endif // GRAPH_H
