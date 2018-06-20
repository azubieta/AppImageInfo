//
// Created by alexis on 6/13/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_FILEDOWNLOADER_H
#define APPIMAGE_RELEASES_INDEXER_FILEDOWNLOADER_H

#include <QObject>
#include <QFile>
#include <QSharedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class FileDownloader : public QObject {
Q_OBJECT
    QString url;
    QString target;
    bool errored;
    bool working;
    QNetworkAccessManager *manager;
    QFile file;
    QVariantMap headers;
    QNetworkReply *reply;
    QDateTime cacheDate;
public:
    FileDownloader(const QString &url, QString target, QObject *parent = nullptr);

    bool isFailed() const;

    const QVariantMap &getHeaders() const;

    void setCacheDate(const QDateTime &cacheDate);

public slots:

    void startDownload();

protected slots:

    void handleDownloadFinished(QNetworkReply *reply);

    void handleReadyRead();

signals:

    void downloadCompleted();

    void headersReady(const QVariantMap headers);

    void unmodified();

};

#endif //APPIMAGE_RELEASES_INDEXER_FILEDOWNLOADER_H
