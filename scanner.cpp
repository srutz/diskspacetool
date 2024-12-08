#include "scanner.h"
#include "applicationstate.h"
#include <QDir>
#include <QDebug>

Scanner::Scanner(QObject *parent)
    : QObject{parent}
{}

void Scanner::startWork() {
    auto state = ApplicationState::instance();
    ScanResult result;
    processDirectory(result, state->rootDirectory());
    state->setScanResult(result);
    emit completed();
}

void Scanner::processDirectory(ScanResult &result, const QString &directoryPath) {
    qDebug() << "processing " << directoryPath;
    QDir dir(directoryPath);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << directoryPath;
        return;
    }

    auto files = dir.entryList(QDir::Files);
    qint64 dirSize = 0;
    int count;
    for (const QString &file : files) {
        QFile f(file);
        dirSize += f.size();
        count++;
    }
    ScanEntry entry = {
        .path = directoryPath,
        .size = dirSize,
        .fileCount = count,
    };
    result.entries.emplace_back(entry);
    emit progress(directoryPath, result.entries.size());

    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &subDir : subDirs) {
        QString subDirPath = dir.absoluteFilePath(subDir);
        //sqDebug() << "Entering directory:" << subDirPath;
        processDirectory(result, subDirPath);
    }
}
