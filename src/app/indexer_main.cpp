#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QTextStream>
#include <QtCore/QCommandLineParser>

#include "Indexer.h"

QString parseArguments(const QCoreApplication &app);

void printResult(const QVariantMap &metadata);

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    auto url = parseArguments(app);

    auto indexer = new Indexer(url);
    indexer->execute();

    return app.exec();
}


QString parseArguments(const QCoreApplication &app) {
    QCommandLineParser parser;
    parser.setApplicationDescription(
            "Utility to extract metadata from AppImage files.");
    parser.addHelpOption();
    parser.addVersionOption();

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

    return url;
}