//
// Created by alexis on 6/5/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_METADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_METADATAEXTRACTOR_H

#include <QString>

class MetadataExtractor {
    QString path;
    QString fileList;
public:
    MetadataExtractor(const QString& path);
    // Throws runtime_error when isn't possible to get the files list;
    QStringList loadFileList();

    QVariantMap parseDesktopFile(QStringList list);

protected:
    QString getDesktopFileName(const QStringList& list) const;
    QString tryGetDesktopFileName(const QStringList& list) const;
    QDir* getTmpDir() const;
    QDir* tryGetTmpDir() const;
    void tryExtractFile(const QString& filePath, const QDir*  targetDir) const;
};

#endif //APPIMAGE_RELEASES_INDEXER_METADATAEXTRACTOR_H
