#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H
#include <QProxyStyle>

class CustomTabStyle : public QProxyStyle
{
    Q_OBJECT
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = Q_NULLPTR) const override;
};

#endif // CUSTOMTABSTYLE_H
