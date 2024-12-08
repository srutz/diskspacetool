#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include <QDebug>
#include <QStyle>
#include <QPushButton>
#include <QStackedWidget>
#include "applicationstate.h"
#include "choosedirectoryform.h"
#include "scanform.h"
#include "resultsform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    auto chooseDirectoryForm = new ChooseDirectoryForm(this);
    auto scanForm = new ScanForm(this);
    auto resultsForm = new ResultsForm(this);

    auto layout = this->ui->content->layout();
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->addWidget(stackedWidget);

    stackedWidget->addWidget(chooseDirectoryForm);
    stackedWidget->addWidget(scanForm);
    stackedWidget->addWidget(resultsForm);
    this->applyGuiState();

    // wire listeners to state
    auto state = ApplicationState::instance();
    connect(state, &ApplicationState::guiStateChanged, this, [=] {
        applyGuiState();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout()
{
    AboutDialog d(this);
    d.exec();
}

void MainWindow::applyGuiState() {
    auto state = ApplicationState::instance();
    // take guiState and adjust the gui accordingly
    switch (state->guiState()) {
    case CHOOSE_DIRECTORY:
        stackedWidget->setCurrentIndex(0); break;
    case SCANNING:
        stackedWidget->setCurrentIndex(1); break;
    case RESULTS:
        stackedWidget->setCurrentIndex(2); break;
    }
}

