#include <gtest/gtest.h>
#include "StringDataSource.h"
#include <vector>

// -------------------- EndTest --------------------
TEST(StringDataSource, EndTest) {
    CStringDataSource emptySource("");
    CStringDataSource baseSource("Hello");

    EXPECT_TRUE(emptySource.End());
    EXPECT_FALSE(baseSource.End());
}

// -------------------- PeekTest --------------------
TEST(StringDataSource, PeekTest) {
    CStringDataSource emptySource("");
    CStringDataSource source1("Hello");
    CStringDataSource source2("Bye");

    char ch = 'x';

    // Empty source should fail
    EXPECT_FALSE(emptySource.Peek(ch));
    EXPECT_EQ(ch, 'x');

    // Non-empty sources
    EXPECT_TRUE(source1.Peek(ch));
    EXPECT_EQ(ch, 'H');

    EXPECT_TRUE(source2.Peek(ch));
    EXPECT_EQ(ch, 'B');
}

// -------------------- GetTest --------------------
TEST(StringDataSource, GetTest) {
    CStringDataSource emptySource("");
    CStringDataSource source1("Hello");
    CStringDataSource source2("Bye");

    char ch = 'x';

    // Empty source
    EXPECT_FALSE(emptySource.Get(ch));
    EXPECT_EQ(ch, 'x');

    // Get from source1
    EXPECT_TRUE(source1.Get(ch));
    EXPECT_EQ(ch, 'H');
    EXPECT_TRUE(source1.Peek(ch));
    EXPECT_EQ(ch, 'e');

    // Get from source2
    EXPECT_TRUE(source2.Get(ch));
    EXPECT_EQ(ch, 'B');
    EXPECT_TRUE(source2.Peek(ch));
    EXPECT_EQ(ch, 'y');
}

// -------------------- ReadTest --------------------
TEST(StringDataSource, ReadTest) {
    CStringDataSource emptySource("");
    CStringDataSource source1("Hello");
    CStringDataSource source2("Bye");

    std::vector<char> tempVector;
    char ch = 'x';

    // Empty source read
    EXPECT_FALSE(emptySource.Read(tempVector, 3));
    EXPECT_EQ(tempVector.size(), 0);

    // Read first 4 chars from source1
    tempVector.clear();
    EXPECT_TRUE(source1.Read(tempVector, 4));
    ASSERT_EQ(tempVector.size(), 4);
    EXPECT_EQ(tempVector[0], 'H');
    EXPECT_EQ(tempVector[1], 'e');
    EXPECT_EQ(tempVector[2], 'l');
    EXPECT_EQ(tempVector[3], 'l');

    // Next char should be 'o'
    EXPECT_TRUE(source1.Peek(ch));
    EXPECT_EQ(ch, 'o');

    // Read all chars from source2 ("Bye") with requested size 4
    tempVector.clear();
    EXPECT_TRUE(source2.Read(tempVector, 4));
    ASSERT_EQ(tempVector.size(), 3);
    EXPECT_EQ(tempVector[0], 'B');
    EXPECT_EQ(tempVector[1], 'y');
    EXPECT_EQ(tempVector[2], 'e');

    // Peek after end should fail
    ch = 'x';
    EXPECT_FALSE(source2.Peek(ch));
    EXPECT_EQ(ch, 'x');
}