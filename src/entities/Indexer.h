//
// Created by alexis on 6/9/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_INDEXER_H
#define APPIMAGE_RELEASES_INDEXER_INDEXER_H

#include <QStringList>
class Indexer {
    QStringList urls;
public:
    Indexer(const QStringList& urls);
    void run();
};

#endif //APPIMAGE_RELEASES_INDEXER_INDEXER_H
