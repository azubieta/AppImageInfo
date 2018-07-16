//
// Created by alexis on 6/5/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_METADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_METADATAEXTRACTOR_H

#include <QDir>
#include <QString>

#include <CImg.h>

class FileMetadataExtractor {
    QString path;
    QStringList list;
public:
    FileMetadataExtractor();
    explicit FileMetadataExtractor(const QString& path);
    // Throws runtime_error when isn't possible to get the files list;
    QStringList loadFileList();
    const QString& getPath() const;
    void setPath(const QString& path);
    QVariantMap extractMetadata();
    cimg_library::CImg<unsigned char> extractIcon();
protected:
    QVariantMap extractDesktopFileData();
    QVariantMap extractAppStreamFileData();
    QVariantMap extractBinaryFileData();

    QString getDesktopFileName(const QStringList& list) const;
    QString tryGetDesktopFileName(const QStringList& list) const;
    QDir* getTmpDir() const;
    QDir* tryGetTmpDir() const;
    void tryExtractFile(const QString& filePath, const QDir* targetDir) const;
    QString tryGetAppStreamFileName(QStringList list);
};

#endif //APPIMAGE_RELEASES_INDEXER_METADATAEXTRACTOR_H
