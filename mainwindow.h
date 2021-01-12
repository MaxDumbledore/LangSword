#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "translatecore.h"

#include <QSystemTrayIcon>
#include <QHotkey>

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

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *restoreAction, *selectionTranslateAction, *quitAction;
    QHotkey *translateHotkey;

    TranslateCore *translateCore;

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
