//
// Created by alexis on 6/18/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_APPINFOASSEMBLER_H
#define APPIMAGE_RELEASES_INDEXER_APPINFOASSEMBLER_H


#include <QObject>
#include <QVariantMap>
#include "../gateways/PageDownloader.h"

class AppInfoBuilder {

    QVariantMap appInfo;
    QVariantMap appName;
    QVariantMap appAbstract;
    QVariantMap appDescription;
    QVariantMap appDeveloper;
    QVariantMap appLinks;
    QStringList appCategories;
    QVariantList appReleases;

public:
    void fillMissingAppInfoFields(const QVariantMap &fileInfo);

    QVariantMap build() const;

    void setAppInfo(const QVariantMap &appInfo);

    const QVariantMap &getAppName() const;

    void setAppName(const QVariantMap &appName);

    const QVariantMap &getAppAbstract() const;

    void setAppAbstract(const QVariantMap &appAbstract);

    const QVariantMap &getAppDescription() const;

    void setAppDescription(const QVariantMap &appDescription);

    const QVariantMap &getAppDeveloper() const;

    void setAppDeveloper(const QVariantMap &appDeveloper);

    const QVariantMap &getAppLinks() const;

    void setAppLinks(const QVariantMap &appLinks);

    const QStringList &getAppCategories() const;

    void setAppCategories(const QStringList &appCategories);

    const QVariantList &getAppReleases() const;

    void setAppReleases(const QVariantList &appReleases);

    void addRelease(const QVariantMap &release);

    void setFormat(int format);

    void setKeyWords(const QStringList keywords);

    void setLicense(QString license);

    void addDescription(const QString &locale, const QString &description);

    void addLink(const QString &type, const QString &url);
};


#endif //APPIMAGE_RELEASES_INDEXER_APPINFOASSEMBLER_H
