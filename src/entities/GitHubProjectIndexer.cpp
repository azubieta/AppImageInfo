#include <QRegExp>
#include <QtCore/QJsonDocument>
#include "GitHubProjectIndexer.h"

GitHubProjectIndexer::GitHubProjectIndexer(const QString& url, QObject* parent)
        :QObject(parent), url(url), running(false), repoPageDownloader(nullptr), repoPageFinished(false),
         userPageFinished(false), releasesPageFinished(false)
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
    queryRepoInfo();
    queryDeveloperInfo();
    queryReleasesInfo();
}
void GitHubProjectIndexer::queryRepoInfo()
{
    repoPageDownloader = new PageDownloader("https://api.github.com/repos/"+path);
    connect(repoPageDownloader, &PageDownloader::downloaded, this, &GitHubProjectIndexer::handleRepoInfo);

    repoPageDownloader->download();
}
void GitHubProjectIndexer::handleRepoInfo()
{
    if (repoPageDownloader->isErrored())
        qWarning() << "Unable to download repo information";

    QByteArray rawData = repoPageDownloader->getData();
    repoPageDownloader->deleteLater();
    repoPageDownloader = nullptr;

    auto doc = QJsonDocument::fromJson(rawData);
    auto gitHubRepoData = doc.toVariant().toMap();
    repoData["description"] = gitHubRepoData["description"];
    repoData["homepage"] = gitHubRepoData["homepage"];
    repoData["stargazers_count"] = gitHubRepoData["stargazers_count"];
    repoData["development_page"] = gitHubRepoData["html_url"];
    if (gitHubRepoData["has_wiki"].toBool())
        repoData["wiki_page"] = repoData["development_page"].toString()+"/wiki";

    auto license = gitHubRepoData["license"].toMap();
    repoData["licence"] = license["name"];

    qInfo() << gitHubRepoData;
    qInfo() << repoData;

    repoPageFinished = true;
    checkAllTasksCompletion();
}
void GitHubProjectIndexer::checkAllTasksCompletion()
{
    if (repoPageFinished && userPageFinished && releasesPageFinished) {
        running = false;
        emit completed();
    }
}
const QString& GitHubProjectIndexer::getPath() const
{
    return path;
}
void GitHubProjectIndexer::queryDeveloperInfo()
{
    userPageDownloader = new PageDownloader("https://api.github.com/users/"+user);
    connect(userPageDownloader, &PageDownloader::downloaded, this, &GitHubProjectIndexer::handleDeveloperInfo);

    userPageDownloader->download();
}
void GitHubProjectIndexer::handleDeveloperInfo()
{
    if (userPageDownloader->isErrored())
        qWarning() << "Unable to download user information";

    QByteArray rawData = userPageDownloader->getData();
    userPageDownloader->deleteLater();
    userPageDownloader = nullptr;

    auto doc = QJsonDocument::fromJson(rawData);
    auto gitHubUserData = doc.toVariant().toMap();

    developerData["name"] = gitHubUserData["name"];
    developerData["avatar_url"] = gitHubUserData["avatar_url"];
    developerData["bio"] = gitHubUserData["bio"];
    developerData["url"] = gitHubUserData["url"];
    developerData["blog"] = gitHubUserData["blog"];
    developerData["type"] = gitHubUserData["type"];

    qInfo() << developerData;
    qInfo() << gitHubUserData;
    userPageFinished = true;
    checkAllTasksCompletion();
}
void GitHubProjectIndexer::queryReleasesInfo()
{
    releasesPageDownloader = new PageDownloader("https://api.github.com/repos/"+path+"/releases");
    connect(releasesPageDownloader, &PageDownloader::downloaded, this, &GitHubProjectIndexer::handleReleasesInfo);

    releasesPageDownloader->download();
}
void GitHubProjectIndexer::handleReleasesInfo()
{
    if (releasesPageDownloader->isErrored())
        qWarning() << "Unable to download releases information";

    QByteArray rawData = releasesPageDownloader->getData();
    releasesPageDownloader->deleteLater();
    releasesPageDownloader = nullptr;

    auto doc = QJsonDocument::fromJson(rawData);
    auto gitHubReleasesData = doc.toVariant().toList();

    for (const auto variant: gitHubReleasesData) {
        QVariantMap release;
        const auto gitHubRelease = variant.toMap();

        release["version"] = gitHubRelease["tag_name"];
        release["channel"] = gitHubRelease["prerelease"].toBool() ? "development" : "stable";
        release["date"] = gitHubRelease["published_at"];
        release["description"] = gitHubRelease["body"];
        release["source"] = gitHubRelease["tarball_url"];

        QStringList assets;
        const auto assetsVariant = gitHubRelease["assets"].toList();
        for (const auto assetVariant: assetsVariant) {
            const auto asset = assetVariant.toMap();
            const auto url = asset["browser_download_url"].toString();
            if (url.endsWith("AppImage"))
                assets << url;
        }
        release["assets"] = assets;

        releaseData << release;
    }

    releasesPageFinished = true;
    checkAllTasksCompletion();
}


