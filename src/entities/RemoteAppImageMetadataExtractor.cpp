//
// Created by alexis on 6/14/18.
//

#include <QtCore/QUuid>
#include "RemoteAppImageMetadataExtractor.h"
#include "MetadataExtractor.h"

RemoteAppImageMetadataExtractor::RemoteAppImageMetadataExtractor(const QString &url, QObject *parent)
        : QObject(parent), url(url), downloader(nullptr) {

    tmpFile = QDir::tempPath() + "/" + QUuid::createUuid().toString().mid(2, 34) + ".AppImage";
    downloader = new FileDownloader(url, tmpFile, this);
    qInfo() << "Downloading " << url;
}

void RemoteAppImageMetadataExtractor::run() {
    connect(downloader, &FileDownloader::finished, this,
            &RemoteAppImageMetadataExtractor::handleDownloadFinished);
    downloader->start();
}

void RemoteAppImageMetadataExtractor::handleDownloadFinished() {
    if (downloader->isFailed())
        qWarning() << "Download failed";
    else
        try {
            MetadataExtractor extractor(tmpFile);
            metadata = extractor.extractMetadata();
        }
        catch (...) {
            qWarning() << "Unable to extract AppImage metadata from " << tmpFile;
        }

    QFile::remove(tmpFile);
    emit completed();
}

const QVariantMap &RemoteAppImageMetadataExtractor::getMetadata() const {
    return metadata;
}

const QString &RemoteAppImageMetadataExtractor::getUrl() const {
    return url;
}
