//
// Created by alexis on 6/14/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_REMOTEAPPIMAGEMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_REMOTEAPPIMAGEMETADATAEXTRACTOR_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QTemporaryFile>
#include <QSharedPointer>
#include "../gateways/FileDownloader.h"

class RemoteAppImageMetadataExtractor : public QObject {
Q_OBJECT
    QString url;
    QVariantMap metadata;
    QString tmpFile;
    FileDownloader *downloader;
    QString cacheDir;
public:
    explicit RemoteAppImageMetadataExtractor(const QString &url, QObject *parent = nullptr);

    void run();

    const QVariantMap &getMetadata() const;

    const QString &getUrl() const;

    void setCacheDir(const QString &cacheDir);

signals:

    void completed();

protected slots:

    void handleDownloadFinished();

    void handleUnmodified();

protected:

    QVariantMap getMetadataCache() const;

    void saveMetadataCache() const;

};

#endif //APPIMAGE_RELEASES_INDEXER_REMOTEAPPIMAGEMETADATAEXTRACTOR_H
