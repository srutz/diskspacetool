#include "scanform.h"
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QThread>
#include <QPushButton>
#include "scanner.h"
#include "applicationstate.h"
#include "toast.h"

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
    label->setFixedWidth(this->width());
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(false);

    auto state = ApplicationState::instance();

    auto cancelButton = new QPushButton("Cancel Scan", this);
    connect(cancelButton, &QPushButton::clicked, this, [=] {
        if (state->guiState() == SCANNING) {
            state->setCancelRequest(true);
        }
    });
    cancelButton->setFixedWidth(240);
    QWidget *buttonPanel = new QWidget(this);
    auto buttonPanelLayout = new QHBoxLayout(buttonPanel);
    buttonPanelLayout->addWidget(cancelButton);


    auto contentLayout = new QVBoxLayout(content);
    contentLayout->addStretch();
    contentLayout->addWidget(label);
    contentLayout->addSpacing(32);
    contentLayout->addWidget(progressBar);
    contentLayout->addStretch();
    contentLayout->addWidget(buttonPanel);

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
            connect(scanner, &Scanner::cancelled, this, [=] {
                auto state = ApplicationState::instance();
                state->setGuiState(CHOOSE_DIRECTORY);
                Toast::showToast(this, "Your scan was cancelled");
            });
            connect(thread, &QThread::started, scanner, &Scanner::startWork);
            thread->start();
        }
    });

}
