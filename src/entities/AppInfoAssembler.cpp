//
// Created by alexis on 6/18/18.
//

#include "GitHubProjectIndexer.h"
#include <QtCore/QJsonDocument>
#include <QRegExp>
#include "AppInfoAssembler.h"

void AppInfoAssembler::fillMissingAppInfoFields(const QVariantMap &fileInfo) {
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

QVariantMap AppInfoAssembler::getAppInfo() const {
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

void AppInfoAssembler::setAppInfo(const QVariantMap &appInfo) {
    AppInfoAssembler::appInfo = appInfo;
}

const QVariantMap &AppInfoAssembler::getAppName() const {
    return appName;
}

void AppInfoAssembler::setAppName(const QVariantMap &appName) {
    AppInfoAssembler::appName = appName;
}

const QVariantMap &AppInfoAssembler::getAppAbstract() const {
    return appAbstract;
}

void AppInfoAssembler::setAppAbstract(const QVariantMap &appAbstract) {
    AppInfoAssembler::appAbstract = appAbstract;
}

const QVariantMap &AppInfoAssembler::getAppDescription() const {
    return appDescription;
}

void AppInfoAssembler::setAppDescription(const QVariantMap &appDescription) {
    AppInfoAssembler::appDescription = appDescription;
}

const QVariantMap &AppInfoAssembler::getAppDeveloper() const {
    return appDeveloper;
}

void AppInfoAssembler::setAppDeveloper(const QVariantMap &appDeveloper) {
    AppInfoAssembler::appDeveloper = appDeveloper;
}

const QVariantMap &AppInfoAssembler::getAppLinks() const {
    return appLinks;
}

void AppInfoAssembler::setAppLinks(const QVariantMap &appLinks) {
    AppInfoAssembler::appLinks = appLinks;
}

const QStringList &AppInfoAssembler::getAppCategories() const {
    return appCategories;
}

void AppInfoAssembler::setAppCategories(const QStringList &appCategories) {
    AppInfoAssembler::appCategories = appCategories;
}

const QVariantList &AppInfoAssembler::getAppReleases() const {
    return appReleases;
}

void AppInfoAssembler::setAppReleases(const QVariantList &appReleases) {
    AppInfoAssembler::appReleases = appReleases;
}

void AppInfoAssembler::addRelease(const QVariantMap &release) {
    appReleases << release;
}

void AppInfoAssembler::setFormat(int format) {
    AppInfoAssembler::appInfo["format"] = format;
}

void AppInfoAssembler::setKeyWords(const QStringList keywords) {
    appInfo["keywords"] = keywords;
}

void AppInfoAssembler::setLicense(QString license) {
    appInfo["license"] = license;
}

void AppInfoAssembler::addDescription(const QString &locale, const QString &description) {
    appDescription[locale] = description;
}

void AppInfoAssembler::addLink(const QString &type, const QString &url) {
    appLinks[type] = url;
}
