#include "choosedirectoryform.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "applicationstate.h"

ChooseDirectoryForm::ChooseDirectoryForm(QWidget *parent)
    : QWidget(parent)
{
    auto content= new QWidget(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->addWidget(content);

    auto startScanButton = new QPushButton("Choose directory to analyze", this);
    startScanButton->setStyleSheet("padding: 16px");
    QIcon icon(QIcon::fromTheme("folder-open"));
    startScanButton->setIcon(icon);
    startScanButton->setIconSize(QSize(32, 32));

    auto contentLayout = new QVBoxLayout(content);
    contentLayout->addWidget(startScanButton, 0, Qt::AlignmentFlag::AlignCenter);


    //layout->setAlignment(startScanButton , Qt::AlignHCenter);
    //this->setStyleSheet("background-color: orange;");

    connect(startScanButton, &QPushButton::clicked, this, [=] {
        QString directory = QFileDialog::getExistingDirectory(
            this,
            "Select Directory",
            QDir::homePath(),
            QFileDialog::ShowDirsOnly
        );
        if (directory.isEmpty()) {
            return;
        }
        //QMessageBox::information(
        //    nullptr,
        //    "Selected Directory",
        //    "You selected: " + directory
        //);
        auto state = ApplicationState::instance();
        state->setRootDirectory(directory);
        state->setGuiState(SCANNING);
    });
}
