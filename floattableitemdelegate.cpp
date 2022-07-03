#include "floattableitemdelegate.h"
#include <QDoubleSpinBox>
#include <QPainter>
#include "floattablemodel.h"

FloatTableItemDelegate::FloatTableItemDelegate(QObject *pParent, FloatTableModel *pTableModel)
    : QItemDelegate(pParent)
{
    this->m_pTableModel = pTableModel;
}

QWidget *FloatTableItemDelegate::createEditor(QWidget                     *pParent,
                                              const QStyleOptionViewItem&,
                                              const QModelIndex&          index) const
{
    if (index.row() == (this->m_pTableModel->rowCount() - 1))
    {
        QSpinBox *editor = new QSpinBox(pParent);
        editor->setMinimum(-100000);
        editor->setMaximum(100000);
        editor->setSingleStep(100);
        return editor;
    }

    QDoubleSpinBox *editor = new QDoubleSpinBox(pParent);
    editor->setMinimum(-100000);
    editor->setMaximum(100000);
    editor->setDecimals(3);
    return editor;
}

void FloatTableItemDelegate::setEditorData(QWidget *pEditor, const QModelIndex& index) const
{
    // Bottom row is Int's
    if (index.row() == (this->m_pTableModel->rowCount() - 1))
    {
        float fvalue = index.model()->data(index, Qt::EditRole).toFloat();
        int   value  = (int)fvalue;

        QSpinBox *spinBox = static_cast<QSpinBox *>(pEditor);
        spinBox->setValue(value);
        return;
    }

    float value = index.model()->data(index, Qt::EditRole).toFloat();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(pEditor);
    spinBox->setValue(value);
}

void FloatTableItemDelegate::setModelData(QWidget *pEditor, QAbstractItemModel *pModel,
                                          const QModelIndex& index) const
{
    if (index.row() == (this->m_pTableModel->rowCount() - 1))
    {
        QSpinBox *spinBox = static_cast<QSpinBox *>(pEditor);
        spinBox->interpretText();
        int value = spinBox->value();

        pModel->setData(index, value, Qt::EditRole);

        FloatTableModel* flModel = static_cast<FloatTableModel*>(pModel);
        flModel->calculateColors();
        return;
    }

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(pEditor);
    spinBox->interpretText();
    float value = spinBox->value();

    pModel->setData(index, value, Qt::EditRole);

    FloatTableModel* flModel = static_cast<FloatTableModel*>(pModel);
    flModel->calculateColors();
}

void FloatTableItemDelegate::updateEditorGeometry(QWidget *pEditor,
                                                  const QStyleOptionViewItem& option, const QModelIndex&) const
{
    pEditor->setGeometry(option.rect);
}
