//
// Created by alexis on 6/7/18.
//

#include "FileMetadataMerger.h"

void MetadataMerger::setDesktop(const QVariantMap &desktop) {
    MetadataMerger::desktop = desktop;
}

void MetadataMerger::setAppStream(const QVariantMap &appstream) {
    MetadataMerger::appStream = appstream;
}

MetadataMerger::MetadataMerger() {}

QVariantMap MetadataMerger::getOutput() {
    data.clear();

    data["format"] = 1;
    data["id"] = appStream.contains("id") ? appStream.value("id") : desktop.value("id");
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

    data = removeEmptyFields(data).toMap();
    return data;
}

QVariantList MetadataMerger::getCategories() const {
    const auto desktopEntry = desktop["Desktop Entry"].toMap();
    return desktopEntry.value("Categories").toList();
}

QVariantMap MetadataMerger::getDescription() const {
    QVariantMap description;
    if (appStream.contains("description"))
        description["null"] = appStream.value("description");
    return description;
}

QMap<QString, QVariant> MetadataMerger::getAbstract() const {
    const auto desktopEntry = desktop["Desktop Entry"].toMap();
    return desktopEntry.value("Comment").toMap();
}

const QVariant MetadataMerger::getName() const {
    const auto desktopEntry = desktop["Desktop Entry"].toMap();
    return desktopEntry.value("Name");
}

QStringList MetadataMerger::getKeywords() const {
    QStringList keywords;
    const auto desktopEntry = desktop["Desktop Entry"].toMap();
    const auto genericNames = desktopEntry["GenericName"].toMap();
    for (auto value: genericNames.values())
        keywords << value.toString();
    return keywords;
}

QVariant MetadataMerger::removeEmptyFields(QVariant variant) {
    if (variant.type() == QVariant::Map) {
        auto variantMap = variant.toMap();
        for (auto key: variantMap.keys()) {
            variantMap[key] = removeEmptyFields(variantMap[key]);
            if (variantMap[key].isNull())
                variantMap.remove(key);
        }

        if (variantMap.isEmpty())
            return QVariant();
        else
            return variantMap;
    }

    if (variant.type() == QVariant::List) {
        auto variantList = variant.toList();
        for (auto &i : variantList)
            i = removeEmptyFields(i);

        variantList.removeAll(QVariant());
        if (variantList.isEmpty())
            return QVariant();
        else
            return variantList;
    }

    return variant;
}

void MetadataMerger::setBinary(const QVariantMap &binary) {
    MetadataMerger::binary = binary;
}

QVariant MetadataMerger::getIcon() {
    // TODO: Implement icon url extraction from AppStream
    return QVariant();
}

QVariant MetadataMerger::getLicense() {
    QVariantMap license;
    license["id"] = appStream["project_license"];
    return license;
}

QVariantMap MetadataMerger::getDeveloper() {
    QVariantMap developer;
    developer["name"] = appStream["developer_name"];

    return developer;
}

QVariantMap MetadataMerger::getRelease() {
    QVariantMap release;
    const auto appStreamReleases = data["releases"].toList();
    if (!appStreamReleases.isEmpty()) {
        auto appStreamLatestRelease = appStreamReleases.first().toMap();
        release["version"] = appStreamLatestRelease["version"];
        release["date"] = appStreamLatestRelease.contains("date") ? appStreamLatestRelease["date"] : binary["date"];

        QVariantMap changeLog;
        changeLog["null"] = appStreamLatestRelease["description"];
        release["changelog"] = changeLog;
    }
    return release;
}

QVariantMap MetadataMerger::getFile() {
    QVariantMap file;
    file["type"] = binary["type"];
    file["size"] = binary["size"];
    file["architecture"] = binary["architecture"];
    file["sha512checksum"] = binary["sha512checksum"];
    return file;
}

QVariantList MetadataMerger::getLanguages() {
    QVariantList languages;
    const auto desktopEntry = desktop["Desktop Entry"].toMap();
    const auto genericNames = desktopEntry["Name"].toMap();
    for (auto value: genericNames.keys())
        languages << value;

    languages.removeAll("null");
    return languages;
}

QVariantList MetadataMerger::getScreenShots() {
    QVariantList screenShots;
    const auto appStreamScreenShots = appStream.value("screenshots").toList();
    for (const auto &variant: appStreamScreenShots) {
        const auto &appStreamScreenShot = variant.toMap();

        QVariantMap screenShot;
        screenShot["height"] = appStreamScreenShot["height"];
        screenShot["width"] = appStreamScreenShot["width"];
        screenShot["language"] = appStreamScreenShot["language"];
        screenShot["caption"] = appStreamScreenShot["caption"];
        screenShot["url"] = appStreamScreenShot["url"];
    }

    return screenShots;
}

QVariantMap MetadataMerger::getLinks() {
    return appStream.value("urls").toMap();
}

QVariantList MetadataMerger::getMimeTypes() {
    const auto desktopEntry = desktop["Desktop Entry"].toMap();
    auto mimeTypes = desktopEntry.value("MimeType").toList();
    return mimeTypes;
}
