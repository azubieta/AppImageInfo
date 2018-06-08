//
// Created by alexis on 6/5/18.
//

#include <appimage/appimage.h>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include <QSharedPointer>
#include <QDebug>

#include "MetadataExtractor.h"
#include "DesktopFileMetadataExtractor.h"
#include "AppStreamMetadataExtractor.h"
#include "MetadataMerger.h"

MetadataExtractor::MetadataExtractor(const QString& path)
        :path(path) { }
QStringList MetadataExtractor::loadFileList()
{
    QStringList fileList;
    char** rawFileList = appimage_list_files(path.toStdString().c_str());
    if (rawFileList==nullptr || *rawFileList==nullptr)
        throw std::runtime_error(std::string("Unable to list files in ")+path.toStdString());

    for (int i = 0; rawFileList[i]!=nullptr; i++)
        fileList << QString(rawFileList[i]);

    appimage_string_list_free(rawFileList);
    return fileList;
}
QVariantMap MetadataExtractor::extractDesktopFileData()
{
    QSharedPointer<QDir> tmpDir{tryGetTmpDir(), [](QDir* dir) {
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
QDir* MetadataExtractor::getTmpDir() const
{
    const char* tmp_dir_path = "/tmp/appimage_indexer";
    auto tmpDir = new QDir(tmp_dir_path);
    if (!tmpDir->exists())
        tmpDir->mkpath(tmp_dir_path);

    return tmpDir;
}
QString MetadataExtractor::getDesktopFileName(const QStringList& list) const
{
    QString desktopfileName;

    for (auto file : list) {
        if (!file.contains("/") && file.endsWith(".desktop", Qt::CaseInsensitive))
            desktopfileName = file;
    }

    return desktopfileName;
}
QDir* MetadataExtractor::tryGetTmpDir() const
{
    auto dir = getTmpDir();

    if (dir==nullptr || !dir->exists())
        throw std::runtime_error(std::string("Unable to create tmp dir: ")+dir->absolutePath().toStdString());

    return dir;
}
QString MetadataExtractor::tryGetDesktopFileName(const QStringList& list) const
{
    auto fileName = getDesktopFileName(list);
    if (fileName.isEmpty())
        throw std::runtime_error(std::string("Not .desktop file found."));

    return fileName;
}
void MetadataExtractor::tryExtractFile(const QString& filePath, const QDir* targetDir) const
{
    appimage_extract_file_following_symlinks(path.toStdString().c_str(), filePath.toStdString().c_str(),
            targetDir->absoluteFilePath(filePath).toStdString().c_str());

    if (!targetDir->exists(filePath))
        throw std::runtime_error(std::string("Failed to extract file: ")+filePath.toStdString());
}
QVariantMap MetadataExtractor::extractAppStreamFileData()
{

    QSharedPointer<QDir> tmpDir{tryGetTmpDir(), [](QDir* dir) {
      dir->removeRecursively();
      delete dir;
    }};

    auto appStreamFileName = tryGetAppStreamFileName(list);
    tryExtractFile(appStreamFileName, tmpDir.data());

    AppStreamMetadataExtractor extractor(tmpDir->absoluteFilePath(appStreamFileName));
    return extractor.getContent();
}
QString MetadataExtractor::tryGetAppStreamFileName(QStringList list)
{
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
MetadataExtractor::MetadataExtractor()
{

}
const QString& MetadataExtractor::getPath() const
{
    return path;
}
void MetadataExtractor::setPath(const QString& path)
{
    MetadataExtractor::path = path;
}
QVariantMap MetadataExtractor::extractMetadata()
{
    list = loadFileList();
    QVariantMap desktop;
    QVariantMap appstream;
    try {
        desktop = extractDesktopFileData();
    }
    catch (std::runtime_error& e) {
        qWarning() << e.what();
    }

    try {
        appstream = extractAppStreamFileData();
    }
    catch (std::runtime_error& e) {
        qWarning() << e.what();
    }

    MetadataMerger merger;
    merger.setAppStream(appstream);
    merger.setDesktop(desktop);

    return merger.merge();
}
