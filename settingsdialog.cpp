#include "customtabstyle.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mainwindow.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::updateSettings()
{
    auto mainWindow=qobject_cast<MainWindow *>(parent());
    if(mainWindow->getSyncMode()!=ui->SynModeCheckBox->isChecked())
        mainWindow->changeSyncMode();
}
