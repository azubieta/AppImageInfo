//
// Created by alexis on 6/8/18.
//
#include <QDebug>
#include <QProcess>
#include "BinaryMetadataExtractor.h"

BinaryMetadataExtractor::BinaryMetadataExtractor(const QString& target)
        :target(target) { }

QVariantMap BinaryMetadataExtractor::getMetadata()
{
    QVariantMap data;
    data["architecture"] = getBinaryArch();
    data["sha512checksum"] = getSha512CheckSum();
    return data;
}
QString BinaryMetadataExtractor::getSha512CheckSum() const
{
    auto process = new QProcess();
    QStringList arguments{target};
    process->start("sha512sum", arguments);

    process->waitForFinished();
    if (process->exitCode()!=0)
        throw std::runtime_error(process->errorString().toStdString());
    QString rawOutput = process->readAll();
    QString sha512checksum = rawOutput.section(" ", 0, 0).trimmed();

    process->deleteLater();
    return sha512checksum;
}
QString BinaryMetadataExtractor::getBinaryArch() const
{
    auto process = new QProcess();
    QStringList arguments;
    arguments << "-b" << "-e" << "ascii" << "-e" << "apptype" << "-e" << "encoding" <<
              "-e" << "elf" << "-e" << "tokens" << "-e" << "tar" << "-e" << "cdf" <<
              "-e" << "compress" << target;

    process->start("file", arguments);
    process->waitForFinished();
    if (process->exitCode()!=0)
        throw std::runtime_error(process->errorString().toStdString());
    QString rawOutput = process->readAll();
    QString arch = rawOutput.section(",", 1, 1).trimmed();

    process->deleteLater();
    return arch;
}

