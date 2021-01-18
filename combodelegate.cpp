#include "combodelegate.h"

#include <QComboBox>

ComboDelegate::ComboDelegate(const QStringList &_items, QObject *parent) :
    QItemDelegate(parent),
    items(_items)
{
}

QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor=new QComboBox(parent);
    editor->hidePopup();
    editor->addItems(items);
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value=index.model()->data(index).toString();
    QComboBox *comboBox=qobject_cast<QComboBox *>(editor);
    comboBox->setCurrentText(value);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox=qobject_cast<QComboBox *>(editor);
    QString value=comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
