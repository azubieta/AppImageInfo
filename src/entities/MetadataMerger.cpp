//
// Created by alexis on 6/7/18.
//

#include "MetadataMerger.h"
void MetadataMerger::setDesktop(const QVariantMap& desktop)
{
    MetadataMerger::desktop = desktop;
}
void MetadataMerger::setAppStream(const QVariantMap& appstream)
{
    MetadataMerger::appstream = appstream;
}
MetadataMerger::MetadataMerger() { }
QVariantMap MetadataMerger::merge()
{
    data = QVariantMap();
    auto desktopEntry = desktop["Desktop Entry"].toMap();
    data["id"] = appstream.contains("id") ? appstream.value("id") : desktop.value("id");
    data["name"] = desktopEntry.value("Name");
    mergeDescription(desktopEntry);
    data["categories"] = desktopEntry.value("Categories");
    data["developer_name"] = appstream.value("developer_name");
    data["links"] = appstream.value("urls");
    data["screenshots"] = appstream.value("screenshots");
    data["releases"] = appstream.value("releases");
    data["MimeType"] = desktopEntry.value("MimeType");
    return data;
}
void MetadataMerger::mergeDescription(const QMap<QString, QVariant>& desktopEntry)
{
    QVariantMap description;
    if (appstream.contains("description"))
        description["default"] = appstream.value("description");
    else
        description = desktopEntry.value("Comment").toMap();
    data["description"] = description;
}
