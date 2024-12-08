#include "scanform.h"
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QThread>
#include "scanner.h"
#include "applicationstate.h"

ScanForm::ScanForm(QWidget *parent)
    : QWidget{parent}
{
    auto content= new QWidget(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->addWidget(content);

    auto progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);

    auto label = new QLabel("Scanning ...", this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);

    auto contentLayout = new QVBoxLayout(content);
    contentLayout->addWidget(label);
    contentLayout->addWidget(progressBar);

    auto state = ApplicationState::instance();
    connect(state, &ApplicationState::guiStateChanged, this, [=] {
        if (state->guiState() == SCANNING) {
            auto thread = new QThread();
            thread->setObjectName("Scanner-Thread");
            auto scanner= new Scanner();
            scanner->moveToThread(thread);
            connect(scanner, &Scanner::progress, this, [=] (const QString &currentPath, int counter) {
                label->setText(QString("Scanning ") + currentPath);
            });
            connect(scanner, &Scanner::completed, this, [=] {
                auto state = ApplicationState::instance();
                state->setGuiState(RESULTS);
            });
            connect(thread, &QThread::started, scanner, &Scanner::startWork);
            thread->start();
        }
    });

}
