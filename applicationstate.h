#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

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
    int fileCount;
    ScanEntry* parent = nullptr;
    vector<shared_ptr<ScanEntry>> children;
};

struct ScanResult {
    vector<shared_ptr<ScanEntry>> entries;
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
