#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include <QObject>
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
};

struct ScanResult {
    vector<ScanEntry> entries;
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

signals:
    void guiStateChanged();
    void rootDirectoryChanged();
    void scanResultChanged();

private:
    explicit ApplicationState(QObject *parent = nullptr) : m_guiState(CHOOSE_DIRECTORY) {
    }

    GuiState m_guiState;
    QString m_rootDirectory;
    ScanResult m_scanResult;
};

#endif // APPLICATIONSTATE_H
