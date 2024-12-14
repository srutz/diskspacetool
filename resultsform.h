#ifndef RESULTSFORM_H
#define RESULTSFORM_H

#include <QWidget>
#include <memory>
#include "applicationstate.h"

class ResultsForm : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsForm(QWidget *parent = nullptr);
    virtual ~ResultsForm();

private:
    shared_ptr<ScanEntry> m_root;

signals:

private slots:
    void showContextMenu(const QPoint &pos);
};

#endif // RESULTSFORM_H
