#include "resultsform.h"
#include "applicationstate.h"
#include <QVBoxLayout>
#include <QString>
#include <QTextStream>
#include <QPlainTextEdit>
#include <QTabWidget>

ResultsForm::ResultsForm(QWidget *parent)
    : QWidget{parent}
{
    auto content= new QWidget(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->addWidget(content);

    auto textArea = new QPlainTextEdit(this);
    textArea->setReadOnly(true);
    textArea->setWordWrapMode(QTextOption::NoWrap);

    auto tabs = new QTabWidget(this);
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
                stream << entry->path;
                if (entry->parent) {
                    stream << " " << entry->parent->path;
                }
                stream << "\n";
            }
            textArea->setPlainText(buffer);
        }
    });
}
