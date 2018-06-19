//
// Created by alexis on 6/18/18.
//

#include <QDebug>
#include "Indexer.h"
#include "../entities/FileMetadataExtractor.h"
#include "../entities/AppInfoAssembler.h"


Indexer::Indexer(const QString &url, QObject *parent) : QObject(parent), url(url) {
}

void Indexer::execute() {
    if (GitHubProjectIndexer::isGitHubProject(url)) {
        ghIndexer = new GitHubProjectIndexer(url);
        connect(ghIndexer, &GitHubProjectIndexer::completed, this, &Indexer::handleGitHubProjectIndexerCompleted);
        ghIndexer->run();
        return;
    }

    if (url.startsWith("http")) {
        appImageMetadataExtractor = new RemoteAppImageMetadataExtractor(url);
        connect(appImageMetadataExtractor, &RemoteAppImageMetadataExtractor::completed,
                this, &Indexer::handleRemoteAppImageMetadataExtractorCompleted);
        appImageMetadataExtractor->run();
        return;
    }

    if (QFile::exists(url)) {
        fileMetadataExtractor = new FileMetadataExtractor();
        fileMetadataExtractor->setPath(url);
        auto metadata = fileMetadataExtractor->extractMetadata();
        printMetadataAsJson(metadata);
        exit(0);
    }

    qCritical() << "Unable to process " << url;
    exit(1);
}

void Indexer::handleGitHubProjectIndexerCompleted() {
    auto metadata = ghIndexer->getAppInfo();
    printMetadataAsJson(metadata);

    exit(0);
}


void Indexer::handleRemoteAppImageMetadataExtractorCompleted() {
    auto metadata = appImageMetadataExtractor->getMetadata();
    printMetadataAsJson(metadata);

    exit(0);
}


void Indexer::printMetadataAsJson(const QVariantMap &metadata) const {
    auto doc = QJsonDocument::fromVariant(metadata);
    QTextStream textStream(stdout);
    textStream << doc.toJson();
}