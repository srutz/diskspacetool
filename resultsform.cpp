#include "resultsform.h"
#include "applicationstate.h"
#include <QVBoxLayout>
#include <QString>
#include <QTextStream>
#include <QTabWidget>
#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QClipboard>

#include "scanentrymodel.h"
#include "aligndelegate.h"
#include "toast.h"
#include "util.h"

ResultsForm::ResultsForm(QWidget *parent)
    : QWidget{parent}, m_root(nullptr)
{
    auto content= new QWidget(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->addWidget(content);

    auto treeView = new QTreeView(this);
    treeView->setObjectName("treeview");
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    auto rightAlign = [=](int column) {
        treeView->setItemDelegateForColumn(column, new AlignDelegate(column, Qt::AlignRight | Qt::AlignVCenter, treeView));
    };

    rightAlign(1);
    rightAlign(2);
    rightAlign(3);
    connect(treeView, &QTreeView::customContextMenuRequested, this, &ResultsForm::showContextMenu);

    auto titleLabel = new QLabel("", this);
    auto tabs = new QTabWidget(this);
    tabs->setTabBarAutoHide(true);
    tabs->addTab(treeView, "TreeView");

    auto contentLayout = new QVBoxLayout(content);
    contentLayout->addWidget(titleLabel);
    contentLayout->addWidget(tabs);

    auto state = ApplicationState::instance();
    connect(state, &ApplicationState::guiStateChanged, this, [=] {
        if (state->guiState() == RESULTS) {
            QString buffer;
            QTextStream stream(&buffer);
            auto result = state->scanResult();

            auto oldModel = treeView->model();
            if (oldModel) {
                auto oldScanEntryModel = static_cast<ScanEntryModel*>(oldModel);
                delete oldScanEntryModel;
            }
            m_root = result.root;
            auto model = new ScanEntryModel(m_root.get());
            treeView->setModel(model);
            treeView->setColumnWidth(0, 400);

            auto title = QString("Scanresults for %1").arg(m_root->path);
            titleLabel->setText(title);

        }
    });
}

ResultsForm::~ResultsForm() {
}


void ResultsForm::showContextMenu(const QPoint &pos) {
    auto treeView = this->findChild<QTreeView*>("treeview");
    auto index = treeView->indexAt(pos);
    QMenu contextMenu;
    ScanEntry *entry = nullptr;

    QAction *openAction = contextMenu.addAction("Open path");
    QAction *copyAction = contextMenu.addAction("Copy path to clipboard");
    if (index.isValid()) {
        entry = static_cast<ScanEntry*>(index.internalPointer());
    } else {
        openAction->setEnabled(false);
        copyAction->setEnabled(false);
    }
    contextMenu.addSeparator();
    QPoint globalPos = treeView->mapToGlobal(pos);
    QAction *expandAction = contextMenu.addAction("Expand all");
    QAction *collapseAction = contextMenu.addAction("Collapse all");
    QAction *selectedAction = contextMenu.exec(globalPos);
    if (selectedAction == expandAction) {
        treeView->expandAll();
    } else if (selectedAction == collapseAction) {
        treeView->collapseAll();
    } else if (selectedAction == copyAction) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(entry->path);
        Toast::showToast(this, "Path copied to clipboard", 750);
    } else if (selectedAction == openAction) {
        Util::openLocalFile(entry->path);
    }
}
