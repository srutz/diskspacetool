#include "scanner.h"
#include "applicationstate.h"
#include <QDir>
#include <QDebug>
#include "guard.h"

struct CancelException {};

Scanner::Scanner(QObject *parent)
    : QObject{parent}
{}

void Scanner::startWork() {
    auto state = ApplicationState::instance();
    Guard g([=] {
        state->setCancelRequest(false);
    });
    ScanResult result;
    try {
        processDirectory(result, nullptr, state->rootDirectory());
        state->setScanResult(result);
        emit completed();
    } catch (CancelException signal) {
        emit cancelled();
    }
}

void Scanner::processDirectory(ScanResult &result, ScanEntry* parent, const QString &directoryPath) {
    auto state = ApplicationState::instance();
    if (state->cancelRequest()) {
        throw CancelException();
    }
    qDebug() << "processing " << directoryPath;
    QDir dir(directoryPath);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << directoryPath;
        return;
    }

    auto files = dir.entryList(QDir::Files);
    auto entry = make_shared<ScanEntry>(ScanEntry{
        .path = directoryPath,
        //.size = dirSize,
        //.fileCount = count,
        .parent = parent,
    });
    qint64 dirSize = 0;
    int count = 0;
    for (const QString &fileName : files) {
        QString fullFileName = dir.filePath(fileName);
        QFile f(fullFileName);
        auto exits = f.exists();
        qDebug() << fileName;
        dirSize += f.size();
        count++;
    }
    entry->fileCount = count;
    entry->size = dirSize;
    if (parent) {
        parent->children.push_back(entry);
    }

    result.entries.emplace_back(entry);
    emit progress(directoryPath, result.entries.size());

    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (const QString &subDir : subDirs) {
        QString subDirPath = dir.absoluteFilePath(subDir);
        processDirectory(result, entry.get(), subDirPath);
    }

}
