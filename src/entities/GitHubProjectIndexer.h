//
// Created by alexis on 6/9/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_GITHUBPROJECTINDEXER_H
#define APPIMAGE_RELEASES_INDEXER_GITHUBPROJECTINDEXER_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVariantList>

#include "../gateways/PageDownloader.h"
#include "../gateways/GitHubApiClient.h"
#include "RemoteAppImageMetadataExtractor.h"
#include "AppInfoAssembler.h"

class GitHubProjectIndexer : public QObject {
Q_OBJECT

    static constexpr auto regEx = R"(github.com\/([\w\.\-]+\/[\w\.\-]+)[\/$]?)";
    QString url;
    bool running;
    QString path;
    QString user;
    QString repo;
    QVariantMap appCurrentRelease;
    QVariantList appCurrentReleaseFiles;

    GitHubApiClient ghClient;

    AppInfoAssembler appInfoAssembler;
    QVariantList ghReleases;
    QStringList ghReleaseFiles;

    RemoteAppImageMetadataExtractor *extractor;
public:
    explicit GitHubProjectIndexer(const QString &url, QObject *parent = nullptr);

    static bool isGitHubProject(QString url);

    void run();

    const QString &getPath() const;

    QVariantMap getAppInfo();

signals:

    void completed();

protected slots:

    void handleGitHubApiDataReady();

    void handleReleaseFileInfoExtractionCompleted();

private:
    void processNextRelease();

    QString extractPath(const QString &url);

    QVariantMap extractDeveloperInfo() const;

    void processNextReleaseFile();

    QVariantMap getAppReleaseFileInfo(const QVariantMap &fileInfo, const QString &url) const;

    void resetCurrentRelease(const QMap<QString, QVariant> &latestRelease);

    void addCurrentReleaseToAppInfo();
};

#endif //APPIMAGE_RELEASES_INDEXER_GITHUBPROJECTINDEXER_H
