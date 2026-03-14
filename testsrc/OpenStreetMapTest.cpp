#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

<<<<<<< HEAD
TEST(OpenStreetMapTest, SimpleTest){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    ASSERT_EQ(OpenStreetMap.NodeCount(), 2);
    auto Node = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(Node,nullptr);
    EXPECT_EQ(Node->ID(),1);
    auto Location = CStreetMap::SLocation{38.5,-121.7};
    EXPECT_EQ(Node->Location(),Location);
    ASSERT_EQ(OpenStreetMap.WayCount(), 1);

}

TEST(OpenStreetMapTest, EmptyTest){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(), 0);//no nodes, so should return 0 for node count
    EXPECT_EQ(OpenStreetMap.WayCount(), 0);//no nodes or ways, so should return 0 for both counts
    EXPECT_EQ(OpenStreetMap.NodeByIndex(0), nullptr);//no nodes, so should return nullptr
    EXPECT_EQ(OpenStreetMap.WayByIndex(0), nullptr);//no ways, so should return nullptr
}

TEST(OpenStreetMapTest, InvalidTest){ //asks for 100th node and way, which do not exist
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeByIndex(100), nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(100), nullptr);
}

TEST(OpenStreetMapTest, WayNodeTest){
    auto OSMSource = std::make_shared<CStringDataSource>(
=======
// -------------------- SimpleTest --------------------
TEST(OpenStreetMapTest, SimpleTest) {
    auto osmSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
        "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
        "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
        "  <way id=\"1000\">\n"
        "    <nd ref=\"1\"/>\n"
        "    <nd ref=\"2\"/>\n"
        "  </way>\n"
        "</osm>"
    );

    auto osmReader = std::make_shared<CXMLReader>(osmSource);
    OpenStreetMap osm(osmReader);

    ASSERT_EQ(osm.NodeCount(), 2);

    auto node = osm.NodeByIndex(0);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->ID(), static_cast<decltype(node->ID())>(1));

    const auto expectedLocation = StreetMap::SLocation{38.5, -121.7};
    EXPECT_DOUBLE_EQ(node->Location().lat, expectedLocation.lat);
    EXPECT_DOUBLE_EQ(node->Location().lon, expectedLocation.lon);

    ASSERT_EQ(osm.WayCount(), 1);
}

// -------------------- EmptyTest --------------------
TEST(OpenStreetMapTest, EmptyTest) {
    auto osmSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
        "</osm>"
    );

    auto osmReader = std::make_shared<CXMLReader>(osmSource);
    OpenStreetMap osm(osmReader);

    EXPECT_EQ(osm.NodeCount(), 0);
    EXPECT_EQ(osm.WayCount(), 0);
    EXPECT_EQ(osm.NodeByIndex(0), nullptr);
    EXPECT_EQ(osm.WayByIndex(0), nullptr);
}

// -------------------- InvalidTest --------------------
TEST(OpenStreetMapTest, InvalidTest) {
    auto osmSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
        "</osm>"
    );

    auto osmReader = std::make_shared<CXMLReader>(osmSource);
    OpenStreetMap osm(osmReader);

    EXPECT_EQ(osm.NodeByIndex(100), nullptr);
    EXPECT_EQ(osm.WayByIndex(100), nullptr);
}

// -------------------- WayNodeTest --------------------
TEST(OpenStreetMapTest, WayNodeTest) {
    auto osmSource = std::make_shared<CStringDataSource>(
>>>>>>> main
        "<osm>\n"
        "   <way id=\"500\">\n"
        "       <nd ref=\"10\"/>\n"
        "       <nd ref=\"20\"/>\n"
        "       <nd ref=\"30\"/>\n"
        "   </way>\n"
        "</osm>"
    );
<<<<<<< HEAD
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);
    ASSERT_EQ(OpenStreetMap.WayCount(), 1);
    auto Way = OpenStreetMap.WayByIndex(0);
    ASSERT_NE(Way, nullptr);
    EXPECT_EQ(Way->ID(), 500);
    EXPECT_EQ(Way->NodeCount(), 3);
    EXPECT_EQ(Way->GetNodeID(0), 10);
    EXPECT_EQ(Way->GetNodeID(1), 20);
    EXPECT_EQ(Way->GetNodeID(2), 30);
}
>>>>>>> main
=======

    auto osmReader = std::make_shared<CXMLReader>(osmSource);
    OpenStreetMap osm(osmReader);

    ASSERT_EQ(osm.WayCount(), 1);

    auto way = osm.WayByIndex(0);
    ASSERT_NE(way, nullptr);

    EXPECT_EQ(way->ID(), static_cast<decltype(way->ID())>(500));
    EXPECT_EQ(way->NodeCount(), 3);
    EXPECT_EQ(way->GetNodeID(0), static_cast<decltype(way->GetNodeID(0))>(10));
    EXPECT_EQ(way->GetNodeID(1), static_cast<decltype(way->GetNodeID(0))>(20));
    EXPECT_EQ(way->GetNodeID(2), static_cast<decltype(way->GetNodeID(0))>(30));
}
>>>>>>> main
