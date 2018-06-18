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
    MetadataMerger::appStream = appstream;
}
MetadataMerger::MetadataMerger() { }
QVariantMap MetadataMerger::merge()
{
    data = QVariantMap();
    auto desktopEntry = desktop["Desktop Entry"].toMap();
    data["id"] = appStream.contains("id") ? appStream.value("id") : desktop.value("id");
    data["name"] = desktopEntry.value("Name");
    mergeDescription(desktopEntry);
    data["categories"] = desktopEntry.value("Categories");
    data["developer_name"] = appStream.value("developer_name");
    data["links"] = appStream.value("urls");
    data["screenshots"] = appStream.value("screenshots");
    mergeRelease();
    data["MimeType"] = desktopEntry.value("MimeType");

    data["architecture"] = binary["architecture"];
    data["sha512checksum"] = binary["sha512checksum"];
    data["size"] = binary["size"];
    data["type"] = binary["type"];

    removeEmptyFields();
    return data;
}
void MetadataMerger::removeEmptyFields()
{

    for (const auto& key: data.keys()) {
        const auto& value = data.value(key);
        if (value.isNull() || !value.isValid())
            data.remove(key);
    }
}
void MetadataMerger::mergeRelease()
{
    QVariantMap release;
    auto appStreamReleases = appStream.value("releases").toList();
    if (appStreamReleases.size()>0) {
        QMap<QString, QVariant> latestAppStreamRelease;
        latestAppStreamRelease = appStreamReleases.first().toMap();

        release["version"] = latestAppStreamRelease["version"];
        release["date"] = latestAppStreamRelease["date"];
        release["urgency"] = latestAppStreamRelease["urgency"];
        release["channel"] = latestAppStreamRelease["type"];
        release["changes"] = latestAppStreamRelease["description"];
    }

    if (!release.isEmpty())
        data["release"] = release;
}
void MetadataMerger::mergeDescription(const QMap<QString, QVariant>& desktopEntry)
{
    QVariantMap description;
    if (appStream.contains("description"))
        description["null"] = appStream.value("description");
    else
        description = desktopEntry.value("Comment").toMap();
    data["description"] = description;
}
void MetadataMerger::setBinary(const QVariantMap& binary)
{
    MetadataMerger::binary = binary;
}
