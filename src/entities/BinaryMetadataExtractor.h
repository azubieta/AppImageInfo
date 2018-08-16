//
// Created by alexis on 6/8/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H

#include <QString>
#include <QVariantMap>
#include <bfd.h>

class BadFileFormat : public std::runtime_error {
public:
    explicit BadFileFormat(const std::string &__arg);
};

class BinaryMetadataExtractor {
    std::string target;
    bfd *abfd;
public:
    explicit BinaryMetadataExtractor(const std::string &target);

    virtual ~BinaryMetadataExtractor();

    QVariantMap getMetadata();

protected:
    QString getSha512CheckSum() const;

    std::string getBinaryArch() const;

    qint64 getFileSize() const;

    int getAppImageType() const;

    QDateTime getTime() const;
};

#endif //APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
