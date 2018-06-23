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

    QVariantMap getOutput();
protected:

    QStringList getKeywords() const;

    const QVariant getName() const;

    QMap<QString, QVariant> getAbstract() const;

    QVariantMap getDescription() const;

    QVariant getIcon();

    QVariant getLicense();

    QVariantList getCategories() const;

    QVariantMap getDeveloper();

    QVariantMap getRelease();

    QVariantMap getFile();

    QVariantList getLanguages();

    QVariantList getScreenShots();

    QVariantMap getLinks();

    QVariantList getMimeTypes();

    QVariant removeEmptyFields(QVariant variant);
};

#endif //APPIMAGE_RELEASES_INDEXER_METADATAMERGER_H
