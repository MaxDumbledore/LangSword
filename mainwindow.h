#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"
#include "commontranslatecore.h"

#include <QSystemTrayIcon>
#include <QHotkey>
#include <QLabel>
#include <QTimer>
#include <QActionGroup>

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
    void changeSyncMode();
    bool getSyncMode() const;
    void replaceCommonUsedPairs(QStringList newCommonUsedPairs);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void updateStatusByteCountLabel();
    void showSettingsDialog();

    void on_swapLangButton_clicked();
    void on_clearButton_clicked();

    void retime();

private:
    Ui::MainWindow *ui;
    QLabel *statusByteCountLabel;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *restoreAction, *selectionTranslateAction,*commonUsedPairsStartSep,*commonUsedPairsEndSep, *quitAction;
    QActionGroup *commonUsedPairGroup;
    QHotkey *translateHotkey;

    CommonTranslateCore *dialogCommonTranslateCore,*mainCommonTranslateCore;

    bool notInformAnymore,minimizeAfterClose;
    bool syncMode;

    QTimer *syncTimer;

    void createTrayActions();
    void createTrayIcon();
    void createTranslateHotkey();
    void createLanguageOptions();
    void createMenuActions();

    void readOutDialogSettings();
    void writeOutDialogSettings();
    void readInDialogSettings();
    void writeInDialogSettings();
};
#endif

#endif // MAINWINDOW_H
