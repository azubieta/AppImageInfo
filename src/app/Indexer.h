//
// Created by alexis on 6/18/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_INDEXER_H
#define APPIMAGE_RELEASES_INDEXER_INDEXER_H


#include <QtCore/QObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QCoreApplication>
#include "../entities/GitHubProjectIndexer.h"
#include "../entities/FileMetadataExtractor.h"

class Indexer : public QObject {
Q_OBJECT
    QString url;
    GitHubProjectIndexer *ghIndexer;
    RemoteAppImageMetadataExtractor *appImageMetadataExtractor;
    FileMetadataExtractor *fileMetadataExtractor;
public:
    Indexer(const QString &url, QObject *parent = nullptr);

    void execute();

protected slots:

    void handleGitHubProjectIndexerCompleted();

    void handleRemoteAppImageMetadataExtractorCompleted();

    void printMetadataAsJson(const QVariantMap &metadata) const;
};


#endif //APPIMAGE_RELEASES_INDEXER_INDEXER_H
