//
// Created by alexis on 6/9/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_INDEXER_H
#define APPIMAGE_RELEASES_INDEXER_INDEXER_H

#include <QStringList>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include "GitHubProjectIndexer.h"

class Indexer : public QObject {
Q_OBJECT
    QStringList urls;
    QPointer<GitHubProjectIndexer> gitHubProjectIndexer;
public:
    Indexer(const QStringList &urls, QObject *parent = nullptr);

    void run();

signals:
    void completed();

protected:
    void indexNextUrl();

protected slots:
    void handleUrlCompleted();
};

#endif //APPIMAGE_RELEASES_INDEXER_INDEXER_H
