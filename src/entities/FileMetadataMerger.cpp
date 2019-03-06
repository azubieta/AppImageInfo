#include "FileMetadataMerger.h"

void MetadataMerger::setDesktop(const nlohmann::json& desktop) {
    MetadataMerger::desktop = desktop;
}

void MetadataMerger::setAppStream(const nlohmann::json& appstream) {
    MetadataMerger::appStream = appstream;
}

MetadataMerger::MetadataMerger() {}

nlohmann::json MetadataMerger::getOutput() {
    data.clear();

    data["format"] = 1;
    data["id"] = appStream.find("id") != appStream.end() ? appStream["id"] : desktop["id"];
    data["name"] = getName();
    data["icon"] = getIcon();
    data["abstract"] = getAbstract();
    data["description"] = getDescription();
    data["license"] = getLicense();
    data["categories"] = getCategories();
    data["keywords"] = getKeywords();
    data["languages"] = getLanguages();
    data["developer"] = getDeveloper();
    data["release"] = getRelease();
    data["file"] = getFile();
    data["screenshots"] = getScreenShots();
    data["mime-types"] = getMimeTypes();
    data["links"] = getLinks();

    data = removeEmptyFields(data);
    return data;
}

nlohmann::json MetadataMerger::getCategories() const {
    nlohmann::json categories;
    if (desktop.find("Desktop Entry") != desktop.end() &&
        desktop["Desktop Entry"].find("Categories") != desktop["Desktop Entry"].end()) {
        const auto desktopEntry = desktop["Desktop Entry"];
        categories = desktopEntry["Categories"];
    }
    return categories;
}

nlohmann::json MetadataMerger::getDescription() const {
    nlohmann::json description;
    if (appStream.find("description") != appStream.end())
        description[DEFUALT_LOCALE_NAME] = appStream["description"];
    return description;
}

nlohmann::json MetadataMerger::getAbstract() const {
    nlohmann::json abstract;
    if (desktop.find("Desktop Entry") != desktop.end() &&
        desktop["Desktop Entry"].find("Comment") != desktop["Desktop Entry"].end()) {
        const auto desktopEntry = desktop["Desktop Entry"];
        abstract = desktopEntry["Comment"];
    }

    return abstract;
}

const nlohmann::json MetadataMerger::getName() const {
    nlohmann::json name;
    if (desktop.find("Desktop Entry") != desktop.end() &&
        desktop["Desktop Entry"].find("Name") != desktop["Desktop Entry"].end()) {
        const auto desktopEntry = desktop["Desktop Entry"];
        name = desktopEntry["Name"];
    }

    return name;
}

std::list<std::string> MetadataMerger::getKeywords() const {
    std::list<std::string> keywords;
    if (desktop.find("Desktop Entry") != desktop.end() &&
        desktop["Desktop Entry"].find("GenericName") != desktop["Desktop Entry"].end()) {

        auto ptr = "/Desktop Entry/GenericName"_json_pointer;

        const nlohmann::json genericNames = desktop[ptr];
        for (const auto it: genericNames) {
            keywords.push_back(it.get<std::string>());
        }
    }
    return keywords;
}

nlohmann::json MetadataMerger::removeEmptyFields(nlohmann::json json) {
    if (json.is_object()) {
        for (auto it = json.begin(); it != json.end(); ++it) {
            json[it.key()] = removeEmptyFields(it.value());

            if (json[it.key()].empty())
                json.erase(it.key());
        }
    }

    if (json.empty())
        return nlohmann::json{};
    else
        return json;
}

void MetadataMerger::setBinary(const nlohmann::json& binary) {
    MetadataMerger::binary = binary;
}

std::string MetadataMerger::getIcon() {
    // TODO: Implement icon url extraction from AppStream
    return std::string();
}

nlohmann::json MetadataMerger::getLicense() {
    nlohmann::json license;
    license["id"] = appStream["project_license"];
    return license;
}

nlohmann::json MetadataMerger::getDeveloper() {
    nlohmann::json developer;
    developer["name"] = appStream["developer_name"];

    return developer;
}

nlohmann::json MetadataMerger::getRelease() {
    nlohmann::json release;
    const auto appStreamReleases = data["releases"];
    if (!appStreamReleases.empty()) {
        auto appStreamLatestRelease = appStreamReleases.front();
        release["version"] = appStreamLatestRelease["version"];
        release["date"] =
            appStreamLatestRelease.find("date") != appStreamLatestRelease.end() ? appStreamLatestRelease["date"]
                                                                                : binary["date"];

        nlohmann::json changeLog;
        changeLog["null"] = appStreamLatestRelease["description"];
        release["changelog"] = changeLog;
    }
    return release;
}

nlohmann::json MetadataMerger::getFile() {
    nlohmann::json file;
    file["type"] = binary["type"];
    file["size"] = binary["size"];
    file["architecture"] = binary["architecture"];
    file["sha512checksum"] = binary["sha512checksum"];
    return file;
}

std::list<std::string> MetadataMerger::getLanguages() {
    std::list<std::string> languages;
    const auto desktopEntry = desktop["Desktop Entry"];
    const auto names = desktopEntry["Name"];
    for (auto it = names.begin(); it != names.end(); it++)
        languages.push_back(it.key());

    languages.remove(DEFUALT_LOCALE_NAME);
    return languages;
}

nlohmann::json MetadataMerger::getScreenShots() {
    nlohmann::json screenShots;
    const auto appStreamScreenShots = appStream["screenshots"];
    for (const auto& appStreamScreenShot: appStreamScreenShots) {
        nlohmann::json screenShot;
        screenShot["height"] = appStreamScreenShot["height"];
        screenShot["width"] = appStreamScreenShot["width"];
        screenShot["language"] = appStreamScreenShot["language"];
        screenShot["caption"] = appStreamScreenShot["caption"];
        screenShot["url"] = appStreamScreenShot["url"];

        screenShots.push_back(screenShot);
    }

    return screenShots;
}

nlohmann::json MetadataMerger::getLinks() {
    return appStream["urls"];
}

nlohmann::json MetadataMerger::getMimeTypes() {
    if (desktop.find("Desktop Entry") != desktop.end() &&
        desktop["Desktop Entry"].find("MimeType") != desktop["Desktop Entry"].end())
        return desktop["/Desktop Entry/MimeType"_json_pointer];
    return nlohmann::json();
}
