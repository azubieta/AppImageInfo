#include "PageDownloader.h"
#include <QNetworkRequest>

PageDownloader::PageDownloader(const QString &url, QObject *parent)
        : QObject(parent), url(url), manager(new QNetworkAccessManager(this)), working(false), errored(false) {
    connect(manager, &QNetworkAccessManager::finished, this, &PageDownloader::handleDownloadFinished);
}

PageDownloader::~PageDownloader() {

}

void PageDownloader::download() {
    if (isWorking())
        throw std::runtime_error("Download already running");

    QNetworkRequest request(url);
    manager->get(request);

    working = true;
    errored = false;
}

QByteArray PageDownloader::getData() {
    return data;
}

void PageDownloader::handleDownloadFinished(QNetworkReply *reply) {
    working = false;
    errored = reply->error() != QNetworkReply::NoError;

    data = reply->readAll();
    reply->deleteLater();

    emit finished();
}

bool PageDownloader::isWorking() const {
    return working;
}

bool PageDownloader::isErrored() const {
    return errored;
}

const QString &PageDownloader::getUrl() const {
    return url;
}

void PageDownloader::setUrl(const QString &url) {
    PageDownloader::url = url;
}
