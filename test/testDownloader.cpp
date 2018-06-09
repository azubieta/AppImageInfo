#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include <gateways/PageDownloader.h>
class TestDownloader : public QObject {
Q_OBJECT
private slots:
    void downloadPage()
    {
        PageDownloader d("https://github.com/ZencashOfficial/arizen/");
        d.download();
        QSignalSpy spy(&d, &PageDownloader::downloaded);
        spy.wait();

        auto data = d.getData();
        Q_ASSERT(!data.isEmpty());
    }

    void downloadWrongPage()
    {
        PageDownloader d("https://nowere.com/provably/is/not/going/to/work");
        d.download();
        QSignalSpy spy(&d, &PageDownloader::downloaded);
        spy.wait();

        auto data = d.getData();
        Q_ASSERT(data.isEmpty());
        Q_ASSERT(d.isErrored());
    }
};

QTEST_MAIN(TestDownloader)
#include "testDownloader.moc"
