#ifndef STARTSCANFORM_H
#define STARTSCANFORM_H

#include <QWidget>

namespace Ui {
class StartScanForm;
}

class StartScanForm : public QWidget
{
    Q_OBJECT

public:
    explicit StartScanForm(QWidget *parent = nullptr);
    ~StartScanForm();

private:
    Ui::StartScanForm *ui;
};

#endif // STARTSCANFORM_H
