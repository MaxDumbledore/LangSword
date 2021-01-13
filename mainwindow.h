#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "translatecore.h"

#include <QSystemTrayIcon>
#include <QHotkey>
#include <QLabel>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void translateContentInClipboard();
    void updateStatusByteCountLabel();

    void on_translateButton_clicked();

    void on_swapLangButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *statusByteCountLabel;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *restoreAction, *selectionTranslateAction, *quitAction;
    QHotkey *translateHotkey;

    TranslateCore *translateCore,*mainTranslateCore;

    bool notInformAnymore,minimizeAfterClose;

    QMap<QString,QString> languageToCode;

    void createTrayActions();
    void createTrayIcon();
    void createTranslateHotkey();
    void createLanguageOptions();

    void readSettings();
    void writeSettings();
};
#endif

#endif // MAINWINDOW_H
