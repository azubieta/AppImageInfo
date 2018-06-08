#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QJsonDocument>

#include "entities/MetadataExtractor.h"

void parseArguments(const QCoreApplication& app, MetadataExtractor& extractor);
void printResult(const QVariantMap& metadata);
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    MetadataExtractor extractor;
    parseArguments(app, extractor);

    extractor.loadFileList();
    QVariantMap metadata = extractor.extractMetadata();
    printResult(metadata);

    return 0;
}
void printResult(const QVariantMap& metadata)
{
    auto doc = QJsonDocument::fromVariant(metadata);
    QTextStream textStream(stdout);
    textStream << doc.toJson();
}

void parseArguments(const QCoreApplication& app, MetadataExtractor& extractor)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Utility to extract metadata from AppImage files.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "AppImage file to be inspected."));

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty() || args.length() > 1)
        parser.showHelp(1);
    else
        extractor.setPath(args.at(0));
}
