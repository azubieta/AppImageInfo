//
// Created by alexis on 6/14/18.
//

#include <QJsonDocument>
#include "GitHubApiClient.h"
GitHubApiClient::GitHubApiClient(QObject* parent)
        :QObject(parent), repoPageDownloader(nullptr), repoPageFinished(false),
         userPageFinished(false), releasesPageFinished(false) { }

void GitHubApiClient::fetch()
{
    if (running)
        throw std::runtime_error("GitHubProjectIndexer already running!");

    running = true;

    queryRepoInfo();
    queryDeveloperInfo();
    queryReleasesInfo();
}
void GitHubApiClient::queryRepoInfo()
{
    repoPageDownloader = new PageDownloader("https://api.github.com/repos/"+user+"/"+repo);
    connect(repoPageDownloader, &PageDownloader::finished, this, &GitHubApiClient::handleRepoInfo);

    repoPageDownloader->download();
}
void GitHubApiClient::handleRepoInfo()
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
void GitHubApiClient::queryDeveloperInfo()
{
    userPageDownloader = new PageDownloader("https://api.github.com/users/"+user);
    connect(userPageDownloader, &PageDownloader::finished, this, &GitHubApiClient::handleDeveloperInfo);

    userPageDownloader->download();
}
void GitHubApiClient::handleDeveloperInfo()
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
void GitHubApiClient::queryReleasesInfo()
{
    releasesPageDownloader = new PageDownloader("https://api.github.com/repos/"+user+"/"+repo+"/releases");

    connect(releasesPageDownloader, &PageDownloader::finished, this, &GitHubApiClient::handleReleasesInfo);

    releasesPageDownloader->download();
}
void GitHubApiClient::handleReleasesInfo()
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

        const auto assetsVariant = gitHubRelease["assets"].toList();
        for (const auto assetVariant: assetsVariant) {
            const auto asset = assetVariant.toMap();
            const auto url = asset["browser_download_url"].toString();
            if (url.endsWith("AppImage")) {
                auto fileRelease = release;
                fileRelease["download_url"] = url;
                releasesData << fileRelease;
            }
        }
    }

    releasesPageFinished = true;
    checkAllTasksCompletion();
}

void GitHubApiClient::checkAllTasksCompletion()
{
    if (repoPageFinished && userPageFinished && releasesPageFinished) {
        running = false;
        emit completed();
    }
}
void GitHubApiClient::setUser(const QString& user)
{
    GitHubApiClient::user = user;
}
void GitHubApiClient::setRepo(const QString& repo)
{
    GitHubApiClient::repo = repo;
}
const QVariantMap& GitHubApiClient::getRepoData() const
{
    return repoData;
}
const QVariantMap& GitHubApiClient::getDeveloperData() const
{
    return developerData;
}
const QVariantList& GitHubApiClient::getReleasesData() const
{
    return releasesData;
}
