//
// Created by alexis on 6/14/18.
//

#include <QCryptographicHash>
#include <QtCore/QUuid>
#include <QtCore/QJsonDocument>
#include "RemoteAppImageMetadataExtractor.h"
#include "FileMetadataExtractor.h"

RemoteAppImageMetadataExtractor::RemoteAppImageMetadataExtractor(const QString &url, QObject *parent)
        : QObject(parent), url(url), downloader(nullptr) {

    tmpFile = QDir::tempPath() + "/" + QUuid::createUuid().toString().mid(2, 34) + ".AppImage";
    downloader = new FileDownloader(url, tmpFile, this);
    connect(downloader, &FileDownloader::downloadCompleted, this,
            &RemoteAppImageMetadataExtractor::handleDownloadFinished);
    connect(downloader, &FileDownloader::unmodified, this,
            &RemoteAppImageMetadataExtractor::handleUnmodified);
}

void RemoteAppImageMetadataExtractor::run() {
    QVariantMap metadataCache = getMetadataCache();
    qInfo() << "GET: " << url;
    qInfo() << "If-Modified-Since: " << metadataCache["url_date"].toDateTime().toString();
    downloader->setCacheDate(metadataCache["url_date"].toDateTime());
    downloader->startDownload();
}

void RemoteAppImageMetadataExtractor::handleDownloadFinished() {
    if (downloader->isFailed())
        qWarning() << "Download failed";
    else
        try {
            FileMetadataExtractor extractor(tmpFile);
            metadata = extractor.extractMetadata();
            auto httpHeaders = downloader->getHeaders();
            metadata["url_date"] = httpHeaders["last-modified"];

            saveMetadataCache();
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

void RemoteAppImageMetadataExtractor::setCacheDir(const QString &cacheDir) {
    RemoteAppImageMetadataExtractor::cacheDir = cacheDir;
}

void RemoteAppImageMetadataExtractor::handleUnmodified() {
    qInfo() << "Unmodified resource, using metadata cache.";
    QVariantMap metadataCache = getMetadataCache();

    metadata = metadataCache;
    emit completed();
}

void RemoteAppImageMetadataExtractor::saveMetadataCache() const {
    if (!cacheDir.isEmpty()) {
        QDir dir(cacheDir);
        if (dir.mkpath(dir.absolutePath())) {
            QString fileName = QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Sha1).toHex();
            QFile f(dir.absoluteFilePath(fileName));
            if (f.open(QIODevice::WriteOnly)) {
                auto doc = QJsonDocument::fromVariant(metadata);
                f.write(doc.toJson());
                f.close();
            }
        }
    }
}

QVariantMap RemoteAppImageMetadataExtractor::getMetadataCache() const {
    QVariantMap cacheMetadata;
    QDir dir(cacheDir);
    QString fileName = QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Sha1).toHex();
    if (dir.exists(fileName)) {
        QFile f(dir.absoluteFilePath(fileName));
        if (f.open(QIODevice::ReadOnly)) {
            auto content = f.readAll();
            auto document = QJsonDocument::fromJson(content);
            cacheMetadata = document.toVariant().toMap();
            f.close();
        }
    }
    return cacheMetadata;
}
