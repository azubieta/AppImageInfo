//
// Created by alexis on 6/5/18.
//

#include <QtCore/QFile>
#include <QtCore/QSet>
#include "DesktopFileMetadataExtractor.h"
DesktopFileMetadataExtractor::DesktopFileMetadataExtractor(const QString& filePath)
        :filePath(filePath) { }
QVariantMap DesktopFileMetadataExtractor::getContent()
{
    tryReadLines();
    parseLines();
    return data;
}
void DesktopFileMetadataExtractor::parseApplicationId()
{
    data["id"] = filePath.section("/", -1, -1);
}
void DesktopFileMetadataExtractor::parseLines()
{
    data = QVariantMap();
    currentGroupName = QString();
    currentGroup = QVariantMap();

    parseApplicationId();

    for (const auto& line: fileContent) {
        auto cleanLine = removeComments(line);
        if (isGroupEntry(cleanLine))
            handleGroupEntry(cleanLine);

        if (isKeyEntry(cleanLine))
            handleKeyEntry(cleanLine);
    }

    saveLastGroup();
}
void DesktopFileMetadataExtractor::saveLastGroup()
{
    if (!currentGroupName.isEmpty())
        saveCurrentGroup();
}
void DesktopFileMetadataExtractor::handleGroupEntry(const QString& cleanLine)
{
    auto groupName = cleanLine.mid(1, cleanLine.length()-2);

    if (!currentGroupName.isEmpty())
        saveCurrentGroup();

    if (currentGroupName.isEmpty())
        currentGroupName = groupName;
}
void DesktopFileMetadataExtractor::handleKeyEntry(const QString& cleanLine)
{
    auto full_key = cleanLine.section('=', 0, 0);
    auto key = extractKey(full_key);
    auto locale = extractLocale(full_key);
    auto value = extractValue(cleanLine, key);

    if (isLocaleStringKey(key)) {
        auto localeStringsGroup = currentGroup.value("key", QVariantMap()).toMap();
        localeStringsGroup[locale] = value;
        currentGroup[key] = localeStringsGroup;
    }
    else
        currentGroup[key] = value;
}
void DesktopFileMetadataExtractor::saveCurrentGroup()
{
    data[currentGroupName] = currentGroup;
    currentGroupName = QString();
    currentGroup = QVariantMap();
}
QString DesktopFileMetadataExtractor::extractLocale(const QString& full_key) const
{
    QRegExp sep("[\\[\\]]");
    auto locale = full_key.section(sep, 1, 1);
    locale = locale.isEmpty() ? "default" : locale;
    return locale;
}
QString DesktopFileMetadataExtractor::extractKey(const QString& full_key) const
{
    QRegExp sep("[\\[\\]]");
    auto key = full_key.section(sep, 0, 0);
    return key;
}
bool DesktopFileMetadataExtractor::isLocaleStringKey(const QString& key) const
{
    static const QSet<QString> localeStrings{"Name", "GenericName", "Comment", "Icon", "Keywords"};
    return localeStrings.contains(key);
}
QVariant DesktopFileMetadataExtractor::extractValue(const QString& cleanLine, const QString& key) const
{
    static const QSet<QString> arrayValues{"OnlyShowIn", "NotShowIn", "Actions", "MimeType", "Categories",
                                           "Implements", "Keywords"};
    QVariant value = cleanLine.section('=', 1, -1);;
    if (arrayValues.contains(key)) {
        auto rawList = value.toString().split(";");
        QVariantList list;
        for (const auto& item: rawList)
            if (!item.isEmpty())
                list << item.trimmed();
        value = list;
    }

    return value;
}
bool DesktopFileMetadataExtractor::isKeyEntry(const QString& cleanLine) const { return cleanLine.contains("="); }
bool DesktopFileMetadataExtractor::isGroupEntry(const QString& cleanLine) const
{
    return cleanLine.startsWith('[') && cleanLine.endsWith(']');
}
QString DesktopFileMetadataExtractor::removeComments(const QString& cleanLine) const
{
    return cleanLine.section('#', 0, 0).simplified().trimmed();
}
void DesktopFileMetadataExtractor::tryReadLines()
{
    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly)) {
        while (!f.atEnd())
            fileContent << f.readLine();
    }
    else
        throw std::runtime_error("Unable to open file: "+filePath.toStdString());
}
