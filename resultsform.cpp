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
    treeView->setItemDelegateForColumn(1, new AlignDelegate(1, Qt::AlignRight | Qt::AlignVCenter, treeView));
    treeView->setItemDelegateForColumn(2, new AlignDelegate(2, Qt::AlignRight | Qt::AlignVCenter, treeView));
    connect(treeView, &QTreeView::customContextMenuRequested, this, &ResultsForm::showContextMenu);

    auto tabs = new QTabWidget(this);
    tabs->setTabBarAutoHide(true);
    tabs->addTab(treeView, "TreeView");

    auto contentLayout = new QVBoxLayout(content);
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

    QAction *copyAction = contextMenu.addAction("Copy path to clipboard");
    if (index.isValid()) {
        auto item = treeView->model()->data(index);
        entry = static_cast<ScanEntry*>(index.internalPointer());
    } else {
        copyAction->setEnabled(false);
    }
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
        Toast::showToast(this, "Path copied to clipboard", 500);
    }
}
