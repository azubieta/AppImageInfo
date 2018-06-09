//
// Created by alexis on 6/8/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H

#include <QString>
#include <QVariantMap>
class BinaryMetadataExtractor {
    QString target;
public:
    BinaryMetadataExtractor(const QString& target);
    QVariantMap getMetadata();
    QString getBinaryArch() const;
    QString getSha512CheckSum() const;
};

#endif //APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
