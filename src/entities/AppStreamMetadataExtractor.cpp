//
// Created by alexis on 6/6/18.
//

#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>
#include "AppStreamMetadataExtractor.h"
AppStreamMetadataExtractor::AppStreamMetadataExtractor(const QString& filePath)
        :filePath(filePath)
{

}
QVariantMap AppStreamMetadataExtractor::getContent()
{
    data = QVariantMap();
    QByteArray rawData = readFile();
    xml.addData(rawData);

    QSet<QString> simpleEntries{"id", "name", "summary", "metadata_license", "project_group", "project_license",
                                "developer_name", "update_contact"};
    while (!xml.atEnd()) {
        token = xml.readNext();
        tokenName = xml.name().toString();
        if (token==QXmlStreamReader::StartDocument)
            continue;

        if (token==QXmlStreamReader::StartElement) {
            if (simpleEntries.contains(tokenName))
                data[tokenName] = xml.readElementText();

            if ("screenshots"==tokenName)
                data[tokenName] = readScreenshots();

            if ("description"==tokenName)
                data[tokenName] = readHtml();

            if ("url"==tokenName)
                readUrl();

            if ("releases"==tokenName)
                data["releases"] = readReleases();
        }
    }
    if (xml.hasError())
        qWarning() << "Error found in AppStream file: " << xml.errorString();

    xml.clear();
    return data;
}
QVariantList AppStreamMetadataExtractor::readReleases()
{
    QVariantList releases;
    while (!xml.atEnd() && !(token==QXmlStreamReader::EndElement && tokenName=="releases")) {
        if (token==QXmlStreamReader::StartElement && tokenName=="release")
            releases << readRelease();

        token = xml.readNext();
        tokenName = xml.name().toString();
    }
    return releases;
}
QVariantMap AppStreamMetadataExtractor::readRelease()
{
    QVariantMap release;
    release["version"] = xml.attributes().value("version").toString();
    release["date"] = xml.attributes().value("date").toString();
    release["urgency"] = xml.attributes().value("urgency").toString();
    release["timestamp"] = xml.attributes().value("timestamp").toString();

    while (!xml.atEnd() && !(token==QXmlStreamReader::EndElement && tokenName=="release")) {
        if (token==QXmlStreamReader::StartElement) {
            if (tokenName=="location")
                release["location"] = xml.readElementText();

            if (tokenName=="checksum") {
                auto checksumType = xml.attributes().value("type").toString();
                release[checksumType] = xml.readElementText();
            }

            if (tokenName=="description")
                release["description"] = readHtml();
        }
        token = xml.readNext();
        tokenName = xml.name().toString();
    }
    return release;
}
void AppStreamMetadataExtractor::readUrl()
{
    auto urls = data.value("urls", QVariantMap()).toMap();
    auto type = xml.attributes().value("type").toString();
    urls[type] = xml.readElementText();
    data["urls"] = urls;
}
QVariantList AppStreamMetadataExtractor::readScreenshots()
{
    QVariantList screenShots;
    while (!xml.atEnd() && !(token==QXmlStreamReader::EndElement && tokenName=="screenshots")) {
        if (token==QXmlStreamReader::StartElement && tokenName=="screenshot")
            screenShots << readScreenShot();

        token = xml.readNext();
        tokenName = xml.name().toString();
    }

    return screenShots;
}
QVariantMap AppStreamMetadataExtractor::readScreenShot()
{
    QVariantMap screenshot;
    while (!xml.atEnd() && !(token==QXmlStreamReader::EndElement && tokenName=="screenshot")) {
        if (token==QXmlStreamReader::StartElement) {
            if (tokenName=="caption")
                screenshot["caption"] = xml.readElementText();

            if (tokenName=="image") {
                screenshot["url"] = xml.readElementText();
                screenshot["type"] = xml.attributes().value("type", "source").toString();
                screenshot["width"] = xml.attributes().value("width").toInt();
                screenshot["height"] = xml.attributes().value("height").toInt();
                screenshot["language"] = xml.attributes().value("xml:lang").toInt();
            }
        }
        token = xml.readNext();
        tokenName = xml.name().toString();
    }
    return screenshot;
}
QByteArray AppStreamMetadataExtractor::readFile() const
{
    QByteArray data;
    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly))
        data = f.readAll();
    else
        throw std::runtime_error("Unable to open: "+filePath.toStdString());
    f.close();
    return data;
}
QString AppStreamMetadataExtractor::readHtml()
{
//    if (xml.namespaceUri().toString()!="http://www.w3.org/1999/xhtml") {
//        return xml.readElementText(QXmlStreamReader::IncludeChildElements);
//    }
    QString terminatingElement = xml.name().toString();
    QString html;
    QXmlStreamWriter writer(&html);
    do {
        xml.readNext();
        switch (xml.tokenType()) {
        case QXmlStreamReader::StartElement:writer.writeStartElement(xml.name().toString());
            writer.writeAttributes(xml.attributes());
            break;
        case QXmlStreamReader::EndElement:writer.writeEndElement();
            break;
        case QXmlStreamReader::Characters:writer.writeCharacters(xml.text().toString());
            break;
        case QXmlStreamReader::NoToken:break;
        case QXmlStreamReader::Invalid:break;
        case QXmlStreamReader::StartDocument:break;
        case QXmlStreamReader::EndDocument:break;
        case QXmlStreamReader::Comment:break;
        case QXmlStreamReader::DTD:break;
        case QXmlStreamReader::EntityReference:break;
        case QXmlStreamReader::ProcessingInstruction:break;
        }
    }
    while (!xml.atEnd() && xml.name()!=terminatingElement);
    return html;
}
