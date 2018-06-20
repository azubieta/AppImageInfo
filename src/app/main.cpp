#include <QCoreApplication>
#include <QJsonDocument>
#include <QTextStream>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>

#include "../entities/FileMetadataExtractor.h"


QString parseArguments(const QCoreApplication &app);

void printResult(const QVariantMap &metadata);

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);


    QString path = parseArguments(app);
    FileMetadataExtractor fileMetadataExtractor;
    fileMetadataExtractor.setPath(path);
    auto metadata = fileMetadataExtractor.extractMetadata();

    printResult(metadata);

    return 0;
}


QString parseArguments(const QCoreApplication &app) {
    QCommandLineParser parser;
    parser.setApplicationDescription(
            "Utility to extract metadata from AppImage files.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("path", QCoreApplication::translate("main", "AppImage file <path>."));

    parser.process(app);

    QString path;
    const QStringList args = parser.positionalArguments();
    if (args.isEmpty() || args.length() > 1)
        parser.showHelp(1);
    else
        path = args.at(0);

    if (!QFile::exists(path)) {
        qCritical() << "Invalid path: " << path;
        parser.showHelp(1);
    }

    return path;
}

void printResult(const QVariantMap &metadata) {
    auto doc = QJsonDocument::fromVariant(metadata);
    QTextStream textStream(stdout);
    textStream << doc.toJson();
}