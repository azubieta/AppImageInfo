#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include <gateways/PageDownloader.h>
#include <entities/GitHubProjectIndexer.h>
#include <QtCore/QJsonDocument>

class TestIndexer : public QObject {
Q_OBJECT
    GitHubProjectIndexer* gitHubProjectIndexer;
private slots:
    void initTestCase()
    {
        gitHubProjectIndexer = new GitHubProjectIndexer("https://github.com/AppImage/AppImageKit");
    };
    void testGitHubProjectMatch()
    {
        auto res = GitHubProjectIndexer::isGitHubProject(
                "https://github.com/--ZencashOfficiasd332324234--.sd0kdf.l/--sd.arizen/");
        Q_ASSERT(res);

        GitHubProjectIndexer indexer("https://github.com/--ZencashOfficiasd332324234--.sd0kdf.l/--sd.arizen/");
        auto path = indexer.getPath();
        QCOMPARE(path, QString("--ZencashOfficiasd332324234--.sd0kdf.l/--sd.arizen"));
    }

    void testGetGitHubProjectInfo()
    {
        QSignalSpy spy(gitHubProjectIndexer, &GitHubProjectIndexer::completed);

        gitHubProjectIndexer->run();
        spy.wait(60000);

        QCOMPARE(spy.count(), 1);
        auto doc = QJsonDocument::fromVariant(gitHubProjectIndexer->getAppInfo());
        qInfo() << doc.toJson();
    }
    void cleanupTestCase()
    {
        gitHubProjectIndexer->deleteLater();
    }
};

QTEST_MAIN(TestIndexer)
#include "testIndexer.moc"
