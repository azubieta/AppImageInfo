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
    QNetworkAccessManager* manager;
    QFile file;
    QNetworkReply *reply;
public:
    FileDownloader(const QString& url, QString target, QObject* parent = nullptr);

    bool isFailed() const;
    const QString& getUrl() const;
    const QString& getTarget() const;

public slots:
    void start();

protected slots:
    void handleDownloadFinished(QNetworkReply* reply);
    void handleReadyRead();

signals:
    void finished();
};

#endif //APPIMAGE_RELEASES_INDEXER_FILEDOWNLOADER_H
