//
// Created by alexis on 6/6/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H

#include <QString>
#include <QVariant>

class AppStreamMetadataExtractor {
    QString filePath;
public:
    explicit AppStreamMetadataExtractor(const QString& filePath);
    QVariantMap getContent();
};

#endif //APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H
