#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QTextStream>
#include <QtCore/QCommandLineParser>
#include "entities/GitHubProjectIndexer.h"

QString parseArguments(const QCoreApplication &app);

void printResult(const QVariantMap &metadata);

class IndexerApplication : public QObject {
Q_OBJECT
    QString url;
    GitHubProjectIndexer *ghIndexer;
public:
    IndexerApplication(const QString &url, QObject *parent = nullptr) : QObject(parent), url(url) {
        ghIndexer = new GitHubProjectIndexer(url);
        connect(ghIndexer, &GitHubProjectIndexer::completed, this, &IndexerApplication::handleIndexerCompleted);
    }

    void execute() {
        ghIndexer->run();
    }

public slots:

    void handleIndexerCompleted() {
        auto doc = QJsonDocument::fromVariant(ghIndexer->getAppInfo());
        QTextStream textStream(stdout);
        textStream << doc.toJson();

        QCoreApplication::exit(0);
    };
};

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    auto url = parseArguments(app);

    auto indexerApp = new IndexerApplication(url);
    indexerApp->execute();

    return app.exec();
}


QString parseArguments(const QCoreApplication &app) {
    QCommandLineParser parser;
    parser.setApplicationDescription("Utility to metadata from AppImage files published as GitHub repo releases.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", QCoreApplication::translate("main", "Url to the repo."));

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty() || args.length() > 1)
        parser.showHelp(1);
    else
        return args.at(0);
}

#include "indexer_main.moc"