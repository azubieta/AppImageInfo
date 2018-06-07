#include <QObject>
#include <QTest>
#include <QDebug>

#include <entities/MetadataExtractor.h>
#include <entities/DesktopFileMetadataExtractor.h>
#include <entities/AppStreamMetadataExtractor.h>

class TestMetadataExtractor : public QObject {
Q_OBJECT
    MetadataExtractor* m;
    QStringList fileList;
private slots:
    void initTestCase()
    {
        m = new MetadataExtractor(TEST_DATA_DIR"/appimagetool-x86_64.AppImage");
    }
    void listFilesTest()
    {
        try {
            fileList = m->loadFileList();
            qInfo() << fileList;
        }
        catch (std::runtime_error& e) {
            QFAIL(e.what());
        }

    }
    void parseDesktopFileTest()
    {
        try {
            DesktopFileMetadataExtractor e(TEST_DATA_DIR"/appimagetool.desktop");
            qInfo() << e.getContent();
        }
        catch (std::runtime_error& e) {
            QFAIL(e.what());
        }
    }

    void parseAppStreamFileTest()
    {
        try {
            AppStreamMetadataExtractor e(TEST_DATA_DIR"/appimagetool.appdata.xml");
            qInfo() << e.getContent();
        }
        catch (std::runtime_error& e) {
            QFAIL(e.what());
        }
    }
    void cleanupTestCase() { delete (m); }
};

QTEST_MAIN(TestMetadataExtractor)
#include "testMetadataExtractor.moc"