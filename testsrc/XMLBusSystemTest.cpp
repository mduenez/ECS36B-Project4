#include <gtest/gtest.h>
#include "XMLBusSystem.h"

TEST(XMLBusSystemTest, Stub) {
    auto reader1 = std::shared_ptr<CXMLReader>(nullptr);
    auto reader2 = std::shared_ptr<CXMLReader>(nullptr);

    CXMLBusSystem system(reader1, reader2);

    EXPECT_EQ(system.StopCount(), 0);
    EXPECT_EQ(system.RouteCount(), 0);
}
