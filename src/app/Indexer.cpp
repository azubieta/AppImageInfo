//
// Created by alexis on 6/18/18.
//

#include <QDebug>
#include "Indexer.h"
#include "../entities/FileMetadataExtractor.h"
#include "../entities/AppInfoBuilder.h"


Indexer::Indexer(QObject *parent) : QObject(parent) {
}

void Indexer::execute() {
    if (GitHubProjectIndexer::isGitHubProject(url)) {
        ghIndexer = new GitHubProjectIndexer(url);
        connect(ghIndexer, &GitHubProjectIndexer::completed, this, &Indexer::handleGitHubProjectIndexerCompleted);
        ghIndexer->setCacheDir(cacheDir);
        ghIndexer->run();
        return;
    }

    if (url.startsWith("http")) {
        appImageMetadataExtractor = new RemoteAppImageMetadataExtractor(url);
        connect(appImageMetadataExtractor, &RemoteAppImageMetadataExtractor::completed,
                this, &Indexer::handleRemoteAppImageMetadataExtractorCompleted);
        appImageMetadataExtractor->setCacheDir(cacheDir);
        appImageMetadataExtractor->run();
        return;
    }

    if (QFile::exists(url)) {
        fileMetadataExtractor = new FileMetadataExtractor();
        fileMetadataExtractor->setPath(url);
        auto metadata = fileMetadataExtractor->extractMetadata();

        QVariantMap output = buildAppInfo(metadata);

        printMetadataAsJson(output);
        exit(0);
    }

    qCritical() << "Unable to process " << url;
    exit(1);
}

QVariantMap Indexer::buildAppInfo(const QVariantMap &metadata) const {
    AppInfoBuilder appInfoBuilder;
    appInfoBuilder.setFormat(1);
    appInfoBuilder.fillMissingAppInfoFields(metadata);

    QVariantMap release;
    release["channel"] = "unknown";
    release["version"] = "date_based";
    if (metadata.contains("url_date"))
        release["date"] = metadata["url_date"];
    else
        release["date"] = metadata["file_date"];

    QVariantMap file;
    file["size"] = metadata["size"];
    file["sha512checksum"] = metadata["sha512checksum"];
    file["type"] = metadata["type"];
    release["files"] = QVariantList{file};

    appInfoBuilder.addRelease(release);
    auto output = appInfoBuilder.build();
    return output;
}

void Indexer::handleGitHubProjectIndexerCompleted() {
    auto metadata = ghIndexer->getAppInfo();
    printMetadataAsJson(metadata);

    exit(0);
}


void Indexer::handleRemoteAppImageMetadataExtractorCompleted() {
    auto metadata = appImageMetadataExtractor->getMetadata();
    printMetadataAsJson(buildAppInfo(metadata));

    exit(0);
}


void Indexer::printMetadataAsJson(const QVariantMap &metadata) const {
    auto doc = QJsonDocument::fromVariant(metadata);
    QTextStream textStream(stdout);
    textStream << doc.toJson();
}

void Indexer::setUrl(const QString &url) {
    Indexer::url = url;
}

void Indexer::setCacheDir(const QString &cacheDir) {
    Indexer::cacheDir = cacheDir;
}
