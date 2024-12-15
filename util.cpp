#include "util.h"
#include <QUrl>
#include <QFileInfo>
#include <QDesktopServices>


QMainWindow* Util::getMainWindow(QWidget *widget) {
    return qobject_cast<QMainWindow *>(widget->window());
}

void Util::openLocalFile(const QString &path) {
    auto  url = QUrl::fromLocalFile(path);
    if (!QDesktopServices::openUrl(url)) {
        qWarning() << "Failed to open directory:" << path;
    }
}

QString Util::basename(const QString& path) {
    QFileInfo fileInfo(path);
    QString lastComponent = fileInfo.fileName();
    return lastComponent;
}
