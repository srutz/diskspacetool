#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include "applicationstate.h"

using namespace std;



class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(QObject *parent = nullptr);

public slots:
    void startWork();

signals:
    void completed();
    void cancelled();
    void progress(const QString &currentPath, int counter);

private:
    void processDirectory(ScanResult &result, ScanEntry* parent, const QString &directoryPath);
};

#endif // SCANNER_H
