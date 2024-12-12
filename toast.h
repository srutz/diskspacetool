#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QVBoxLayout>
#include <QApplication>
#include <QMainWindow>

#include "util.h"

class Toast : public QWidget {
    Q_OBJECT
public:
    explicit Toast(const QString &message, int duration = 3000, QWidget *parent = nullptr)
            : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_ShowWithoutActivating);
        auto *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel(message, this);
        label->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.7); padding: 10px; border-radius: 5px;");
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label, Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
        QTimer::singleShot(duration, this, &Toast::deleteLater);
    }

    void showAtTopCenterOf(QWidget *parentWindow) {
        if (!parentWindow) return;
        QRect parentGeometry = parentWindow->geometry();
        int x = parentGeometry.x() + (parentGeometry.width() - this->width()) / 2;
        int y = parentGeometry.y() + parentGeometry.height() - this->height() - 8;
        move(x, y);

        show();
    }

    static void showToast(QWidget *widget, const QString &message) {
        // Show a toast notification at the top center of the main window
        auto mainWindow = Util::getMainWindow(widget);
        if (!mainWindow) {
            qDebug() << "no window avail";
        }
        Toast *toast = new Toast(message);
        toast->resize(300, 50);
        toast->showAtTopCenterOf(mainWindow);
    }
};


#endif // TOAST_H
