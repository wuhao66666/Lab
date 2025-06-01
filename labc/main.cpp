#include "Graph.h"

int main() {
    Graph graph;
    std::string filePath;

    std::cout << "请输入文本文件路径: ";
    std::cin >> filePath;

    graph.generateGraph(filePath);
    graph.calculatePageRank(0.85);

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