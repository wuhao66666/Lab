#include "Graph.h"

void Graph::generateGraph(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return;
    }

    std::string line, word, prevWord;
    while (std::getline(file, line)) {
        line = removePunctuation(line);
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            if (token.empty()) {
                continue;
            }
            if (!prevWord.empty()) {
                addEdge(prevWord, token);
            }
            prevWord = token;
        }
        prevWord.clear();
    }
    file.close();
}

void Graph::addEdge(const std::string& from, const std::string& to) {
    if (nodes.find(from) == nodes.end()) {
        nodes[from] = Node();
    }
    if (nodes.find(to) == nodes.end()) {
        nodes[to] = Node();
    }
    nodes[from].edges[to]++;
}

void Graph::showDirectedGraph() const {
    for (const auto& node : nodes) {
        std::cout << node.first << ": ";
        for (const auto& edge : node.second.edges) {
            std::cout << edge.first << "(" << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> Graph::queryBridgeWords(const std::string& word1, const std::string& word2) const {
    std::vector<std::string> bridgeWords;
    if (nodes.find(word1) == nodes.end() || nodes.find(word2) == nodes.end()) {
        return bridgeWords;
    }
    for (const auto& edge1 : nodes.at(word1).edges) {
        if (nodes.at(edge1.first).edges.find(word2) != nodes.at(edge1.first).edges.end()) {
            bridgeWords.push_back(edge1.first);
        }
    }
    return bridgeWords;
}

std::string Graph::generateNewText(const std::string& inputText) const {
    std::istringstream iss(inputText);
    std::string word, prevWord, result;
    while (iss >> word) {
        word = removePunctuation(word);
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (word.empty()) {
            continue;
        }
        if (!prevWord.empty()) {
            auto bridgeWords = queryBridgeWords(prevWord, word);
            if (!bridgeWords.empty()) {
                result += " " + bridgeWords[0];
            }
        }
        result += " " + word;
        prevWord = word;
    }
    if (!result.empty() && result.front() == ' ') {
        result.erase(result.begin());
    }
    return result;
}

std::string Graph::calcShortestPath(const std::string& word1, const std::string& word2) const {
    if (nodes.find(word1) == nodes.end() || nodes.find(word2) == nodes.end()) {
        return "No path found";
    }

    std::unordered_map<std::string, double> distances;
    std::unordered_map<std::string, std::string> predecessors;
    std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std::greater<>> pq;

    for (const auto& node : nodes) {
        distances[node.first] = std::numeric_limits<double>::infinity();
    }
    distances[word1] = 0;
    pq.push({0, word1});

    while (!pq.empty()) {
        auto [currentDist, currentWord] = pq.top();
        pq.pop();

        if (currentWord == word2) break;

        for (const auto& [neighbor, weight] : nodes.at(currentWord).edges) {
            double newDist = currentDist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                predecessors[neighbor] = currentWord;
                pq.push({newDist, neighbor});
            }
        }
    }

    if (distances[word2] == std::numeric_limits<double>::infinity()) {
        return "No path found";
    }

    std::vector<std::string> path;
    for (std::string at = word2; at != word1; at = predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(word1);
    std::reverse(path.begin(), path.end());

    std::ostringstream oss;
    for (size_t i = 0; i < path.size(); ++i) {
        oss << path[i];
        if (i < path.size() - 1) {
            oss << " -> ";
        }
    }
    return oss.str();
}

void Graph::calculatePageRank(double d) {
    size_t numNodes = nodes.size();
    if (numNodes == 0) return;

    std::unordered_map<std::string, double> newPageRanks;
    std::unordered_map<std::string, double> oldPageRanks;

    for (const auto& node : nodes) {
        oldPageRanks[node.first] = node.second.pageRank;
    }

    bool converged = false;
    const double threshold = 1e-6;
    int iterations = 0;
    const int maxIterations = 100;

    while (!converged && iterations < maxIterations) {
        converged = true;
        iterations++;

        newPageRanks.clear();

        for (const auto& node : nodes) {
            newPageRanks[node.first] = (1 - d) / numNodes;
            for (const auto& neighbor : nodes) {
                if (neighbor.second.edges.find(node.first) != neighbor.second.edges.end()) {
                    newPageRanks[node.first] += d * oldPageRanks[neighbor.first] / neighbor.second.edges.size();
                }
            }
        }

        for (auto& node : nodes) {
            double delta = std::abs(newPageRanks[node.first] - oldPageRanks[node.first]);
            if (delta > threshold) {
                converged = false;
            }
            oldPageRanks[node.first] = newPageRanks[node.first];
            node.second.pageRank = newPageRanks[node.first];
        }
    }
}

double Graph::calcPageRank(const std::string& word) const {
    if (nodes.find(word) == nodes.end()) {
        return 0.0;
    }
    return nodes.at(word).pageRank;
}

std::string Graph::randomWalk() const {
    std::vector<std::string> allNodes;
    for (const auto& node : nodes) {
        allNodes.push_back(node.first);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, allNodes.size() - 1);

    std::string current = allNodes[dis(gen)];
    std::ostringstream oss;
    oss << current;

    while (true) {
        if (nodes.at(current).edges.empty()) {
            break;
        }
        std::vector<std::string> neighbors;
        for (const auto& edge : nodes.at(current).edges) {
            neighbors.push_back(edge.first);
        }
        current = neighbors[dis(gen) % neighbors.size()];
        oss << " -> " << current;
    }

    return oss.str();
}

std::string Graph::removePunctuation(const std::string& str) const {
    std::string result;
    for (char c : str) {
        if (!std::ispunct(c)) {
            result += c;
        }
    }
    return result;
}

void Graph::exportGraphvizCode(const std::string& outputFilePath) const {
    std::ofstream outFile(outputFilePath);
    if (!outFile.is_open()) {
        std::cerr << "无法打开输出文件: " << outputFilePath << std::endl;
        return;
    }

    outFile << "digraph G {\n";
    outFile << "    node [shape=box, fontname=\"Arial\", fontsize=12];\n";
    outFile << "    edge [fontname=\"Arial\", fontsize=10];\n\n";

    for (const auto& node : nodes) {
        outFile << "    " << node.first << ";\n";
    }

    outFile << "\n";

    for (const auto& node : nodes) {
        for (const auto& edge : node.second.edges) {
            outFile << "    " << node.first << " -> " << edge.first << " [label=\"" << edge.second << "\"];\n";
        }
    }

    outFile << "}\n";
    outFile.close();

    std::cout << "Graphviz代码已成功导出到文件: " << outputFilePath << std::endl;
}