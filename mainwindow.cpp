#include "closeinformdialog.h"
#include "mainwindow.h"
#include "responsedialog.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QRadioButton>
#include <QSettings>
#include <QClipboard>
#include "languageconstants.h"
#include "commontranslatecore.h"
#include "documenttranslatecore.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    syncTimer=new QTimer(this);
    syncTimer->setSingleShot(true);

    createMenuActions();
    createTrayActions();
    createTrayIcon();
    createTranslateHotkey();
    createLanguageOptions();

    dialogCommonTranslateCore=new CommonTranslateCore();
    connect(dialogCommonTranslateCore,&CommonTranslateCore::resultReceived,[&](QString result){
        ResponseDialog *responseDialog=new ResponseDialog(result);
        responseDialog->show();
        responseDialog->activateWindow();
    });

    mainCommonTranslateCore=new CommonTranslateCore(ui->sourceComboBox->currentData().toString(),
                                                    ui->targetComboBox->currentData().toString());
    connect(mainCommonTranslateCore,&CommonTranslateCore::resultReceived,[&](QString result){
        ui->targetEdit->setText(result);
    });

    statusByteCountLabel=new QLabel("0",this);
    statusByteCountLabel->setAlignment(Qt::AlignRight);
    ui->statusbar->addWidget(statusByteCountLabel,1);

    connect(ui->sourceEdit,&QTextEdit::textChanged,this,&MainWindow::updateStatusByteCountLabel);

    commonUsedPairGroup=new QActionGroup(this);
    connect(commonUsedPairGroup,&QActionGroup::triggered,[&](){
        auto t=commonUsedPairGroup->checkedAction()->text().split(" -> ");
        dialogCommonTranslateCore->setFrom(LanguageConstants::getLanguageMap()[t[0]]);
        dialogCommonTranslateCore->setTo(LanguageConstants::getLanguageMap()[t[1]]);
    });

    readOutDialogSettings();
    readInDialogSettings();

    trayIcon->setIcon(QIcon(":/icons/langSword.svg"));
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!trayIcon->isVisible())
    {
        writeOutDialogSettings();
        //return ;
        qApp->exit();
    }
    if(!notInformAnymore)
    {
        CloseInformDialog closeInformDialog(notInformAnymore,minimizeAfterClose,this);
        if(closeInformDialog.exec()==QDialog::Rejected)
        {
            event->ignore();
            return ;
        }
        notInformAnymore=closeInformDialog.getWhetherNotInformAnymore();
        minimizeAfterClose=closeInformDialog.getClosePurpose();
    }
    writeOutDialogSettings();
    if(minimizeAfterClose)
    {
        hide();
        event->ignore();
        return ;
    }
    qApp->exit();
}

void MainWindow::setIcon()
{

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

}

void MainWindow::createTrayActions()
{
    restoreAction=new QAction(tr("Show Main Interface(&S)"),this);
    connect(restoreAction,&QAction::triggered,[&](){
        showNormal();
        activateWindow();
    });
    selectionTranslateAction=new QAction(tr("Autotranslate The Selection(&A)"),this);
    selectionTranslateAction->setCheckable(true);
    commonUsedPairsStartSep=new QAction(this);
    commonUsedPairsStartSep->setSeparator(true);
    commonUsedPairsEndSep=new QAction(this);
    commonUsedPairsEndSep->setSeparator(true);
    quitAction=new QAction(tr("Quit(&Q)"),this);
    connect(quitAction,&QAction::triggered,qApp,&QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu=new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(selectionTranslateAction);
    trayIconMenu->addAction(commonUsedPairsStartSep);
    trayIconMenu->addAction(commonUsedPairsEndSep);
    trayIconMenu->addAction(quitAction);

    trayIcon=new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::createTranslateHotkey()
{
    translateHotkey=new QHotkey(QKeySequence("Ctrl+G"),true,qApp);
    connect(translateHotkey,&QHotkey::activated,[&](){
        dialogCommonTranslateCore->translate(qApp->clipboard()->text());
    });
}

void MainWindow::createLanguageOptions()
{
    for(auto &i:LanguageConstants::getSourceLanguages())
        ui->sourceComboBox->addItem(i,LanguageConstants::getLanguageMap().value(i));
    for(auto &i:LanguageConstants::getTargetLanguages())
        ui->targetComboBox->addItem(i,LanguageConstants::getLanguageMap().value(i));
    connect(ui->sourceComboBox,&QComboBox::currentTextChanged,[&](const QString &currentText){
        mainCommonTranslateCore->setFrom(ui->sourceComboBox->currentData().toString());
        if(getSyncMode()&&!syncTimer->isActive())
            mainCommonTranslateCore->translate(ui->sourceEdit->toPlainText());
    });
    connect(ui->targetComboBox,&QComboBox::currentTextChanged,[&](const QString &currentText){
        mainCommonTranslateCore->setTo(ui->targetComboBox->currentData().toString());
        if(getSyncMode()&&!syncTimer->isActive())
            mainCommonTranslateCore->translate(ui->sourceEdit->toPlainText());
    });
}

void MainWindow::createMenuActions()
{
    connect(ui->actionSettings,&QAction::triggered,this,&MainWindow::showSettingsDialog);
}

void MainWindow::readOutDialogSettings()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    notInformAnymore=settings.value("notInformAnymore",false).toBool();
    minimizeAfterClose=settings.value("minimizeAfterClose",true).toBool();
    ui->sourceComboBox->setCurrentText(settings.value("mainSourceLanguage",LanguageConstants::defaultSourceLanguage).toString());
    ui->targetComboBox->setCurrentText(settings.value("mainTargetLanguage",LanguageConstants::defaultTargetLanguage).toString());
}

void MainWindow::writeOutDialogSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("notInformAnymore",notInformAnymore);
    settings.setValue("minimizeAfterClose",minimizeAfterClose);
    settings.setValue("mainSourceLanguage",ui->sourceComboBox->currentText());
    settings.setValue("mainTargetLanguage",ui->targetComboBox->currentText());
}

void MainWindow::readInDialogSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    syncMode=false;
    if(settings.value("syncMode",false).toBool())
        changeSyncMode();

    auto commonUsedPairs=settings.value("commonUsedPairs",
                                        QStringList(LanguageConstants::defaultSourceLanguage+" -> "+LanguageConstants::defaultTargetLanguage)).toStringList();
    for(auto &i:qAsConst(commonUsedPairs))
    {
        auto action=new QAction(i,commonUsedPairGroup);
        action->setCheckable(true);
    }
    commonUsedPairGroup->actions()[0]->setChecked(true);
    trayIconMenu->insertActions(commonUsedPairsEndSep,commonUsedPairGroup->actions());
}

void MainWindow::writeInDialogSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("syncMode",syncMode);

    QStringList commonUsedPairs;
    for(auto &i:commonUsedPairGroup->actions())
        commonUsedPairs.append(i->text());
    settings.setValue("commonUsedPairs",commonUsedPairs);
}

void MainWindow::retime(){
    syncTimer->start(500);
}

bool MainWindow::getSyncMode() const
{
    return syncMode;
}

void MainWindow::replaceCommonUsedPairs(QStringList newCommonUsedPairs)
{
    for(auto &i:commonUsedPairGroup->actions())
    {
        trayIconMenu->removeAction(i);
        delete i;
    }
    for(auto &i:qAsConst(newCommonUsedPairs))
    {
        auto action=new QAction(i,commonUsedPairGroup);
        action->setCheckable(true);
    }
    commonUsedPairGroup->actions()[0]->setChecked(true);
    trayIconMenu->insertActions(commonUsedPairsEndSep,commonUsedPairGroup->actions());
}

void MainWindow::changeSyncMode()
{
    syncMode=!syncMode;
    if(syncMode){
        retime();
        connect(syncTimer,&QTimer::timeout,[&](){
            mainCommonTranslateCore->translate(ui->sourceEdit->toPlainText());
        });
        connect(ui->sourceEdit,&QTextEdit::textChanged,this,&MainWindow::retime);
    }else{
        disconnect(ui->sourceEdit,&QTextEdit::textChanged ,this, &MainWindow::retime);
        syncTimer->stop();
    }
}

void MainWindow::updateStatusByteCountLabel()
{
    statusByteCountLabel->setText(QString::number(ui->sourceEdit->toPlainText().toUtf8().length()));
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog settingsDialog(this);
    if(settingsDialog.exec()==QDialog::Accepted)
        writeInDialogSettings();
}

void MainWindow::on_swapLangButton_clicked()
{
    auto p=ui->sourceComboBox->currentText();
    if(p==LanguageConstants::langAuto)
        return ;
    auto tmp=ui->sourceEdit->toPlainText();
    ui->sourceEdit->setText(ui->targetEdit->toPlainText());
    ui->targetEdit->setText(tmp);
    ui->sourceComboBox->setCurrentText(ui->targetComboBox->currentText());
    ui->targetComboBox->setCurrentText(p);
}

void MainWindow::on_clearButton_clicked()
{
    ui->sourceEdit->clear();
    ui->targetEdit->clear();
}
