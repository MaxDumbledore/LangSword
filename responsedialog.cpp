#include "responsedialog.h"
#include "ui_responsedialog.h"

#include <QCloseEvent>
#include <QScrollBar>
#include <QFontMetrics>
#include <QDebug>

const double initialOpacity=0.7;

ResponseDialog::ResponseDialog(QString content, QWidget *parent) :
    QDialog(parent, Qt::WindowStaysOnTopHint|Qt::Tool),
    ui(new Ui::ResponseDialog)
{
    ui->setupUi(this);
    ui->textEdit->setText(content);
    move(QCursor::pos());
    setWindowOpacity(initialOpacity);
    auto font=ui->textEdit->document()->defaultFont();
    auto fontMetrics=new QFontMetrics(font);
    auto textSize=fontMetrics->size(0,content);
    resize(width(), textSize.width()*textSize.height()/ui->textEdit->width());
    ui->textEdit->moveCursor(QTextCursor::End);
    createFadeAnimation();
}

ResponseDialog::~ResponseDialog()
{
    delete ui;
}

void ResponseDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    if(event->type()==QEvent::ActivationChange)
    {
        if(isActiveWindow())
        {
            //qDebug()<<"in"<<endl;
            fadeAnimation->stop();
            setWindowOpacity(initialOpacity);
        }
        else
        {
            //qDebug()<<"out"<<endl;
            //
            fadeAnimation->start();
        }
    }
}

void ResponseDialog::closeEvent(QCloseEvent *event)
{
    deleteLater();
    event->ignore();
}

void ResponseDialog::createFadeAnimation()
{
    fadeAnimation=new QPropertyAnimation(this,"windowOpacity",this);
    fadeAnimation->setEndValue(0);
    fadeAnimation->setDuration(3000);
    fadeAnimation->setEasingCurve(QEasingCurve::OutBack);
    connect(fadeAnimation,&QPropertyAnimation::finished,this,&QDialog::close);
}
