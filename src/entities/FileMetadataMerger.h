//
// Created by alexis on 6/7/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
#define APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H

#include <QVariantMap>

class MetadataMerger {
    QVariantMap desktop;
    QVariantMap appStream;
    QVariantMap binary;
    QVariantMap data;
public:

    MetadataMerger();
    void setDesktop(const QVariantMap& desktop);
    void setAppStream(const QVariantMap& appstream);
    void setBinary(const QVariantMap& binary);

    QVariantMap merge();
protected:
    void mergeDescription(const QMap<QString, QVariant>& desktopEntry);
    void mergeRelease();
    void removeEmptyFields();
};

#endif //APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
