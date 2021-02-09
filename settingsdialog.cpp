#include "customtabstyle.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mainwindow.h"
#include "combodelegate.h"
#include "languageconstants.h"

#include <QComboBox>
#include <QSettings>
#include <QStandardItemModel>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle());
    getSettingsFromMainWindow();

    connect(this,&QDialog::accepted,this,&SettingsDialog::updateSettings);
    connect(ui->addButton,&QPushButton::clicked,[&](){
        pModel->appendRow(QList<QStandardItem *>({new QStandardItem(LanguageConstants::defaultSourceLanguage),
                                                  new QStandardItem(LanguageConstants::defaultTargetLanguage)}));
    });
    connect(ui->deleteButton,&QPushButton::clicked,[&](){
        pModel->removeRow(ui->pairsView->currentIndex().row());
    });
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::updateSettings()
{
    auto mainWindow=qobject_cast<MainWindow *>(parent());
    if(mainWindow->getSyncMode()!=ui->syncModeCheckBox->isChecked())
        mainWindow->changeSyncMode();
    mainWindow->setNotInformAnymore(!ui->informCheckBox->isChecked());
    QStringList list;
    for(auto i=0;i<pModel->rowCount();i++)
        list.append(pModel->item(i,0)->text()+" -> "+pModel->item(i,1)->text());
    mainWindow->replaceCommonUsedPairs(list);
}

void SettingsDialog::getSettingsFromMainWindow()
{
    auto mainWindow=qobject_cast<MainWindow *>(parent());
    ui->syncModeCheckBox->setChecked(mainWindow->getSyncMode());
    ui->informCheckBox->setChecked(!mainWindow->getNotInformAnymore());

    auto list=mainWindow->getCommonUsedPairs();
    pModel= new QStandardItemModel(list.size(),2,this);
    ui->pairsView->setModel(pModel);
    ui->pairsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pairsView->verticalHeader()->show();
    ComboDelegate *sourceComboBox=new ComboDelegate(LanguageConstants::getSourceLanguages());
    ComboDelegate *targetComboBox=new ComboDelegate(LanguageConstants::getTargetLanguages());
    ui->pairsView->setItemDelegateForColumn(0,sourceComboBox);
    ui->pairsView->setItemDelegateForColumn(1,targetComboBox);

    for(int i=0;i<list.size();i++)
    {
        auto temp=list[i].split(" -> ");
        pModel->setItem(i,0,new QStandardItem(temp[0]));
        pModel->setItem(i,1,new QStandardItem(temp[1]));
    }
}

