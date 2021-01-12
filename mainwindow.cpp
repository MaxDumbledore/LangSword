#include "closeinformdialog.h"
#include "mainwindow.h"
#include "responsedialog.h"
#include "ui_mainwindow.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QCheckBox>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QRadioButton>
#include <QSettings>
#include <QClipboard>
#include "translatecore.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createTrayActions();
    createTrayIcon();
    createTranslateHotkey();
    createLanguageOptions();

    readSettings();

    translateCore=new TranslateCore();

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
        writeSettings();
        return ;
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
    writeSettings();
    if(minimizeAfterClose)
    {
        hide();
        event->ignore();
        return ;
    }
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
    connect(restoreAction,&QAction::triggered,this,&QWidget::showNormal);
    selectionTranslateAction=new QAction(tr("Autotranslate The Selection(&A)"),this);
    selectionTranslateAction->setCheckable(true);
    quitAction=new QAction(tr("Quit(&Q)"),this);
    connect(quitAction,&QAction::triggered,qApp,&QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu=new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(selectionTranslateAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon=new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::createTranslateHotkey()
{
    translateHotkey=new QHotkey(QKeySequence("Ctrl+G"),true,qApp);
    QObject::connect(translateHotkey,&QHotkey::activated,this,&MainWindow::translateContentInClipboard);
}

void MainWindow::createLanguageOptions()
{
    languageToCode[tr("Chinese")]="zh";
    languageToCode[tr("English")]="en";
    languageToCode[tr("Cantonese")]="yue";
    languageToCode[tr("Classical Chinese")]="wyw";
    languageToCode[tr("Japanese")]="jp";
    languageToCode[tr("Korean")]="kor";
    languageToCode[tr("French")]="fra";
    languageToCode[tr("Spanish")]="spa";
    languageToCode[tr("Thai")]="th";
    languageToCode[tr("Arabic")]="ara";
    languageToCode[tr("Russian")]="ru";
    languageToCode[tr("Portuguese")]="pt";
    languageToCode[tr("German")]="de";
    languageToCode[tr("Italian")]="it";
    languageToCode[tr("Greek")]="el";
    languageToCode[tr("Dutch")]="nl";
    languageToCode[tr("Polish")]="pl";
    languageToCode[tr("Bulgarian")]="bul";
    languageToCode[tr("Estonian")]="est";
    languageToCode[tr("Danish")]="dan";
    languageToCode[tr("Finland")]="fin";
    languageToCode[tr("Czech")]="cs";
    languageToCode[tr("Romanian")]="rom";
    languageToCode[tr("Slovenian")]="slo";
    languageToCode[tr("Swedish")]="swe";
    languageToCode[tr("Hungarian")]="hu";
    languageToCode[tr("Traditional Chinese")]="cht";
    languageToCode[tr("Vietnamese")]="vie";
    for(auto &i:languageToCode.keys())
    {
        ui->sourceComboBox->addItem(i,languageToCode.value(i));
        ui->targetComboBox->addItem(i,languageToCode.value(i));
    }
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    notInformAnymore=settings.value("notInformAnymore",false).toBool();
    minimizeAfterClose=settings.value("minimizeAfterClose",true).toBool();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("notInformAnymore",notInformAnymore);
    settings.setValue("minimizeAfterClose",minimizeAfterClose);
    //settings.setValue("defaultSourceLanguage",ui->sourceComboBox->currentText());
    //settings

}

void MainWindow::translateContentInClipboard()
{
    ResponseDialog *responseDialog=new ResponseDialog(translateCore->translate(qApp->clipboard()->text()),this);
    responseDialog->show();
    responseDialog->activateWindow();
}

#endif
