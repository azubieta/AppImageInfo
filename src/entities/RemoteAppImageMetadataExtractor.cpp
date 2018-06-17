//
// Created by alexis on 6/14/18.
//

#include "RemoteAppImageMetadataExtractor.h"
RemoteAppImageMetadataExtractor::RemoteAppImageMetadataExtractor(const QString& url, QObject* parent)
        :QObject(parent), url(url)
{
    downloader.reset(new FileDownloader(url, file.fileName()));
}
void RemoteAppImageMetadataExtractor::run()
{
    connect(downloader.data(), &FileDownloader::finished, this, &RemoteAppImageMetadataExtractor::handleDownloadFinished);
    downloader->start();

}
void RemoteAppImageMetadataExtractor::handleDownloadFinished()
{
    emit completed();
}
