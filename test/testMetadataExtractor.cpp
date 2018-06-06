#include <QObject>
#include <QTest>
#include <QDebug>

#include <entities/MetadataExtractor.h>

class TestMetadataExtractor : public QObject {
Q_OBJECT
private slots:
    void initTestCase() { qDebug("called before everything else"); }
    void listFilesTest()
    {

        MetadataExtractor m(TEST_DATA_DIR"/azpainter_x86_64.AppImage");
        try {
            auto fileList = m.loadFileList();
            qWarning() << fileList;
        }
        catch (std::runtime_error& e) {
            QFAIL(e.what());
        }

    }
    void parseDesktopFileTest()
    {
        MetadataExtractor m(TEST_DATA_DIR"/azpainter_x86_64.AppImage");
        try {
            auto files = m.loadFileList();
            qWarning() << m.parseDesktopFile(files);

        } catch (std::runtime_error &e) {
            QFAIL(e.what());
        }
    }
    void cleanupTestCase() { qDebug("called after myFirstTest and mySecondTest"); }
};

QTEST_MAIN(TestMetadataExtractor)
#include "testMetadataExtractor.moc"