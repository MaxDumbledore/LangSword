#include "closeinformdialog.h"
#include "ui_closeinformdialog.h"

#include<QDebug>

CloseInformDialog::CloseInformDialog(bool notInformAnymore, bool minimizeAfterClose, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloseInformDialog)
{
    ui->setupUi(this);
    setFixedSize(minimumSize());
    if(notInformAnymore)
        ui->checkBox->setChecked(true);
    if(minimizeAfterClose)
        ui->radioButtonM->setChecked(true);
    else
        ui->radioButtonE->setChecked(true);
}

CloseInformDialog::~CloseInformDialog()
{
    delete ui;
}

bool CloseInformDialog::getClosePurpose()
{
    return ui->buttonGroup->checkedButton()==ui->radioButtonM;
}

bool CloseInformDialog::getWhetherNotInformAnymore()
{
    return ui->checkBox->isChecked();
}
