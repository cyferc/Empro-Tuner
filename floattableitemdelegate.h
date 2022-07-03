
#pragma once

#include <QObject>
#include <QItemDelegate>
#include <QModelIndex>
#include "floattablemodel.h"

class FloatTableItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    FloatTableItemDelegate(QObject *pParent, FloatTableModel *pTableModel);
    QWidget *createEditor(QWidget *pParent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget *pEditor, const QModelIndex& index) const override;
    void setModelData(QWidget *pEditor, QAbstractItemModel *pModel, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget *pEditor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    FloatTableModel *m_pTableModel;
};
