#ifndef RESPONSEDIALOG_H
#define RESPONSEDIALOG_H

#include <QDialog>
#include <QPropertyAnimation>

namespace Ui {
class ResponseDialog;
}

class ResponseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResponseDialog(QString content, QWidget *parent = nullptr);
    ~ResponseDialog();

protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::ResponseDialog *ui;

    QPropertyAnimation *fadeAnimation;

    void createFadeAnimation();
};

#endif // RESPONSEDIALOG_H
