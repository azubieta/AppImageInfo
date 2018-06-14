#include <QRegExp>
#include <QtCore/QJsonDocument>
#include "GitHubProjectIndexer.h"
#include <QVariantList>

GitHubProjectIndexer::GitHubProjectIndexer(const QString& url, QObject* parent)
        :QObject(parent), url(url), running(false)
{
    path = extractPath(url);
    user = path.section("/", 0, 0);
    repo = path.section("/", 1, 1);
}
bool GitHubProjectIndexer::isGitHubProject(QString url)
{
    QRegExp regExp(regEx);
    int index = regExp.indexIn(url);
    return index!=-1;
}
QString GitHubProjectIndexer::extractPath(const QString& url)
{
    QRegExp regExp(regEx);
    int index = regExp.indexIn(url);
    if (index==-1)
        throw std::runtime_error("Unable to guess github project path from: "+url.toStdString());
    return regExp.cap(1);
}
void GitHubProjectIndexer::run()
{
    if (running)
        throw std::runtime_error("GitHubProjectIndexer already running!");

    running = true;

    ghClient.setUser(user);
    ghClient.setRepo(repo);
    connect(&ghClient, &GitHubApiClient::completed, this, &GitHubProjectIndexer::handleGitHubApiDataReady);

    ghClient.fetch();
}

const QString& GitHubProjectIndexer::getPath() const
{
    return path;
}
void GitHubProjectIndexer::processNextRelease()
{
//    if (!releasesData.isEmpty()) {
//        auto release = releasesData.first().toMap();
//        releasesData.removeFirst();
//        downloadRelease(release);
//    }
//    else
    emit completed();
}
void GitHubProjectIndexer::downloadRelease(QMap<QString, QVariant> map)
{

}
void GitHubProjectIndexer::handleGitHubApiDataReady()
{
    processNextRelease();
}


