#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

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
        "<osm>\n"
        "   <way id=\"500\">\n"
        "       <nd ref=\"10\"/>\n"
        "       <nd ref=\"20\"/>\n"
        "       <nd ref=\"30\"/>\n"
        "   </way>\n"
        "</osm>"
    );
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
