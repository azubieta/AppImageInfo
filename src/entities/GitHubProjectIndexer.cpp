#include <QRegExp>
#include <QtCore/QJsonDocument>
#include "GitHubProjectIndexer.h"
#include "RemoteAppImageMetadataExtractor.h"
#include "AppInfoBuilder.h"

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

        emit completed();
    }
}

void GitHubProjectIndexer::addCurrentReleaseToAppInfo() {
    appCurrentRelease["files"] = appCurrentReleaseFiles;
    appInfoAssembler.addRelease(appCurrentRelease);
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

    appInfoAssembler.setFormat(1);
    appInfoAssembler.setKeyWords(ghRepo["keywords"].toStringList());
    appInfoAssembler.setLicense(ghRepo["licence"].toString());

    appInfoAssembler.setAppDeveloper(extractDeveloperInfo());
    appInfoAssembler.addDescription(QString("null"), ghRepo["description"].toString());

    appInfoAssembler.addLink(QString("homepage"), ghRepo["homepage"].toString());
    appInfoAssembler.addLink(QString("developers"), ghRepo["development_page"].toString());
    appInfoAssembler.addLink(QString("help"), ghRepo["wiki_page"].toString());

    processNextRelease();
}

void GitHubProjectIndexer::handleReleaseFileInfoExtractionCompleted() {
    const auto fileInfo = extractor->getMetadata();
    const auto url = extractor->getUrl();
    extractor->deleteLater();

    appInfoAssembler.fillMissingAppInfoFields(fileInfo);

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

QVariantMap GitHubProjectIndexer::extractDeveloperInfo() const {
    const auto ghDeveloperData = ghClient.getDeveloperData();
    QVariantMap developer;
    developer["name"] = ghDeveloperData["name"];
    developer["logo"] = ghDeveloperData["avatar_url"];
    developer["website"] = ghDeveloperData["blog"];
    return developer;
}

QVariantMap GitHubProjectIndexer::getAppInfo() {
    return appInfoAssembler.build();
}


