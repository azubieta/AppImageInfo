//
// Created by alexis on 6/6/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H

extern "C" {
#include <libxml/parser.h>
#include <libxml/tree.h>
};

#include <nlohmann/json.hpp>

class AppStreamReadError : public std::runtime_error {
public:
    explicit AppStreamReadError(const std::string &__arg);
};

class AppStreamMetadataExtractor {
    std::string filePath;
    nlohmann::json data{nlohmann::json::object()};
    xmlDoc *doc{nullptr};
    xmlNode *root_element{nullptr};

public:
    explicit AppStreamMetadataExtractor(const std::string &filePath);

    nlohmann::json getContent();

protected:
    void parseComponent(_xmlNode *childern);

    std::string getString(const xmlChar *rawValue) const;

    std::string readDescriptionNode(const xmlNode *cur_node) const;

    nlohmann::json readScreenshots(xmlNode *node);

    nlohmann::json readScreenshot(_xmlNode *node);

    nlohmann::json readReleases(xmlNode *node);

    nlohmann::json readRealease(_xmlNode *node);
};

#endif //APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H
