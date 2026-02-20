#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(XMLReaderTest, SimpleElement){

    std::string xml = "<root attr=\"value\">text</root>";

    auto source = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(source);

    SXMLEntity entity;

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");
    EXPECT_TRUE(entity.AttributeExists("attr"));
    EXPECT_EQ(entity.AttributeValue("attr"), "value");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "text");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "root");

    EXPECT_FALSE(reader.ReadEntity(entity));
    EXPECT_TRUE(reader.End());

}
