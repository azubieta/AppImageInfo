#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include <gateways/PageDownloader.h>
#include <gateways/FileDownloader.h>
class TestDownloader : public QObject {
Q_OBJECT
private slots:
    void downloadPage()
    {
        PageDownloader d("https://github.com/ZencashOfficial/arizen/");
        d.download();
        QSignalSpy spy(&d, &PageDownloader::finished);
        spy.wait();

        auto data = d.getData();
        Q_ASSERT(!data.isEmpty());
    }

    void downloadWrongPage()
    {
        PageDownloader d("https://nowere.com/provably/is/not/going/to/work");
        d.download();
        QSignalSpy spy(&d, &PageDownloader::finished);
        spy.wait();

        auto data = d.getData();
        Q_ASSERT(data.isEmpty());
        Q_ASSERT(d.isErrored());
    }

    void downloadFile()
    {
        QString tmp_file_path = "/tmp/test.download.file.html";
        QString url = "https://github.com/ZencashOfficial/arizen/";
        FileDownloader d(url, tmp_file_path);
        QSignalSpy spy(&d, &FileDownloader::finished);

        d.start();
        spy.wait();

        QFile f(tmp_file_path);
        Q_ASSERT(f.exists());
        Q_ASSERT(f.size()>0);
        f.remove();
    }
    void downloadWrongFile()
    {
        QString tmp_file_path = "/tmp/test.download.file.html";
        QString url = "https://nowere.com/provably/is/not/going/to/work";
        FileDownloader d(url, tmp_file_path);
        QSignalSpy spy(&d, &FileDownloader::finished);
        d.start();
        spy.wait();

        QFile f(tmp_file_path);
        Q_ASSERT(!f.exists());
        Q_ASSERT(d.isFailed());
        f.remove();
    }
};

QTEST_MAIN(TestDownloader)
#include "testDownloader.moc"
