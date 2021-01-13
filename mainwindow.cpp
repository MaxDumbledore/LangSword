#include "closeinformdialog.h"
#include "mainwindow.h"
#include "responsedialog.h"
#include "ui_mainwindow.h"

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

    translateCore=new TranslateCore();
    mainTranslateCore=new TranslateCore(ui->sourceComboBox->currentData().toString(),
                                        ui->targetComboBox->currentData().toString());
    statusByteCountLabel=new QLabel("0",this);
    statusByteCountLabel->setAlignment(Qt::AlignRight);
    ui->statusbar->addWidget(statusByteCountLabel,1);

    connect(ui->sourceEdit,&QTextEdit::textChanged,this,&MainWindow::updateStatusByteCountLabel);

    readSettings();

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
    writeSettings();
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
    connect(translateHotkey,&QHotkey::activated,this,&MainWindow::translateContentInClipboard);
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

    ui->sourceComboBox->addItem(tr("Auto-detection"),"auto");
    for(auto &i:languageToCode.keys())
    {
        ui->sourceComboBox->addItem(i,languageToCode.value(i));
        ui->targetComboBox->addItem(i,languageToCode.value(i));
    }
    connect(ui->sourceComboBox,&QComboBox::currentTextChanged,[&](const QString &currentText){
        mainTranslateCore->from=ui->sourceComboBox->currentData().toString();
    });
    connect(ui->targetComboBox,&QComboBox::currentTextChanged,[&](const QString &currentText){
        mainTranslateCore->to=ui->targetComboBox->currentData().toString();
    });
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    notInformAnymore=settings.value("notInformAnymore",false).toBool();
    minimizeAfterClose=settings.value("minimizeAfterClose",true).toBool();
    ui->sourceComboBox->setCurrentText(settings.value("defaultSourceLanguage",tr("English")).toString());
    ui->targetComboBox->setCurrentText(settings.value("defaultTargetLanguage",tr("Chinese")).toString());
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("notInformAnymore",notInformAnymore);
    settings.setValue("minimizeAfterClose",minimizeAfterClose);
    settings.setValue("defaultSourceLanguage",ui->sourceComboBox->currentText());
    settings.setValue("defaultTargetLanguage",ui->targetComboBox->currentText());
}

void MainWindow::translateContentInClipboard()
{
    ResponseDialog *responseDialog=new ResponseDialog(translateCore->translate(qApp->clipboard()->text()));
    responseDialog->show();
    responseDialog->activateWindow();
}

void MainWindow::updateStatusByteCountLabel()
{
    statusByteCountLabel->setText(QString::number(ui->sourceEdit->toPlainText().toUtf8().length()));
}

void MainWindow::on_translateButton_clicked()
{
    ui->targetEdit->setText(mainTranslateCore->translate(ui->sourceEdit->toPlainText()));
}

void MainWindow::on_swapLangButton_clicked()
{
    auto p=ui->sourceComboBox->currentText();
    if(p==tr("Auto-detection"))
        return ;
    ui->sourceComboBox->setCurrentText(ui->targetComboBox->currentText());
    ui->targetComboBox->setCurrentText(p);
}

void MainWindow::on_clearButton_clicked()
{
    ui->sourceEdit->clear();
    ui->targetEdit->clear();
}
