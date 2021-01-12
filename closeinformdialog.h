#ifndef CLOSEINFORMDIALOG_H
#define CLOSEINFORMDIALOG_H

#include <QDialog>

namespace Ui {
class CloseInformDialog;
}

class CloseInformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CloseInformDialog(bool notInformAnymore, bool minimizeAfterClose, QWidget *parent = nullptr);
    ~CloseInformDialog();

    bool getClosePurpose();
    bool getWhetherNotInformAnymore();

private:
    Ui::CloseInformDialog *ui;
};

#endif // CLOSEINFORMDIALOG_H
