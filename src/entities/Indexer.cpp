//
// Created by alexis on 6/9/18.
//

#include "Indexer.h"
#include "GitHubProjectIndexer.h"
Indexer::Indexer(const QStringList& urls)
        :urls(urls) { }
void Indexer::run()
{
    for (const auto url: urls) {
        if (GitHubProjectIndexer::isGitHubProject(url)) {

        }
    }
}
