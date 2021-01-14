#include "customtabstyle.h"

#include <QStyleOptionTab>
#include <QTabBar>


QSize CustomTabStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize s=QProxyStyle::sizeFromContents(type,option,size,widget);
    if(type==QStyle::CT_TabBarTab)
        s.transpose();
    return s;
}

void CustomTabStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element==CE_TabBarTabLabel){
        auto ptrOpt=qstyleoption_cast<const QStyleOptionTab *>(option);
        if(ptrOpt){
            auto newOpt(*ptrOpt);
            newOpt.shape=QTabBar::RoundedNorth;
            QProxyStyle::drawControl(element,&newOpt,painter,widget);
            return ;
        }
    }
    QProxyStyle::drawControl(element,option,painter,widget);
}
