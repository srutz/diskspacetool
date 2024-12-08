#include "startscanform.h"
#include "ui_startscanform.h"

StartScanForm::StartScanForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartScanForm)
{
    ui->setupUi(this);

}

StartScanForm::~StartScanForm()
{
    delete ui;
}
