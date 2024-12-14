#include "scanner.h"
#include "applicationstate.h"
#include <QDir>
#include <QDebug>
#include "guard.h"

struct CancelException {};

void dumpScanEntry(ScanEntry *entry, int depth = 0) {
    if (!entry) {
        return;
    }
    QString s;
    for (int i = 0; i < depth; i++) {
        s += "  ";
    }
    s += QString("this=") + QString::asprintf("%p", entry);
    s += QString(", parent=") + QString::asprintf("%p", entry->parent);
    s += "  " + entry->path;
    qDebug() << s;
    for (auto &child : entry->children) {
        dumpScanEntry(child.get(), depth + 1);
    }
}


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
        auto root = processDirectory(nullptr, state->rootDirectory());
        //dumpScanEntry(root.get());
        result.root = root;
        state->setScanResult(result);
        emit completed();
    } catch (CancelException signal) {
        emit cancelled();
    }
}

shared_ptr<ScanEntry> Scanner::processDirectory(ScanEntry* parent, const QString &directoryPath) {
    auto state = ApplicationState::instance();
    if (state->cancelRequest()) {
        throw CancelException();
    }
    QDir dir(directoryPath);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << directoryPath;
        return nullptr;
    }

    auto files = dir.entryList(QDir::Files);
    qint64 dirSize = 0;
    auto count = 0;
    for (const QString &fileName : files) {
        QString fullFileName = dir.filePath(fileName);
        QFile f(fullFileName);
        dirSize += f.size();
        count++;
    }
    auto entry = make_shared<ScanEntry>(ScanEntry{
        .path = directoryPath,
        .size = dirSize,
        .localSize = dirSize,
        .fileCount = count,
        .localFileCount = count,
        .parent = parent,
    });
    if (parent) {
        parent->children.push_back(entry);
    }
    emit progress(directoryPath, 1);

    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    qint64 totalSize = entry->localSize;
    auto totalFileCount = entry->localFileCount;

    for (const QString &subDir : subDirs) {
        QString subDirPath = dir.absoluteFilePath(subDir);
        auto child = processDirectory(entry.get(), subDirPath);
        totalSize += child->size;
        totalFileCount += child->fileCount;
    }
    entry->size = totalSize;
    entry->fileCount = totalFileCount;
    return entry;
}
