#include <gtest/gtest.h>
#include <stdexcept>
#include "Vector.h"
#include "Matrix.h"
#include "Graph.h"
#include "Parser.h"
#include "Calculator.h"
#include "Util.h" // for file / string utilities

// -------------------- Vector Tests --------------------
TEST(VectorTest, EmptyVector) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(VectorTest, SingleElement) {
    Vector<int> v;
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.at(0), 42);

    v.set(0, 100);
    EXPECT_EQ(v.at(0), 100);
}

TEST(VectorTest, MultipleElements) {
    Vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);
    EXPECT_EQ(v.size(), 5);
    for (int i = 0; i < 5; ++i) EXPECT_EQ(v.at(i), i);
}

TEST(VectorTest, OutOfBoundsAccess) {
    Vector<int> v;
    v.push_back(10);
    EXPECT_THROW(v.at(1), std::out_of_range);
}

// -------------------- Matrix Tests --------------------
TEST(MatrixTest, EmptyMatrix) {
    Matrix<int> m;
    EXPECT_EQ(m.rows(), 0);
    EXPECT_EQ(m.cols(), 0);
}

TEST(MatrixTest, SingleElementMatrix) {
    Matrix<int> m(1,1);
    m.set(0,0,7);
    EXPECT_EQ(m.get(0,0), 7);
}

TEST(MatrixTest, MultipleElements) {
    Matrix<int> m(2,3);
    m.set(0,0,1);
    m.set(0,1,2);
    m.set(0,2,3);
    m.set(1,0,4);
    m.set(1,1,5);
    m.set(1,2,6);
    EXPECT_EQ(m.get(1,2), 6);
}

TEST(MatrixTest, InvalidOperations) {
    Matrix<int> m1(1,2), m2(2,1);
    EXPECT_THROW(m1 + m2, std::invalid_argument);
    EXPECT_THROW(m1.get(10,10), std::out_of_range);
}

// -------------------- Graph Tests --------------------
TEST(GraphTest, EmptyGraph) {
    Graph g;
    EXPECT_EQ(g.size(), 0);
}

TEST(GraphTest, SingleNodeGraph) {
    Graph g;
    g.addNode(1);
    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.hasNode(1));
}

TEST(GraphTest, DisconnectedGraph) {
    Graph g;
    g.addNode(1);
    g.addNode(2);
    EXPECT_FALSE(g.hasEdge(1,2));
}

TEST(GraphTest, AddEdgeAndTraverse) {
    Graph g;
    g.addNode(1); g.addNode(2); g.addEdge(1,2);
    EXPECT_TRUE(g.hasEdge(1,2));
    EXPECT_TRUE(g.isConnected(1,2));
}

// -------------------- Parser Tests --------------------
TEST(ParserTest, EmptyInput) {
    Parser p;
    EXPECT_THROW(p.parse(""), std::runtime_error);
}

TEST(ParserTest, InvalidInput) {
    Parser p;
    EXPECT_THROW(p.parse("invalid data"), std::runtime_error);
}

TEST(ParserTest, ValidInput) {
    Parser p;
    EXPECT_NO_THROW(p.parse("1 + 2"));
}

// -------------------- Calculator Tests --------------------
TEST(CalculatorTest, DivisionByZero) {
    Calculator calc;
    EXPECT_THROW(calc.divide(10,0), std::runtime_error);
}

TEST(CalculatorTest, BasicOperations) {
    Calculator calc;
    EXPECT_EQ(calc.add(2,3), 5);
    EXPECT_EQ(calc.subtract(5,3), 2);
    EXPECT_EQ(calc.multiply(4,3), 12);
    EXPECT_EQ(calc.divide(10,2), 5);
}

TEST(CalculatorTest, NegativeInput) {
    Calculator calc;
    EXPECT_EQ(calc.abs(-5), 5);
    EXPECT_THROW(calc.sqrt(-1), std::domain_error);
}

TEST(CalculatorTest, ExtremeValues) {
    Calculator calc;
    EXPECT_NO_THROW(calc.multiply(1000000, 1000000));
    EXPECT_NO_THROW(calc.divide(1e9, 1e-3));
}

// -------------------- Utility / Helper Functions --------------------
TEST(UtilTest, StringToNumberInvalid) {
    EXPECT_THROW(Util::StringToNumber("abc"), std::invalid_argument);
    EXPECT_NO_THROW(Util::StringToNumber("123"));
}

TEST(UtilTest, FileReadNonexistent) {
    EXPECT_THROW(Util::ReadFile("does_not_exist.txt"), std::runtime_error);
}

TEST(UtilTest, FileReadEmpty) {
    // Create an empty temp file
    std::ofstream ofs("empty.txt"); ofs.close();
    auto content = Util::ReadFile("empty.txt");
    EXPECT_TRUE(content.empty());
    std::remove("empty.txt");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}