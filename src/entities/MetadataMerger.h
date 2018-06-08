//
// Created by alexis on 6/7/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
#define APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H

#include <QVariantMap>

class MetadataMerger {
    QVariantMap desktop;
    QVariantMap appstream;
    QVariantMap data;
public:

    MetadataMerger();
    void setDesktop(const QVariantMap& desktop);
    void setAppStream(const QVariantMap& appstream);

    QVariantMap merge();

};

#endif //APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
