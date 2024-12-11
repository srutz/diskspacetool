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
    processDirectory(result, nullptr, state->rootDirectory());
    state->setScanResult(result);
    emit completed();
}

void Scanner::processDirectory(ScanResult &result, ScanEntry* parent, const QString &directoryPath) {
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
    auto entry = make_shared<ScanEntry>(ScanEntry{
        .path = directoryPath,
        .size = dirSize,
        .fileCount = count,
        .parent = parent,
    });

    result.entries.emplace_back(entry);
    emit progress(directoryPath, result.entries.size());

    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (const QString &subDir : subDirs) {
        QString subDirPath = dir.absoluteFilePath(subDir);
        processDirectory(result, entry.get(), subDirPath);
    }
}
