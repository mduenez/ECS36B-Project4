#include <gtest/gtest.h>
#include "StringDataSink.h"
#include <vector>

// -------------------- EmptyTest --------------------
TEST(StringDataSink, EmptyTest) {
    CStringDataSink sink;
    
    EXPECT_TRUE(sink.String().empty());
    EXPECT_EQ(sink.String(), "");
}

// -------------------- PutTest --------------------
TEST(StringDataSink, PutTest) {
    CStringDataSink sink;
    
    EXPECT_TRUE(sink.Put('H'));
    EXPECT_TRUE(sink.Put('e'));
    EXPECT_TRUE(sink.Put('l'));
    EXPECT_TRUE(sink.Put('l'));
    EXPECT_EQ(sink.String(), "Hell");

    EXPECT_TRUE(sink.Put('o'));
    EXPECT_EQ(sink.String(), "Hello");
}

// -------------------- WriteTest --------------------
TEST(StringDataSink, WriteTest) {
    std::vector<char> temp1 = {'H','e','l','l','o'};
    std::vector<char> temp2 = {' ','W','o','r','l','d'};
    CStringDataSink sink;

    // Write first vector
    EXPECT_TRUE(sink.Write(temp1));
    EXPECT_EQ(sink.String(), "Hello");

    // Write second vector
    EXPECT_TRUE(sink.Write(temp2));
    EXPECT_EQ(sink.String(), "Hello World");

    // Write empty vector (edge case)
    std::vector<char> emptyVec;
    EXPECT_TRUE(sink.Write(emptyVec));
    EXPECT_EQ(sink.String(), "Hello World"); // string should remain unchanged
}