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

// 定义图结构
class Graph {
public:
    Graph() = default;
    ~Graph() = default;
    
    void generateGraph(const std::string& filePath);
    void showDirectedGraph() const;
    std::vector<std::string> queryBridgeWords(const std::string& word1, const std::string& word2) const;
    std::string generateNewText(const std::string& inputText) const;
    std::string calcShortestPath(const std::string& word1, const std::string& word2) const;
    double calcPageRank(const std::string& word) const;
    std::string randomWalk() const;
    void exportGraphvizCode(const std::string& outputFilePath) const;
    void calculatePageRank(double d = 0.85); // 将 calculatePageRank 函数声明为 public 成员函数
    
private:
    struct Node {
        std::unordered_map<std::string, int> edges;
        double pageRank = 1.0;
    };
    
    std::unordered_map<std::string, Node> nodes;
    
    void addEdge(const std::string& from, const std::string& to);
    std::string removePunctuation(const std::string& str) const;
};

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
                result += " " + bridgeWords[0]; // 直接选择第一个桥接词
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
            newPageRanks[node.first] = (1 - d) / numNodes; // 随机跳转部分
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

int main() {
    Graph graph;
    std::string filePath;

    std::cout << "请输入文本文件路径: ";
    std::cin >> filePath;

    graph.generateGraph(filePath);
    graph.calculatePageRank(0.85); // 计算PageRank，阻尼系数为0.85

    while (true) {
        std::cout << "\n请选择功能：\n";
        std::cout << "1. 展示有向图\n";
        std::cout << "2. 查询桥接词\n";
        std::cout << "3. 根据桥接词生成新文本\n";
        std::cout << "4. 计算最短路径\n";
        std::cout << "5. 计算PageRank\n";
        std::cout << "6. 随机游走\n";
        std::cout << "7. 导出Graphviz代码到文件\n";
        std::cout << "0. 退出\n";

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break;
        }

        switch (choice) {
            case 1:
                graph.showDirectedGraph();
                break;
            case 2: {
                std::string word1, word2;
                std::cout << "请输入两个单词（用空格分隔）: ";
                std::cin >> word1 >> word2;
                auto bridgeWords = graph.queryBridgeWords(word1, word2);
                if (bridgeWords.empty()) {
                    std::cout << "No bridge words from " << word1 << " to " << word2 << "!\n";
                } else {
                    std::cout << "The bridge words from " << word1 << " to " << word2 << " are: ";
                    for (const auto& word : bridgeWords) {
                        std::cout << word << " ";
                    }
                    std::cout << std::endl;
                }
                break;
            }
            case 3: {
                std::string inputText;
                std::cout << "请输入一行新文本: ";
                std::cin.ignore();
                std::getline(std::cin, inputText);
                std::cout << "生成的新文本: " << graph.generateNewText(inputText) << std::endl;
                break;
            }
            case 4: {
                std::string word1, word2;
                std::cout << "请输入两个单词（用空格分隔）: ";
                std::cin >> word1 >> word2;
                std::cout << "最短路径: " << graph.calcShortestPath(word1, word2) << std::endl;
                break;
            }
            case 5: {
                std::string word;
                std::cout << "请输入一个单词: ";
                std::cin >> word;
                std::cout << "PageRank of " << word << ": " << graph.calcPageRank(word) << std::endl;
                break;
            }
            case 6:
                std::cout << "随机游走路径: " << graph.randomWalk() << std::endl;
                break;
            case 7: {
                std::string outputFilePath;
                std::cout << "请输入输出文件路径（例如：output.dot）: ";
                std::cin >> outputFilePath;
                graph.exportGraphvizCode(outputFilePath);
                break;
            }
            default:
                std::cout << "无效的选择，请重新输入。\n";
        }
    }

    return 0;
}