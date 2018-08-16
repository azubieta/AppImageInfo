//
// Created by alexis on 6/5/18.
//

#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QtCore/QUuid>
#include <appimage/appimage.h>

#include "FileMetadataExtractor.h"
#include "DesktopFileMetadataExtractor.h"
#include "AppStreamMetadataExtractor.h"
#include "BinaryMetadataExtractor.h"
#include "FileMetadataMerger.h"

FileMetadataExtractor::FileMetadataExtractor(const QString &path)
        : path(path) {}

QStringList FileMetadataExtractor::loadFileList() {
    QStringList fileList;
    char **rawFileList = appimage_list_files(path.toStdString().c_str());
    if (rawFileList == nullptr || *rawFileList == nullptr)
        throw std::runtime_error(std::string("Unable to list files in ") + path.toStdString());

    for (int i = 0; rawFileList[i] != nullptr; i++)
        fileList << QString(rawFileList[i]);

    appimage_string_list_free(rawFileList);
    return fileList;
}

QVariantMap FileMetadataExtractor::extractDesktopFileData() {
    QSharedPointer<QDir> tmpDir{tryGetTmpDir(), [](QDir *dir) {
        dir->removeRecursively();
        delete dir;
    }};

    auto desktopFileName = tryGetDesktopFileName(list);
    tryExtractFile(desktopFileName, tmpDir.data());

    auto tmpDesktopFileName = tmpDir->absoluteFilePath(desktopFileName);

    DesktopFileMetadataExtractor extractor(tmpDesktopFileName);
    auto data = extractor.getContent();

    return data;
}

QDir *FileMetadataExtractor::getTmpDir() const {
    const char *tmp_dir_path = "/tmp/appimage_indexer";
    auto tmpDir = new QDir(tmp_dir_path);
    if (!tmpDir->exists())
        tmpDir->mkpath(tmp_dir_path);

    return tmpDir;
}

QString FileMetadataExtractor::getDesktopFileName(const QStringList &list) const {
    QString desktopfileName;

    for (auto file : list) {
        if (!file.contains("/") && file.endsWith(".desktop", Qt::CaseInsensitive))
            desktopfileName = file;
    }

    return desktopfileName;
}

QDir *FileMetadataExtractor::tryGetTmpDir() const {
    auto dir = getTmpDir();

    if (dir == nullptr || !dir->exists())
        throw std::runtime_error(std::string("Unable to create tmp dir: ") + dir->absolutePath().toStdString());

    return dir;
}

QString FileMetadataExtractor::tryGetDesktopFileName(const QStringList &list) const {
    auto fileName = getDesktopFileName(list);
    if (fileName.isEmpty())
        throw std::runtime_error(std::string("Not .desktop file found."));

    return fileName;
}

void FileMetadataExtractor::tryExtractFile(const QString &filePath, const QDir *targetDir) const {
    appimage_extract_file_following_symlinks(path.toStdString().c_str(), filePath.toStdString().c_str(),
                                             targetDir->absoluteFilePath(filePath).toStdString().c_str());

    if (!targetDir->exists(filePath))
        throw std::runtime_error(std::string("Failed to extract file: ") + filePath.toStdString());
}

QVariantMap FileMetadataExtractor::extractAppStreamFileData() {

    QSharedPointer<QDir> tmpDir{tryGetTmpDir(), [](QDir *dir) {
        dir->removeRecursively();
        delete dir;
    }};

    auto appStreamFileName = tryGetAppStreamFileName(list);
    tryExtractFile(appStreamFileName, tmpDir.data());

    AppStreamMetadataExtractor extractor(tmpDir->absoluteFilePath(appStreamFileName));
    return extractor.getContent();
}

QString FileMetadataExtractor::tryGetAppStreamFileName(QStringList list) {
    QString fileName;

    for (auto file : list)
        if (file.contains("usr/share/metainfo") && file.endsWith(".appdata.xml", Qt::CaseInsensitive)) {
            fileName = file;
            break;
        }

    if (fileName.isEmpty())
        throw std::runtime_error(std::string("Not .appdata.xml file found."));

    return fileName;
}

FileMetadataExtractor::FileMetadataExtractor() {

}

const QString &FileMetadataExtractor::getPath() const {
    return path;
}

void FileMetadataExtractor::setPath(const QString &path) {
    FileMetadataExtractor::path = path;
}

QVariantMap FileMetadataExtractor::extractMetadata() {
    list = loadFileList();
    QVariantMap desktop;
    QVariantMap appStream;
    QVariantMap binary;
    try {
        desktop = extractDesktopFileData();
    }
    catch (std::runtime_error &e) {
        qWarning() << e.what();
    }

    try {
        appStream = extractAppStreamFileData();
    }
    catch (std::runtime_error &e) {
        qWarning() << e.what();
    }

    try {
        binary = extractBinaryFileData();
    }
    catch (std::runtime_error &e) {
        qWarning() << e.what();
    }

    MetadataMerger merger;
    merger.setAppStream(appStream);
    merger.setDesktop(desktop);
    merger.setBinary(binary);

    return merger.getOutput();
}

QVariantMap FileMetadataExtractor::extractBinaryFileData() {
    BinaryMetadataExtractor extractor(path.toStdString());
    return extractor.getMetadata();
}

QByteArray FileMetadataExtractor::extractIcon() {
    auto tmpFile = "/tmp/appimage-icon-" + QUuid::createUuid().toString().remove('{').remove('}');

    appimage_extract_file_following_symlinks(path.toStdString().c_str(), ".DirIcon", tmpFile.toStdString().c_str());

    QByteArray icon;
    QFile f(tmpFile);
    if (f.open(QIODevice::ReadOnly)) {
        icon = f.readAll();
        f.close();
        f.remove();
    } else
        qWarning() << "Unable to extract application icon.";

    return icon;
}

void FileMetadataExtractor::extractDesktopFile(const std::string &outputPath) {
    auto desktopFileName = tryGetDesktopFileName(list);
    appimage_extract_file_following_symlinks(path.toStdString().c_str(), desktopFileName.toStdString().c_str(),
                                             outputPath.c_str());
}

void FileMetadataExtractor::extractAppStreamFile(const char *targetPath) {
    auto appstreamFileName = tryGetAppStreamFileName(list);
    appimage_extract_file_following_symlinks(path.toStdString().c_str(), appstreamFileName.toStdString().c_str(),
                                             targetPath);
}

void FileMetadataExtractor::extractIconFile(const char *targetPath, const char *iconSize) {
    // Search for an icon of the required size in
    for (const auto &file: list) {
        if (file.contains("/usr/share/icons/hicolor") && file.contains(iconSize)) {
            appimage_extract_file_following_symlinks(path.toStdString().c_str(), file.toStdString().c_str(),
                                                     targetPath);
            return;
        }
    }

    // Fallback to the .AppIcon
    appimage_extract_file_following_symlinks(path.toStdString().c_str(), ".DirIcon", targetPath);
}
