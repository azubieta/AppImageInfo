#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QTextStream>
#include <QtCore/QCommandLineParser>

#include "Indexer.h"

void parseArguments(const QCoreApplication &app, Indexer *indexer);

void printResult(const QVariantMap &metadata);

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);


    auto indexer = new Indexer();
    parseArguments(app, indexer);

    indexer->execute();

    return app.exec();
}


void parseArguments(const QCoreApplication &app, Indexer *indexer) {
    QCommandLineParser parser;
    parser.setApplicationDescription(
            "Utility to extract metadata from AppImage files.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"c", "cacheDir"}, "Directory to store AppImages metadata cache.", "directory"});

    parser.addPositionalArgument("url",
                                 QCoreApplication::translate("main", "Appimage url (local or remote)."
                                                                     " Or GitHub project url."));

    parser.process(app);

    QString url;
    const QStringList args = parser.positionalArguments();
    if (args.isEmpty() || args.length() > 1)
        parser.showHelp(1);
    else
        url = args.at(0);

    indexer->setUrl(url);
    indexer->setCacheDir(parser.value("cacheDir"));
}