#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

/**
* @projectName   LangSword
* @brief         This is the implimentation of the QCombox delegate for the Item in QTableView or other
*                item-based widgets. Usually, it's used to show the language optioins.
* @author        Max.D.
* @date          2021-01-17
*/

#include <QItemDelegate>
#include <QObject>

class ComboDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboDelegate(const QStringList &_items,QObject *parent = nullptr);
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QStringList items;
};

#endif // COMBODELEGATE_H
