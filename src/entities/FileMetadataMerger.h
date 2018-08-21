//
// Created by alexis on 6/7/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
#define APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H

static const char *const DEFUALT_LOCALE_NAME = "default";

#include <list>
#include <nlohmann/json.hpp>

class MetadataMerger {
    nlohmann::json desktop;
    nlohmann::json appStream;
    nlohmann::json binary;
    nlohmann::json data;
public:

    MetadataMerger();
    void setDesktop(const nlohmann::json &desktop);
    void setAppStream(const nlohmann::json &appstream);
    void setBinary(const nlohmann::json &binary);

    nlohmann::json getOutput();
protected:

    std::list<std::string> getKeywords() const;

    const nlohmann::json getName() const;

    nlohmann::json getAbstract() const;

    nlohmann::json getDescription() const;

    std::string getIcon();

    nlohmann::json getLicense();

    nlohmann::json getCategories() const;

    nlohmann::json getDeveloper();

    nlohmann::json getRelease();

    nlohmann::json getFile();

    std::list<std::string> getLanguages();

    nlohmann::json getScreenShots();

    nlohmann::json getLinks();

    nlohmann::json getMimeTypes();

    nlohmann::json removeEmptyFields(nlohmann::json json);
};

#endif //APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
