#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include <QDebug>
#include <QObject>
#include <vector>
#include <memory>
#include <vector>

using namespace std;

enum GuiState {
    CHOOSE_DIRECTORY = 1,
    SCANNING = 2,
    RESULTS = 3,
};

struct ScanEntry {
    QString path;
    qint64 size;
    qint64 localSize;
    double percent;

    int fileCount;
    int localFileCount;
    ScanEntry* parent = nullptr;
    vector<shared_ptr<ScanEntry>> children;

    ~ScanEntry() {
        //qDebug() << QString::asprintf("~ScanEntry %p", this);
    }

    /* run through the entries pre-order */
    template<typename V>
    static void traverse(ScanEntry *entry, V visitor, int depth = 0) {
        if (!entry) {
            return;
        }
        visitor(entry, depth);
        for (auto &child : entry->children) {
            traverse(child.get(), visitor, depth + 1);
        }
    }

    /* run through the entries post-order */
    template<typename V>
    static void traversePostOrder(ScanEntry *entry, V visitor, int depth = 0) {
        if (!entry) {
            return;
        }
        for (auto &child : entry->children) {
            traversePostOrder(child.get(), visitor, depth + 1);
        }
        visitor(entry, depth);
    }

};

struct ScanResult {
    shared_ptr<ScanEntry> root;
};

class ApplicationState : public QObject
{
    Q_OBJECT
public:

    static ApplicationState* instance() {
        static ApplicationState instance;
        return &instance;
    }

    GuiState guiState() const { return m_guiState; }
    void setGuiState(GuiState guiState);

    const QString& rootDirectory() const { return m_rootDirectory; }
    void setRootDirectory(const QString &rootDirectory);

    ScanResult scanResult() const { return m_scanResult; }
    void setScanResult(const ScanResult &result);

    bool cancelRequest() const { return m_cancelRequest; }
    void setCancelRequest(bool chancelRequest);

signals:
    void guiStateChanged();
    void rootDirectoryChanged();
    void scanResultChanged();
    void cancelRequestChanged();

private:
    explicit ApplicationState(QObject *parent = nullptr) : m_guiState(CHOOSE_DIRECTORY) {
    }

    GuiState m_guiState;
    QString m_rootDirectory;
    ScanResult m_scanResult;
    bool m_cancelRequest;
};

#endif // APPLICATIONSTATE_H
