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

    connect(qApp,&QApplication::aboutToQuit,this,&MainWindow::writeSettings);

    createMenuActions();
    createTrayActions();
    createTrayIcon();
    createShortCutTranslateFunction();
    //createSelectionTranslateFunction();
    createLanguageOptions();

    dialogCommonTranslateCore=new CommonTranslateCore();
    connect(dialogCommonTranslateCore,&CommonTranslateCore::resultReceived,[&](QString result){
        ResponseDialog *responseDialog=new ResponseDialog(result);
        responseDialog->show();
        responseDialog->activateWindow();
    });

    syncTimer=new QTimer(this);
    syncTimer->setSingleShot(true);
    mainCommonTranslateCore=new CommonTranslateCore(ui->sourceComboBox->currentData().toString(),
                                                    ui->targetComboBox->currentData().toString());
    connect(mainCommonTranslateCore,&CommonTranslateCore::resultReceived,[&](QString result){
        ui->targetEdit->setText(result);
    });

    singleLineCheckBox=new QCheckBox(tr("Merge to a single line"),this);
    ui->statusbar->addWidget(singleLineCheckBox);
    ui->statusbar->setContentsMargins(10,0,0,0);
    connect(singleLineCheckBox,&QCheckBox::stateChanged,[&](){
        dialogCommonTranslateCore->changeSingleLineMode();
        mainCommonTranslateCore->changeSingleLineMode();
        mainCommonTranslateCore->translate(ui->sourceEdit->toPlainText());
    });

    statusByteCountLabel=new QLabel("0",this);
    statusByteCountLabel->setAlignment(Qt::AlignRight);
    ui->statusbar->addWidget(statusByteCountLabel,1);

    connect(ui->sourceEdit,&QTextEdit::textChanged,this,&MainWindow::updateStatusByteCountLabel);

    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!trayIcon->isVisible())
        qApp->quit();
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
    if(minimizeAfterClose)
    {
        hide();
        event->ignore();
        return ;
    }
    qApp->quit();
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
    connect(quitAction,&QAction::triggered,qApp,&QApplication::quit);

    commonUsedPairGroup=new QActionGroup(this);
    connect(commonUsedPairGroup,&QActionGroup::triggered,[&](){
        auto t=commonUsedPairGroup->checkedAction()->text().split(" -> ");
        dialogCommonTranslateCore->setFrom(LanguageConstants::getLanguageMap()[t[0]]);
        dialogCommonTranslateCore->setTo(LanguageConstants::getLanguageMap()[t[1]]);
    });
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

    trayIcon->setIcon(QIcon(":/icons/langSword.svg"));
    trayIcon->show();
}

void MainWindow::createShortCutTranslateFunction()
{
    translateHotkey=new QHotkey(QKeySequence("Ctrl+G"),true,qApp);
    connect(translateHotkey,&QHotkey::activated,[&](){
        dialogCommonTranslateCore->translate(qApp->clipboard()->text());
    });
}

//detach

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
    connect(ui->actionSettings,&QAction::triggered,[&](){
        SettingsDialog(this).exec();
    });
}

/*
void MainWindow::createSelectionTranslateFunction()
{
    connect(qApp->clipboard(),&QClipboard::dataChanged,[&](){
         //dialogCommonTranslateCore->translate(qApp->clipboard()->text());
        qDebug()<<qApp->clipboard()->text();
    });
    isTranslated=false;

    copyKeyPressEvent=new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
    copyKeyReleaseEvent=new QKeyEvent(QEvent::KeyRelease, Qt::Key_C, Qt::ControlModifier);
    selectionTimer=new QTimer(this);
    lastCursorPos=QCursor::pos();
    connect(selectionTimer,&QTimer::timeout,[&](){
        QPoint nPos=QCursor::pos();
        if(nPos==lastCursorPos)
        {
            if(!isTranslated)
            {
                QCoreApplication::sendEvent(qApp, copyKeyPressEvent);
                QCoreApplication::sendEvent(qApp, copyKeyReleaseEvent);
                //dialogCommonTranslateCore->translate(qApp->clipboard()->text());
                //disconnect(qApp->clipboard(),&QClipboard::changed,nullptr,nullptr);
                isTranslated=true;
            }
        }
        else
        {
            lastCursorPos=nPos;
            isTranslated=false;
        }
    });
    selectionTimer->start(500);
}*/

//detach

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    notInformAnymore=settings.value("notInformAnymore",false).toBool();
    minimizeAfterClose=settings.value("minimizeAfterClose",true).toBool();
    ui->sourceComboBox->setCurrentText(settings.value("mainSourceLanguage",LanguageConstants::defaultSourceLanguage).toString());
    ui->targetComboBox->setCurrentText(settings.value("mainTargetLanguage",LanguageConstants::defaultTargetLanguage).toString());

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

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("notInformAnymore",notInformAnymore);
    settings.setValue("minimizeAfterClose",minimizeAfterClose);
    settings.setValue("mainSourceLanguage",ui->sourceComboBox->currentText());
    settings.setValue("mainTargetLanguage",ui->targetComboBox->currentText());

    settings.setValue("syncMode",syncMode);

    QStringList commonUsedPairs;
    for(auto &i:commonUsedPairGroup->actions())
        commonUsedPairs.append(i->text());
    settings.setValue("commonUsedPairs",commonUsedPairs);
}

void MainWindow::resetSyncTimer()
{
    syncTimer->start(500);
}

bool MainWindow::getNotInformAnymore() const
{
    return notInformAnymore;
}

void MainWindow::setNotInformAnymore(bool value)
{
    notInformAnymore = value;
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

QStringList MainWindow::getCommonUsedPairs()
{
    QStringList list;
    for(auto &i:commonUsedPairGroup->actions())
        list.append(i->text());
    return list;
}

void MainWindow::changeSyncMode()
{
    syncMode=!syncMode;
    if(syncMode){
        resetSyncTimer();
        connect(syncTimer,&QTimer::timeout,[&](){
            mainCommonTranslateCore->translate(ui->sourceEdit->toPlainText());
        });
        connect(ui->sourceEdit,&QTextEdit::textChanged,this,&MainWindow::resetSyncTimer);
    }else{
        disconnect(ui->sourceEdit,&QTextEdit::textChanged ,this, &MainWindow::resetSyncTimer);
        syncTimer->stop();
    }
}

void MainWindow::updateStatusByteCountLabel()
{
    statusByteCountLabel->setText(QString::number(ui->sourceEdit->toPlainText().toUtf8().length()));
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
//
