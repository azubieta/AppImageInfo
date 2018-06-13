#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include <gateways/PageDownloader.h>
#include <entities/GitHubProjectIndexer.h>

class TestIndexer : public QObject {
Q_OBJECT
    GitHubProjectIndexer* gitHubProjectIndexer;
private slots:
    void initTestCase()
    {
        gitHubProjectIndexer = new GitHubProjectIndexer("https://github.com/z-------------/cumulonimbus");
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
        spy.wait();

        QCOMPARE(spy.count(), 1);
    }
    void cleanupTestCase()
    {
        gitHubProjectIndexer->deleteLater();
    }
};

QTEST_MAIN(TestIndexer)
#include "testIndexer.moc"
