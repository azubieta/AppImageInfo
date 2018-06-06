//
// Created by alexis on 6/5/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_DESKTOPFILEMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_DESKTOPFILEMETADATAEXTRACTOR_H

#include <QString>
#include <QVariant>

class DesktopFileMetadataExtractor {
    QString filePath;
    QStringList fileContent;
    QString currentGroupName;
    QVariantMap currentGroup;
    QVariantMap data;
public:
    explicit DesktopFileMetadataExtractor(const QString& filePath);
    QVariantMap getContent();
protected:
    void tryReadLines();
    void parseLines();
    QString removeComments(const QString& cleanLine) const;
    bool isGroupEntry(const QString& cleanLine) const;
    bool isKeyEntry(const QString& cleanLine) const;
    QVariant extractValue(const QString& cleanLine, const QString& key) const;
    bool isLocaleStringKey(const QString& key) const;
    QString extractKey(const QString& full_key) const;
    QString extractLocale(const QString& full_key) const;
    void saveCurrentGroup();
    void handleKeyEntry(const QString& cleanLine);
    void handleGroupEntry(const QString& cleanLine);
    void saveLastGroup();
    void parseApplicationId();
};

#endif //APPIMAGE_RELEASES_INDEXER_DESKTOPFILEMETADATAEXTRACTOR_H
