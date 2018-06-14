//
// Created by alexis on 6/9/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_GITHUBPROJECTINDEXER_H
#define APPIMAGE_RELEASES_INDEXER_GITHUBPROJECTINDEXER_H

#include <QObject>
#include <QString>

#include "../gateways/PageDownloader.h"
#include "../gateways/GitHubApiClient.h"

class GitHubProjectIndexer : public QObject {
Q_OBJECT
    static constexpr auto regEx = R"(github.com\/([\w\.\-]+\/[\w\.\-]+)[\/$]?)";
    QString url;
    bool running;
    QString path;
    QString user;
    QString repo;

    QVariantList appImages;
    GitHubApiClient ghClient;
public:
    explicit GitHubProjectIndexer(const QString& url, QObject* parent = nullptr);

    static bool isGitHubProject(QString url);

    void run();
    void processNextRelease();
    const QString& getPath() const;
signals:
    void completed();

protected slots:
    void handleGitHubApiDataReady();

private:
    QString extractPath(const QString& url);

    void downloadRelease(QMap<QString, QVariant> map);
};

#endif //APPIMAGE_RELEASES_INDEXER_GITHUBPROJECTINDEXER_H
