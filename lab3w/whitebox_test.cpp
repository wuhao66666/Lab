#include <gtest/gtest.h>
#include "Graph.h" // 包含被测试的类

// 测试用例 1：两个单词之间存在路径
TEST(CalcShortestPathTest, ExistPath) {
    Graph graph;
    graph.generateGraph("input.txt"); // 使用 input.txt 文件

    // 假设在 input.txt 中，"scientist" 和 "team" 之间存在路径
    std::string path = graph.calcShortestPath("scientist", "team");
    std::cout << "ExistPath - 期望输出: scientist -> analyzed -> the -> team" << std::endl;
    EXPECT_NE(path, "No path found");
    EXPECT_EQ(path, "scientist -> analyzed -> the -> team");
}

// 测试用例 2：路径包含多个节点
TEST(CalcShortestPathTest, MultiNodePath) {
    Graph graph;
    graph.generateGraph("input.txt");

    // 假设在 input.txt 中，"scientist" 和 "requested" 之间存在多节点路径
    std::string path = graph.calcShortestPath("scientist", "requested");
    std::cout << "MultiNodePath - 期望输出: scientist -> analyzed -> the -> team -> requested" << std::endl;
    EXPECT_NE(path, "No path found");
    EXPECT_EQ(path, "scientist -> analyzed -> the -> team -> requested");
}

// 测试用例 3：输入的单词不存在于图中
TEST(CalcShortestPathTest, NonExistWord) {
    Graph graph;
    graph.generateGraph("input.txt");

    // 输入的单词 "unknown" 不存在于图中
    std::string path = graph.calcShortestPath("unknown", "word");
    std::cout << "NonExistWord - 期望输出: No path found" << std::endl;
    EXPECT_EQ(path, "No path found");
}

// 测试用例 4：输入的两个单词相同
TEST(CalcShortestPathTest, SameWords) {
    Graph graph;
    graph.generateGraph("input.txt");

    // 输入的两个单词相同
    std::string path = graph.calcShortestPath("scientist", "scientist");
    std::cout << "SameWords - 期望输出: scientist 或 Path length is zero" << std::endl;
    EXPECT_EQ(path, "scientist"); // 
    EXPECT_NE(path, "No path found");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}