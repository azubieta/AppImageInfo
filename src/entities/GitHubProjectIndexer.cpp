#include <QRegExp>
#include <QtCore/QJsonDocument>
#include "GitHubProjectIndexer.h"
#include "RemoteAppImageMetadataExtractor.h"
#include <QVariantList>

GitHubProjectIndexer::GitHubProjectIndexer(const QString &url, QObject *parent)
        : QObject(parent), url(url), running(false), extractor(nullptr) {
    path = extractPath(url);
    user = path.section("/", 0, 0);
    repo = path.section("/", 1, 1);
}

bool GitHubProjectIndexer::isGitHubProject(QString url) {
    QRegExp regExp(regEx);
    int index = regExp.indexIn(url);
    return index != -1;
}

QString GitHubProjectIndexer::extractPath(const QString &url) {
    QRegExp regExp(regEx);
    int index = regExp.indexIn(url);
    if (index == -1)
        throw std::runtime_error("Unable to guess github project path from: " + url.toStdString());
    return regExp.cap(1);
}

void GitHubProjectIndexer::run() {
    if (running)
        throw std::runtime_error("GitHubProjectIndexer already running!");

    running = true;

    ghClient.setUser(user);
    ghClient.setRepo(repo);
    connect(&ghClient, &GitHubApiClient::completed, this, &GitHubProjectIndexer::handleGitHubApiDataReady);

    ghClient.fetch();
}

const QString &GitHubProjectIndexer::getPath() const {
    return path;
}

void GitHubProjectIndexer::processNextRelease() {
    if (ghReleaseFiles.isEmpty() && !appCurrentRelease.isEmpty())
        addCurrentReleaseToAppInfo();

    if (!ghReleases.isEmpty() || !ghReleaseFiles.isEmpty()) {
        if (ghReleaseFiles.isEmpty()) {
            auto latestGhRelease = ghReleases.first().toMap();
            ghReleases.pop_front();

            ghReleaseFiles = latestGhRelease["files"].toStringList();

            resetCurrentRelease(latestGhRelease);
        }
        processNextReleaseFile();
    } else {
        appInfo["name"] = appName;
        appInfo["abstract"] = appAbstract;
        appInfo["description"] = appDescription;
        appInfo["developer"] = appDeveloper;
        appInfo["links"] = appLinks;
        appInfo["categories"] = appCategories;
        appInfo["releases"] = appReleases;

        emit completed();
    }
}

void GitHubProjectIndexer::addCurrentReleaseToAppInfo() {
    appCurrentRelease["files"] = appCurrentReleaseFiles;
    appReleases << appCurrentRelease;
}

void GitHubProjectIndexer::resetCurrentRelease(const QMap<QString, QVariant> &latestRelease) {
    appCurrentRelease = QVariantMap();
    appCurrentRelease["channel"] = latestRelease["channel"];
    appCurrentRelease["date"] = latestRelease["date"];
    QVariantMap changelog;
    changelog["null"] = latestRelease["description"];
    appCurrentRelease["changelog"] = changelog;
    appCurrentRelease["version"] = latestRelease["version"];
    appCurrentRelease["source"] = latestRelease["source"];

    appCurrentReleaseFiles = QVariantList();
}

void GitHubProjectIndexer::processNextReleaseFile() {
    if (ghReleaseFiles.isEmpty())
        processNextRelease();
    else {
        if (extractor != nullptr)
            extractor->deleteLater();

        extractor = new RemoteAppImageMetadataExtractor(ghReleaseFiles.front());
        ghReleaseFiles.pop_front();

        connect(extractor, &RemoteAppImageMetadataExtractor::completed,
                this, &GitHubProjectIndexer::handleReleaseFileInfoExtractionCompleted);

        extractor->run();
    }
}

void GitHubProjectIndexer::handleGitHubApiDataReady() {
    const auto ghRepo = ghClient.getRepoData();
    ghReleases = ghClient.getReleasesData();

    appInfo["format"] = 1;
    appInfo["keywords"] = ghRepo["keywords"];
    appInfo["licence"] = ghRepo["licence"];

    appDeveloper = extractDeveloperInfo();
    appDescription["null"] = ghRepo["description"];

    appLinks["homepage"] = ghRepo["homepage"];
    appLinks["developers"] = ghRepo["development_page"];
    appLinks["help"] = ghRepo["wiki_page"];

    processNextRelease();
}

void GitHubProjectIndexer::handleReleaseFileInfoExtractionCompleted() {
    const auto fileInfo = extractor->getMetadata();
    const auto url = extractor->getUrl();
    extractor->deleteLater();

    fillMissingAppInfoFields(fileInfo);

    appCurrentReleaseFiles << getAppReleaseFileInfo(fileInfo, url);
    processNextReleaseFile();
}

QVariantMap GitHubProjectIndexer::getAppReleaseFileInfo(const QVariantMap &fileInfo, const QString &url) const {
    QVariantMap file;
    file["url"] = url;
    file["architecture"] = fileInfo["architecture"];
    file["sha512checksum"] = fileInfo["sha512checksum"];
    file["size"] = fileInfo["size"];
    file["type"] = fileInfo["type"];
    return file;
}

void GitHubProjectIndexer::fillMissingAppInfoFields(const QVariantMap &fileInfo) {
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

    const auto fileDescription = fileInfo["description"].toMap();
    for (auto key: fileDescription.keys())
        if (!appDescription.contains(key))
            appDescription[key] = fileDescription[key];

    const auto fileLinks = fileInfo["links"].toMap();
    for (auto key: fileLinks.keys())
        if (!appLinks.contains(key))
            appLinks[key] = fileLinks[key];
}

QVariantMap GitHubProjectIndexer::extractDeveloperInfo() const {
    const auto ghDeveloperData = ghClient.getDeveloperData();
    QVariantMap developer;
    developer["name"] = ghDeveloperData["name"];
    developer["logo"] = ghDeveloperData["avatar_url"];
    developer["website"] = ghDeveloperData["blog"];
    return developer;
}

const QVariantMap &GitHubProjectIndexer::getAppInfo() const {
    return appInfo;
}


