//
// Created by alexis on 6/14/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_GITHUBAPICLIENT_H
#define APPIMAGE_RELEASES_INDEXER_GITHUBAPICLIENT_H

#include <QObject>
#include <QVariant>

#include "PageDownloader.h"

class GitHubApiClient : public QObject {
Q_OBJECT
    QString user;
    QString repo;

    PageDownloader* repoPageDownloader;
    PageDownloader* userPageDownloader;
    PageDownloader* releasesPageDownloader;
    QVariantMap repoData;
    QVariantMap developerData;
    QVariantList releasesData;
    bool repoPageFinished;
    bool userPageFinished;
    bool releasesPageFinished;
    bool running;
public:
    explicit GitHubApiClient(QObject* parent = nullptr);
    void fetch();
    void setUser(const QString& user);
    void setRepo(const QString& repo);
    const QVariantMap& getRepoData() const;
    const QVariantMap& getDeveloperData() const;
    const QVariantList& getReleasesData() const;
signals:
    void completed();

protected slots:
    void handleRepoInfo();
    void handleDeveloperInfo();
    void handleReleasesInfo();

private:
    void queryRepoInfo();
    void queryDeveloperInfo();
    void queryReleasesInfo();
    void checkAllTasksCompletion();
};

#endif //APPIMAGE_RELEASES_INDEXER_GITHUBAPICLIENT_H
