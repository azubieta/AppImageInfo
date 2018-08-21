//
// Created by alexis on 6/5/18.
//

#include <set>
#include <fstream>
#include "DesktopFileMetadataExtractor.h"
#include "FileMetadataMerger.h"

using namespace std;
using namespace nlohmann;

DesktopFileMetadataExtractor::DesktopFileMetadataExtractor(const std::string &filePath)
        : filePath(filePath) {}

nlohmann::json DesktopFileMetadataExtractor::getContent() {
    tryReadLines();
    parseLines();
    return data;
}

void DesktopFileMetadataExtractor::parseApplicationId() {
    std::size_t found = filePath.find_last_of("/\\");
    data["id"] = filePath.substr(found + 1);
}

void DesktopFileMetadataExtractor::parseLines() {
    data = nlohmann::json{};
    currentGroupName = std::string{};
    currentGroup = nlohmann::json{};

    parseApplicationId();

    for (const auto &line: fileContent) {
        auto cleanLine = removeComments(line);
        if (isGroupEntry(cleanLine))
            handleGroupEntry(cleanLine);

        if (isKeyEntry(cleanLine))
            handleKeyEntry(cleanLine);
    }

    saveLastGroup();
}

void DesktopFileMetadataExtractor::saveLastGroup() {
    if (!currentGroupName.empty())
        saveCurrentGroup();
}

void DesktopFileMetadataExtractor::handleGroupEntry(const std::string &cleanLine) {
    auto groupName = cleanLine.substr(1, cleanLine.length() - 2);

    if (!currentGroupName.empty())
        saveCurrentGroup();

    if (currentGroupName.empty())
        currentGroupName = groupName;
}

void DesktopFileMetadataExtractor::handleKeyEntry(const std::string &cleanLine) {
    auto full_key = cleanLine.substr(0, cleanLine.find('='));
    auto key = full_key.substr(0, full_key.find('['));
    auto locale = extractLocale(full_key);
    auto value = extractValue(cleanLine, key);

    if (isLocaleStringKey(key)) {
        auto localeStringsGroup = currentGroup[key];
        localeStringsGroup[locale] = value;
        currentGroup[key] = localeStringsGroup;
    } else
        currentGroup[key] = value;
}

std::string DesktopFileMetadataExtractor::extractLocale(const std::string &full_key) const {
    auto sIdx = full_key.find('[');
    if (sIdx != std::string::npos) {
        auto locale = full_key.substr(+1, full_key.size() - 1);
        return locale;
    } else
        return DEFUALT_LOCALE_NAME;
}

void DesktopFileMetadataExtractor::saveCurrentGroup() {
    data[currentGroupName] = currentGroup;
    currentGroupName = std::string{};
    currentGroup = nlohmann::json{};
}

bool DesktopFileMetadataExtractor::isLocaleStringKey(const std::string &key) const {
    static const std::set<std::string> localeStrings{"Name", "GenericName", "Comment", "Icon", "Keywords"};
    return localeStrings.find(key) != localeStrings.end();
}

nlohmann::json DesktopFileMetadataExtractor::extractValue(const std::string &cleanLine, const std::string &key) const {
    static const std::set<std::string> arrayValues{"OnlyShowIn", "NotShowIn", "Actions", "MimeType", "Categories",
                                                   "Implements", "Keywords"};

    std::string valueField = cleanLine.substr(cleanLine.find('=') + 1, cleanLine.size());
    if (!valueField.empty() && arrayValues.find(key) != arrayValues.end()) {
        json value;
        unsigned long prev_ptr = 0;
        auto ptr = valueField.find(';');
        while (ptr != std::string::npos) {
            auto item = valueField.substr(prev_ptr, ptr);
            if (!item.empty())
                value.push_back(item);

            prev_ptr = ptr;
            ptr = valueField.find(';', prev_ptr + 1);
        }
        return value;
    } else
        return valueField;

}

bool DesktopFileMetadataExtractor::isKeyEntry(const std::string &cleanLine) const {
    return cleanLine.find("=") != std::string::npos;
}

bool DesktopFileMetadataExtractor::isGroupEntry(const std::string &cleanLine) const {
    return cleanLine[0] == '[' && cleanLine[cleanLine.size() - 1] == ']';
}

std::string DesktopFileMetadataExtractor::removeComments(const std::string &cleanLine) const {
    return cleanLine.substr(0, cleanLine.find('#'));
}

void DesktopFileMetadataExtractor::tryReadLines() {
    std::ifstream f;
    f.open(filePath, std::ifstream::in);
    if (!f.fail()) {
        while (!f.fail() && !f.eof()) {
            std::string line;
            std::getline(f, line);
            fileContent.push_back(line);
        }
    } else
        throw std::runtime_error("Unable to open file: " + filePath);
}
