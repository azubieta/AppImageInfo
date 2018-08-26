//
// Created by alexis on 6/6/18.
//

#include <set>
#include <boost/algorithm/string/trim.hpp>
#include <nlohmann/json.hpp>
#include "AppStreamMetadataExtractor.h"

using namespace nlohmann;

AppStreamMetadataExtractor::AppStreamMetadataExtractor(const std::string &filePath)
        : filePath(filePath) {
}

nlohmann::json AppStreamMetadataExtractor::getContent() {
    LIBXML_TEST_VERSION
    if ((doc = xmlReadFile(filePath.c_str(), nullptr, 0)) == nullptr) {
        throw AppStreamReadError("Could not parse file " + filePath);
    }

    data = json::object();
    root_element = xmlDocGetRootElement(doc);
    xmlNode *cur_node = nullptr;

    for (cur_node = root_element; cur_node; cur_node = cur_node->next) {
        std::string nodeName = reinterpret_cast<const char *>(cur_node->name);
        if (cur_node->type == XML_ELEMENT_NODE && "component" == nodeName)
            parseComponent(cur_node->children);

    }
    xmlFreeDoc(doc);       // free document
    xmlCleanupParser();    // Free globals
    return data;
}

void AppStreamMetadataExtractor::parseComponent(_xmlNode *childern) {
    xmlNode *cur_node = nullptr;

    std::set<std::string> simpleEntries{"id", "name", "summary", "metadata_license", "project_group", "project_license",
                                        "developer_name", "update_contact"};

    for (cur_node = childern; cur_node; cur_node = cur_node->next) {
        if (cur_node->name == nullptr)
            continue;

        std::string nodeName = reinterpret_cast<const char *>(cur_node->name);
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (simpleEntries.find(nodeName) != simpleEntries.end()) {

                auto rawValue = xmlNodeGetContent(cur_node->children);
                data[nodeName] = getString(rawValue);
                free(rawValue);
            }

            if ("description" == nodeName)
                data[nodeName] = readDescriptionNode(cur_node);

            if ("screenshots" == nodeName)
                data[nodeName] = readScreenshots(cur_node);

            if ("url" == nodeName) {
                auto urlType = xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("type"));
                auto value = xmlNodeGetContent(cur_node);
                data["urls"][getString(urlType)] = getString(value);
                free(urlType);
                free(value);
            }

            if ("releases" == nodeName) {
                data[nodeName] = readReleases(cur_node);
            }

        }
    }
}

std::string AppStreamMetadataExtractor::readDescriptionNode(const xmlNode *cur_node) const {
    xmlBufferPtr buff = xmlBufferCreate();

    for (auto cur_sub_node = cur_node->children; cur_sub_node; cur_sub_node = cur_sub_node->next)
        xmlNodeDump(buff, doc, cur_sub_node, 0, 0);

    auto value = getString(buff->content);
    xmlBufferFree(buff);
    return value;
}

std::string AppStreamMetadataExtractor::getString(const xmlChar *rawValue) const {
    std::string value;
    if (rawValue)
        value = reinterpret_cast<const char *>(rawValue);

    return value;
}

nlohmann::json AppStreamMetadataExtractor::readScreenshots(xmlNode *node) {
    json screenShots;

    for (auto cur_node = node->children; cur_node; cur_node = cur_node->next) {
        auto nodeName = getString(cur_node->name);
        if (cur_node->type == XML_ELEMENT_NODE && "screenshot" == nodeName) {
            json scrrenshot = readScreenshot(cur_node);
            screenShots.push_back(scrrenshot);
        }
    }

    return screenShots;
}

nlohmann::json AppStreamMetadataExtractor::readScreenshot(_xmlNode *node) {
    json screenshot = json::object();
    for (auto cur_node = node->children; cur_node; cur_node = cur_node->next) {
        auto nodeName = getString(cur_node->name);
        if (nodeName == "caption") {
            xmlChar *value = xmlNodeGetContent(cur_node);
            screenshot[nodeName] = getString(value);
            free(value);
        }

        if (nodeName == "image") {
            xmlChar *url = xmlNodeGetContent(cur_node);
            auto urlString = getString(url);
            boost::algorithm::trim(urlString);
            screenshot["url"] = urlString;
            xmlChar *type = xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("type"));
            xmlChar *width = xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("width"));
            xmlChar *height = xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("height"));
            xmlChar *language = xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("xml:lang"));
            screenshot["type"] = getString(type);
            screenshot["width"] = getString(width);
            screenshot["height"] = getString(height);
            screenshot["language"] = getString(language);
            free(url);
            free(type);
            free(width);
            free(height);
            free(language);
        }
    }
    return screenshot;
}

nlohmann::json AppStreamMetadataExtractor::readReleases(xmlNode *node) {
    json releases;
    for (auto cur_node = node->children; cur_node; cur_node = cur_node->next) {
        auto nodeName = getString(cur_node->name);
        if (nodeName == "release") {
            json release = readRealease(cur_node);
            releases.push_back(release);
        }
    }
    return releases;
}

nlohmann::json AppStreamMetadataExtractor::readRealease(_xmlNode *node) {
    json release;
    xmlChar *version = xmlGetProp(node, reinterpret_cast<const xmlChar *>("version"));
    xmlChar *date = xmlGetProp(node, reinterpret_cast<const xmlChar *>("date"));
    xmlChar *urgency = xmlGetProp(node, reinterpret_cast<const xmlChar *>("urgency"));
    xmlChar *timestamp = xmlGetProp(node, reinterpret_cast<const xmlChar *>("timestamp"));

    release["date"] = getString(date);
    release["version"] = getString(version);
    release["urgency"] = getString(urgency);
    release["timestamp"] = getString(timestamp);


    free(version);
    free(date);
    free(urgency);
    free(timestamp);

    for (auto cur_node = node->children; cur_node; cur_node = cur_node->next) {
        auto nodeName = getString(cur_node->name);
        if (nodeName == "location") {
            xmlChar *value = xmlNodeGetContent(cur_node);
            release["location"] = getString(value);
            free(value);
        }

        if (nodeName == "checksum") {
            xmlChar *type = xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("type"));
            xmlChar *value = xmlNodeGetContent(cur_node);
            release[getString(type)] = getString(value);
            free(type);
            free(value);
        }

        if (nodeName == "description") {
            xmlChar *value = xmlNodeGetContent(cur_node);
            release["description"] = getString(value);
            free(value);
        }
    }
    return release;
}

AppStreamReadError::AppStreamReadError(const std::string &__arg) : runtime_error(__arg) {}
