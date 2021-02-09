#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"
#include "commontranslatecore.h"

#include <QSystemTrayIcon>
#include <QHotkey>
#include <QLabel>
#include <QTimer>
#include <QActionGroup>
#include <QCheckBox>

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
    QStringList getCommonUsedPairs();

    bool getNotInformAnymore() const;
    void setNotInformAnymore(bool value);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void updateStatusByteCountLabel();

    void on_swapLangButton_clicked();
    void on_clearButton_clicked();

    void resetSyncTimer();

private:
    Ui::MainWindow *ui;
    QLabel *statusByteCountLabel;
    QCheckBox *singleLineCheckBox;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *restoreAction, *selectionTranslateAction,*commonUsedPairsStartSep,*commonUsedPairsEndSep, *quitAction;
    QActionGroup *commonUsedPairGroup;
    QHotkey *translateHotkey;

    CommonTranslateCore *dialogCommonTranslateCore,*mainCommonTranslateCore;

    bool notInformAnymore,minimizeAfterClose;

    bool syncMode;
    QTimer *syncTimer;

    QTimer *selectionTimer;
    QKeyEvent *copyKeyPressEvent,*copyKeyReleaseEvent;
    QPoint lastCursorPos;
    bool isTranslated;

    void createTrayActions();
    void createTrayIcon();
    void createShortCutTranslateFunction();
    void createLanguageOptions();
    void createMenuActions();
    //void createSelectionTranslateFunction();

    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
