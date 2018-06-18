//
// Created by alexis on 6/9/18.
//

#include "Indexer.h"
#include "GitHubProjectIndexer.h"

void Indexer::run() {
    for (const auto url: urls) {

    }
}

Indexer::Indexer(const QStringList &urls, QObject *parent) : QObject(parent), urls(urls) {}

void Indexer::indexNextUrl() {
    if (urls.isEmpty())
            emit completed();
    else {
        auto url = urls.first();
        if (GitHubProjectIndexer::isGitHubProject(url)) {
            gitHubProjectIndexer = new GitHubProjectIndexer(url);
            gitHubProjectIndexer->run();

            connect(gitHubProjectIndexer, &GitHubProjectIndexer::completed, this, &Indexer::handleUrlCompleted);
        }

        urls.removeFirst();
    }

}

void Indexer::handleUrlCompleted() {

    indexNextUrl();
}
