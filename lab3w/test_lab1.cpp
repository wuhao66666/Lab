#include <gtest/gtest.h>
#include "Graph.h"

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        graph.generateGraph("input.txt");
    }

    void TearDown() override {
        // 清理工作
    }

    Graph graph;
};

TEST_F(GraphTest, QueryBridgeWords_Test1) {
    std::vector<std::string> expectedBridgeWords = {"team"};
    std::vector<std::string> actualBridgeWords = graph.queryBridgeWords("the", "requested");

    std::cout << "Test 1: Query bridge words between 'the' and 'requested':" << std::endl;
    if (actualBridgeWords.empty()) {
        std::cout << "No bridge words found between 'the' and 'requested'." << std::endl;
    } else {
        std::cout << "Bridge words between 'the' and 'requested': ";
        for (const auto& word : actualBridgeWords) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_EQ(expectedBridgeWords, actualBridgeWords);
}

TEST_F(GraphTest, QueryBridgeWords_Test2) {
    std::vector<std::string> expectedBridgeWords = {};
    std::vector<std::string> actualBridgeWords = graph.queryBridgeWords("the", "more");

    std::cout << "Test 2: Query bridge words between 'the' and 'more':" << std::endl;
    if (actualBridgeWords.empty()) {
        std::cout << "No bridge words found between 'the' and 'more'." << std::endl;
    } else {
        std::cout << "Bridge words between 'the' and 'more': ";
        for (const auto& word : actualBridgeWords) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_EQ(expectedBridgeWords, actualBridgeWords);
}

TEST_F(GraphTest, QueryBridgeWords_Test3) {
    std::vector<std::string> expectedBridgeWords = {};
    std::vector<std::string> actualBridgeWords = graph.queryBridgeWords("nonono", "requested");

    std::cout << "Test 3: Query bridge words between 'nonono' and 'requested':" << std::endl;
    if (actualBridgeWords.empty()) {
        std::cout << "No bridge words found between 'nonono' and 'requested'." << std::endl;
    } else {
        std::cout << "Bridge words between 'nonono' and 'requested': ";
        for (const auto& word : actualBridgeWords) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_EQ(expectedBridgeWords, actualBridgeWords);
}

TEST_F(GraphTest, QueryBridgeWords_Test4) {
    std::vector<std::string> expectedBridgeWords = {};
    std::vector<std::string> actualBridgeWords = graph.queryBridgeWords("the", "nonono");

    std::cout << "Test 4: Query bridge words between 'the' and 'nononono':" << std::endl;
    if (actualBridgeWords.empty()) {
        std::cout << "No bridge words found between 'the' and 'nonono'." << std::endl;
    } else {
        std::cout << "Bridge words between 'the' and 'nonono': ";
        for (const auto& word : actualBridgeWords) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_EQ(expectedBridgeWords, actualBridgeWords);
}

TEST_F(GraphTest, QueryBridgeWords_Test5) {
    std::vector<std::string> expectedBridgeWords = {};
    std::vector<std::string> actualBridgeWords = graph.queryBridgeWords("", "wrote");

    std::cout << "Test 5: Query bridge words between '' and 'wrote':" << std::endl;
    if (actualBridgeWords.empty()) {
        std::cout << "No bridge words found between '' and 'wrote'." << std::endl;
    } else {
        std::cout << "Bridge words between '' and 'wrote': ";
        for (const auto& word : actualBridgeWords) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_EQ(expectedBridgeWords, actualBridgeWords);
}

TEST_F(GraphTest, QueryBridgeWords_Test6) {
    std::vector<std::string> expectedBridgeWords = {};
    std::vector<std::string> actualBridgeWords = graph.queryBridgeWords("the", "the");

    std::cout << "Test 6: Query bridge words between 'the' and 'the':" << std::endl;
    if (actualBridgeWords.empty()) {
        std::cout << "No bridge words found between 'the' and 'the'." << std::endl;
    } else {
        std::cout << "Bridge words between 'the' and 'the': ";
        for (const auto& word : actualBridgeWords) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_EQ(expectedBridgeWords, actualBridgeWords);
}

int RunAllTests(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
    return RunAllTests(argc, argv);
}