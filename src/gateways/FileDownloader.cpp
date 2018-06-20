//
// Created by alexis on 6/13/18.
//

#include "FileDownloader.h"

FileDownloader::FileDownloader(const QString &url, QString target, QObject *parent)
        : QObject(parent), url(url), errored(false), working(false), manager(new QNetworkAccessManager(this)),
          file(nullptr), target(target), reply(nullptr) {
}

bool FileDownloader::isFailed() const {
    return errored;
}

void FileDownloader::startDownload() {
    if (working)
        throw std::runtime_error("Download already running.");

    working = true;
    connect(manager, &QNetworkAccessManager::finished, this, &FileDownloader::handleDownloadFinished);

    file.setFileName(target);
    if (!file.open(QIODevice::WriteOnly))
        throw std::runtime_error("Unable to open file: " + target.toStdString());


    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setRawHeader("If-Modified-Since", cacheDate.toString().toLocal8Bit());
    if (reply != nullptr)
        reply->deleteLater();
    reply = manager->get(request);
    connect(reply, &QIODevice::readyRead, this, &FileDownloader::handleReadyRead);

    working = true;
    errored = false;
}

void FileDownloader::handleDownloadFinished(QNetworkReply *reply) {
    disconnect(manager, &QNetworkAccessManager::finished, this, &FileDownloader::handleDownloadFinished);
    errored = reply->error() != QNetworkReply::NoError;
    working = false;

    auto lastModifiedHeader = reply->header(QNetworkRequest::LastModifiedHeader);
    headers["last-modified"] = lastModifiedHeader;

    if (reply->operation() == QNetworkAccessManager::GetOperation) {
        if (errored)
            file.remove();
        else {
            handleReadyRead();
            file.close();
        }

        reply->deleteLater();
    }

    auto responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (responseCode == 304 || lastModifiedHeader == cacheDate)
            emit unmodified();
    else
            emit downloadCompleted();
}

void FileDownloader::handleReadyRead() {
    const auto data = reply->readAll();
    file.write(data);
}

const QVariantMap &FileDownloader::getHeaders() const {
    return headers;
}

void FileDownloader::setCacheDate(const QDateTime &cacheDate) {
    FileDownloader::cacheDate = cacheDate;
}
