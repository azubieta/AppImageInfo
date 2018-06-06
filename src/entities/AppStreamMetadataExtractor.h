//
// Created by alexis on 6/6/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H

#include <QString>
#include <QVariant>
#include <QXmlStreamReader>

class AppStreamMetadataExtractor {
    QString filePath;
    QVariantMap data;
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType token;
    QString tokenName;
public:
    explicit AppStreamMetadataExtractor(const QString& filePath);
    QVariantMap getContent();

protected:
    QByteArray readFile() const;
    QVariantList readScreenshots();
    QVariantMap readScreenShot();
    QString readHtml();
    void readUrl();
};

#endif //APPIMAGE_RELEASES_INDEXER_APPSTREAMMETADATAEXTRACTOR_H
