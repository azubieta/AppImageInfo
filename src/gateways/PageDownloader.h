#ifndef APPIMAGE_RELEASES_INDEXER_PAGE_DOWNLOADER_H
#define APPIMAGE_RELEASES_INDEXER_PAGE_DOWNLOADER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class PageDownloader : public QObject {
Q_OBJECT
    QString url;
    QNetworkAccessManager *manager;
    QByteArray data;
    bool working;
    bool errored;
public:
    explicit PageDownloader(const QString& url, QObject* parent = nullptr);
    ~PageDownloader() override;
    bool isWorking() const;
    bool isErrored() const;

    const QString& getUrl() const;
    void setUrl(const QString& url);
public slots:
    void download();
    QByteArray getData();
signals:
    void finished();

protected slots:
    void handleDownloadFinished(QNetworkReply* reply);
};

#endif //APPIMAGE_RELEASES_INDEXER_PAGE_DOWNLOADER_H
