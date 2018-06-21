#include <QGuiApplication>
#include <QJsonDocument>
#include <QTextStream>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QPixmap>

#include "../entities/FileMetadataExtractor.h"


struct {
    QString appImagePath;
    QString outputDirPath;
} typedef AppConfig;

void writeAppImageInfo(const QVariantMap &metadata, const QString &outputDirPath);

void writeAppImageIcon(const QPixmap &icon, const QString &outputDirPath);

AppConfig parseArguments(const QGuiApplication &app);


int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(PROJECT_NAME);
    QGuiApplication::setApplicationVersion(PROJECT_VERSION);

    AppConfig config = parseArguments(app);

    FileMetadataExtractor fileMetadataExtractor;
    fileMetadataExtractor.setPath(config.appImagePath);

    auto metadata = fileMetadataExtractor.extractMetadata();
    auto icon = fileMetadataExtractor.extractIcon();

    writeAppImageInfo(metadata, config.outputDirPath);
    writeAppImageIcon(icon, config.outputDirPath);

    return 0;
}


AppConfig parseArguments(const QGuiApplication &app) {
    QCommandLineParser parser;
    parser.setApplicationDescription(
            "Utility to extract metadata and icon from AppImage files."
            "\n\nOutputs:"
            "\n\t<directory>/AppImageInfo.json\t Mix of \".Desktop\" and \".appdata.xml\" files."
            "\n\t<directory>/AppImageIcon.png\t Application icon.");

    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("path", QCoreApplication::translate("main", "AppImage file <path>."));
    QCommandLineOption targetDirOption({{"t", "output-directory"},
                                        QCoreApplication::translate("main", "Write files into <directory>."),
                                        QCoreApplication::translate("main", "directory")});
    parser.addOption(targetDirOption);

    parser.process(app);

    AppConfig config;
    const QStringList args = parser.positionalArguments();
    if (args.length() == 1)
        config.appImagePath = args.at(0);
    else
        parser.showHelp(1);

    if (!QFile::exists(config.appImagePath)) {
        qCritical() << "Invalid path: " << config.appImagePath;
        parser.showHelp(1);
    }


    config.outputDirPath = parser.value(targetDirOption);
    if (!QFile::exists(config.outputDirPath)) {
        qCritical() << "Invalid path: " << config.appImagePath;
        parser.showHelp(1);
    }

    return config;
}

void writeAppImageInfo(const QVariantMap &metadata, const QString &outputDirPath) {
    auto doc = QJsonDocument::fromVariant(metadata);
    QFile output(outputDirPath + "/AppImageInfo.json");
    if (output.open(QIODevice::WriteOnly)) {
        output.write(doc.toJson());
        output.close();
    }
}

void writeAppImageIcon(const QPixmap &icon, const QString &outputDirPath) {
    auto scaledIcon = icon.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledIcon.save(outputDirPath + "/AppImageIcon.png", "PNG", 100);
}