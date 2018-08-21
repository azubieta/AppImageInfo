//
// Created by alexis on 6/5/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_DESKTOPFILEMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_DESKTOPFILEMETADATAEXTRACTOR_H


#include <list>
#include <nlohmann/json.hpp>

class DesktopFileMetadataExtractor {
    std::string filePath;
    std::list<std::string> fileContent;
    std::string currentGroupName;
    nlohmann::json currentGroup;
    nlohmann::json data;
public:
    explicit DesktopFileMetadataExtractor(const std::string &filePath);
    nlohmann::json getContent();
protected:
    void tryReadLines();
    void parseLines();
    std::string removeComments(const std::string &cleanLine) const;
    bool isGroupEntry(const std::string &cleanLine) const;
    bool isKeyEntry(const std::string &cleanLine) const;
    nlohmann::json extractValue(const std::string &cleanLine, const std::string &key) const;
    bool isLocaleStringKey(const std::string &key) const;

    void saveCurrentGroup();
    void handleKeyEntry(const std::string &cleanLine);
    void handleGroupEntry(const std::string &cleanLine);
    void saveLastGroup();
    void parseApplicationId();

    std::string extractLocale(const std::string &full_key) const;
};

#endif //APPIMAGE_RELEASES_INDEXER_DESKTOPFILEMETADATAEXTRACTOR_H
