#include <gtest/gtest.h>
#include "StreetMapIndexer.h"
#include "StreetMap.h"
#include <memory>
#include <unordered_set>

// -------------------- Helper: Create a simple street map --------------------
std::shared_ptr<StreetMap> CreateSimpleStreetMap() {
    auto map = std::make_shared<StreetMap>();

    // Create nodes
    auto node1 = std::make_shared<StreetMap::SNode>(1, 38.5, -121.7);
    auto node2 = std::make_shared<StreetMap::SNode>(2, 38.5, -121.8);
    map->AddNode(node1);
    map->AddNode(node2);

    // Create a way connecting the nodes
    auto way = std::make_shared<StreetMap::SWay>(100);
    way->AddNodeID(node1->ID());
    way->AddNodeID(node2->ID());
    map->AddWay(way);

    return map;
}

// -------------------- Tests --------------------
TEST(StreetMapIndexerTest, CreateDestroy) {
    auto map = CreateSimpleStreetMap();
    StreetMapIndexer indexer(map);
    SUCCEED(); // Just testing construction/destruction
}

TEST(StreetMapIndexerTest, NodeAndWayCounts) {
    auto map = CreateSimpleStreetMap();
    StreetMapIndexer indexer(map);

    EXPECT_EQ(indexer.NodeCount(), 2);
    EXPECT_EQ(indexer.WayCount(), 1);
}

TEST(StreetMapIndexerTest, SortedNodeByIndex) {
    auto map = CreateSimpleStreetMap();
    StreetMapIndexer indexer(map);

    auto node = indexer.SortedNodeByIndex(0);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->ID(), 1);

    node = indexer.SortedNodeByIndex(1);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->ID(), 2);

    // Out-of-bounds
    node = indexer.SortedNodeByIndex(2);
    EXPECT_EQ(node, nullptr);
}

TEST(StreetMapIndexerTest, SortedWayByIndex) {
    auto map = CreateSimpleStreetMap();
    StreetMapIndexer indexer(map);

    auto way = indexer.SortedWayByIndex(0);
    ASSERT_NE(way, nullptr);
    EXPECT_EQ(way->ID(), 100);

    // Out-of-bounds
    way = indexer.SortedWayByIndex(1);
    EXPECT_EQ(way, nullptr);
}

TEST(StreetMapIndexerTest, WaysByNodeID) {
    auto map = CreateSimpleStreetMap();
    StreetMapIndexer indexer(map);

    std::unordered_set<std::shared_ptr<StreetMap::SWay>> ways = indexer.WaysByNodeID(1);
    EXPECT_EQ(ways.size(), 1);

    ways = indexer.WaysByNodeID(2);
    EXPECT_EQ(ways.size(), 1);

    ways = indexer.WaysByNodeID(999); // Non-existent node
    EXPECT_EQ(ways.size(), 0);
}

TEST(StreetMapIndexerTest, WaysInRange) {
    auto map = CreateSimpleStreetMap();
    StreetMapIndexer indexer(map);

    StreetMap::SLocation lowerLeft{38.4, -121.8};
    StreetMap::SLocation upperRight{38.6, -121.6};

    std::unordered_set<std::shared_ptr<StreetMap::SWay>> ways = indexer.WaysInRange(lowerLeft, upperRight);
    EXPECT_EQ(ways.size(), 1);

    // Range with no ways
    StreetMap::SLocation emptyLower{0, 0};
    StreetMap::SLocation emptyUpper{1, 1};
    ways = indexer.WaysInRange(emptyLower, emptyUpper);
    EXPECT_EQ(ways.size(), 0);
}

// -------------------- Main --------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}