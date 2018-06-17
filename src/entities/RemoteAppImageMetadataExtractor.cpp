//
// Created by alexis on 6/14/18.
//

#include <QtCore/QUuid>
#include "RemoteAppImageMetadataExtractor.h"
#include "MetadataExtractor.h"

RemoteAppImageMetadataExtractor::RemoteAppImageMetadataExtractor(const QString &url, QObject *parent)
        : QObject(parent), url(url) {

    tmpFile = QDir::tempPath() + "/" + QUuid::createUuid().toString().mid(2,34) + ".AppImage";
    downloader.reset(new FileDownloader(url, tmpFile));
}

void RemoteAppImageMetadataExtractor::run() {
    connect(downloader.data(), &FileDownloader::finished, this,
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

    emit completed();
}

const QVariantMap &RemoteAppImageMetadataExtractor::getMetadata() const {
    return metadata;
}
