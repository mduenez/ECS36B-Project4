#include <gtest/gtest.h>
#include "OpenStreetMap.h"

TEST(OpenStreetMapTest, Stub) {
    auto reader = std::shared_ptr<CXMLReader>(nullptr);
    COpenStreetMap map(reader);

    EXPECT_EQ(map.NodeCount(), 0);
    EXPECT_EQ(map.WayCount(), 0);
}
