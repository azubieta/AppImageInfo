#include <gtest/gtest.h>
#include <entities/DesktopFileMetadataExtractor.h>

using namespace nlohmann;

TEST(DesktopFileMetadataExtractorTest, getContent) {
    DesktopFileMetadataExtractor extractor(TEST_DATA_DIR"/appimagetool.desktop");
    json expected = R"({"Desktop Entry":{"Categories":["Development","Utils"],"Comment":{"default":"Tool to generate AppImages from AppDirs","es":"Herramienta para generar AppImages a partir de AppDirs"},"Exec":"appimagetool","Icon":{"default":"appimagetool"},"Name":{"default":"appimagetool"},"Terminal":"true","Type":"Application"},"id":"appimagetool.desktop"})"_json;

    auto result = extractor.getContent();
    ASSERT_EQ(result.dump(), expected.dump());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
