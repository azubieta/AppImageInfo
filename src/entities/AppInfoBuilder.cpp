//
// Created by alexis on 6/18/18.
//

#include "GitHubProjectIndexer.h"
#include <QtCore/QJsonDocument>
#include <QRegExp>
#include "AppInfoBuilder.h"

void AppInfoBuilder::fillMissingAppInfoFields(const QVariantMap &fileInfo) {
    if (!appInfo.contains("id"))
        appInfo["id"] = fileInfo["id"];

    const auto fileName = fileInfo["name"].toMap();
    for (auto key: fileName.keys())
        if (!appName.contains(key))
            appName[key] = fileName[key];

    if (appCategories.isEmpty())
        appCategories = fileInfo["categories"].toStringList();

    if (!appInfo.contains("screenshots") && fileInfo.contains("screenshots"))
        appInfo["screenshots"] = fileInfo["screenshots"];

    if (!appInfo.contains("icon") && fileInfo.contains("icon"))
        appInfo["icon"] = fileInfo["icon"];

    const auto fileAbstract = fileInfo["abstract"].toMap();
    for (auto key: fileAbstract.keys())
        if (!appAbstract.contains(key))
            appAbstract[key] = fileAbstract[key];

    const auto fileDescription = fileInfo["description"].toMap();
    for (auto key: fileDescription.keys())
        if (!appDescription.contains(key))
            appDescription[key] = fileDescription[key];

    const auto fileLinks = fileInfo["links"].toMap();
    for (auto key: fileLinks.keys())
        if (!appLinks.contains(key))
            appLinks[key] = fileLinks[key];
}

QVariantMap AppInfoBuilder::build() const {
    QVariantMap outputAppInfo = appInfo;
    outputAppInfo["name"] = appName;
    outputAppInfo["abstract"] = appAbstract;
    outputAppInfo["description"] = appDescription;
    outputAppInfo["developer"] = appDeveloper;
    outputAppInfo["links"] = appLinks;
    outputAppInfo["categories"] = appCategories;
    outputAppInfo["releases"] = appReleases;

    return outputAppInfo;
}

void AppInfoBuilder::setAppInfo(const QVariantMap &appInfo) {
    AppInfoBuilder::appInfo = appInfo;
}

const QVariantMap &AppInfoBuilder::getAppName() const {
    return appName;
}

void AppInfoBuilder::setAppName(const QVariantMap &appName) {
    AppInfoBuilder::appName = appName;
}

const QVariantMap &AppInfoBuilder::getAppAbstract() const {
    return appAbstract;
}

void AppInfoBuilder::setAppAbstract(const QVariantMap &appAbstract) {
    AppInfoBuilder::appAbstract = appAbstract;
}

const QVariantMap &AppInfoBuilder::getAppDescription() const {
    return appDescription;
}

void AppInfoBuilder::setAppDescription(const QVariantMap &appDescription) {
    AppInfoBuilder::appDescription = appDescription;
}

const QVariantMap &AppInfoBuilder::getAppDeveloper() const {
    return appDeveloper;
}

void AppInfoBuilder::setAppDeveloper(const QVariantMap &appDeveloper) {
    AppInfoBuilder::appDeveloper = appDeveloper;
}

const QVariantMap &AppInfoBuilder::getAppLinks() const {
    return appLinks;
}

void AppInfoBuilder::setAppLinks(const QVariantMap &appLinks) {
    AppInfoBuilder::appLinks = appLinks;
}

const QStringList &AppInfoBuilder::getAppCategories() const {
    return appCategories;
}

void AppInfoBuilder::setAppCategories(const QStringList &appCategories) {
    AppInfoBuilder::appCategories = appCategories;
}

const QVariantList &AppInfoBuilder::getAppReleases() const {
    return appReleases;
}

void AppInfoBuilder::setAppReleases(const QVariantList &appReleases) {
    AppInfoBuilder::appReleases = appReleases;
}

void AppInfoBuilder::addRelease(const QVariantMap &release) {
    appReleases << release;
}

void AppInfoBuilder::setFormat(int format) {
    AppInfoBuilder::appInfo["format"] = format;
}

void AppInfoBuilder::setKeyWords(const QStringList keywords) {
    appInfo["keywords"] = keywords;
}

void AppInfoBuilder::setLicense(QString license) {
    appInfo["license"] = license;
}

void AppInfoBuilder::addDescription(const QString &locale, const QString &description) {
    appDescription[locale] = description;
}

void AppInfoBuilder::addLink(const QString &type, const QString &url) {
    appLinks[type] = url;
}
