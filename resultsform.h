#ifndef RESULTSFORM_H
#define RESULTSFORM_H

#include <QWidget>
#include "applicationstate.h"

class ResultsForm : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsForm(QWidget *parent = nullptr);
    virtual ~ResultsForm();

private:
    ScanEntry* m_root;

signals:
};

#endif // RESULTSFORM_H
