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
    data["id"] = appstream.contains("id") ? appstream.value("id") : desktop.value("id");
    data["name"] = desktop.value("Name");
    data["description"] = appstream.contains("description") ? appstream.value("description") : desktop.value("Comment");
    data["categories"] = desktop.value("Categories");
    data["developer_name"] = appstream.value("developer_name");
    data["links"] = appstream.value("urls");
    data["screenshots"] = appstream.value("screenshots");
    data["releases"] = appstream.value("releases");
    data["MimeType"] = desktop.value("MimeType");
    return data;
}
