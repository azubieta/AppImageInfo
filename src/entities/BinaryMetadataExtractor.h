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
protected:
    QString getBinaryArch() const;
    QString getSha512CheckSum() const;
    qint64 getFileSize() const;
    int getAppImageType() const;

    QDateTime getTime() const;
};

#endif //APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
