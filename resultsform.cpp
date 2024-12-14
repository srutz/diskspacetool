#include "resultsform.h"
#include "applicationstate.h"
#include <QVBoxLayout>
#include <QString>
#include <QTextStream>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTreeView>
#include "scanentrymodel.h"

ResultsForm::ResultsForm(QWidget *parent)
    : QWidget{parent}, m_root(nullptr)
{
    auto content= new QWidget(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->addWidget(content);

    auto textArea = new QPlainTextEdit(this);
    textArea->setReadOnly(true);
    textArea->setWordWrapMode(QTextOption::NoWrap);

    auto treeView = new QTreeView(this);

    auto tabs = new QTabWidget(this);
    tabs->addTab(treeView, "TreeView");
    tabs->addTab(textArea, "Plaintext");

    auto contentLayout = new QVBoxLayout(content);
    contentLayout->addWidget(tabs);

    auto state = ApplicationState::instance();
    connect(state, &ApplicationState::guiStateChanged, this, [=] {
        if (state->guiState() == RESULTS) {
            QString buffer;
            QTextStream stream(&buffer);
            auto result = state->scanResult();
            for (const auto &entry: result.entries) {
                stream << entry->path << ", fileCount=";
                stream << entry->fileCount << ", size=";
                stream << entry->size << ": ";
                if (entry->parent) {
                    stream << " " << entry->parent->path;
                }
                stream << "\n";
            }
            textArea->setPlainText(buffer);

            auto oldModel = treeView->model();
            if (oldModel) {
                auto oldScanEntryModel = dynamic_cast<ScanEntryModel*>(oldModel);
                delete oldScanEntryModel;
            }
            if (m_root) {
                delete m_root;
            }
            m_root = new ScanEntry();
            for (const auto &entry : result.entries) {
                m_root->children.push_back(entry);
            }
            auto model = new ScanEntryModel(m_root);
            treeView->setModel(model);
        }
    });
}

ResultsForm::~ResultsForm() {
    delete m_root;
}

