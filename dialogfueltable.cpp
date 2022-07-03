#include "dialogfueltable.h"
#include "ui_dialogfueltable.h"
#include "floattableitemdelegate.h"
#include "verticallabel.h"
#include <QGridLayout>
#include <QDebug>
#include "helpers.h"

DialogFuelTable::DialogFuelTable(QWidget *pParent, FloatTableModel *pTableModel) :
    QDialog(pParent),
    m_pUi(new Ui::DialogFuelTable)
{
    m_pUi->setupUi(this);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::WindowMaximizeButtonHint);
    m_FirstShow = true;

    this->m_pTableModel = pTableModel;

    m_pTableView = new FloatTableView(this);
    m_pTableView->setModel(pTableModel);
    this->setStyleSheet("FloatTableView { background-color: black; selection-background-color: rgb(0, 0, 255) }");

    FloatTableItemDelegate *delegate = new FloatTableItemDelegate(this, pTableModel);
    m_pTableView->setItemDelegate(delegate);

    QGridLayout *contentLayout = new QGridLayout(m_pUi->widgetTableContent);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(m_pTableView);

    VerticalLabel *lbl = new VerticalLabel(QString("MAP"), this);
    lbl->setMinimumWidth(15);

    QGridLayout *verticalLblContentLayout = new QGridLayout(m_pUi->widgetVerticalLblPlaceholder);
    verticalLblContentLayout->setSpacing(0);
    verticalLblContentLayout->setContentsMargins(0, 0, 0, 0);
    verticalLblContentLayout->addWidget(lbl);
}

DialogFuelTable::~DialogFuelTable()
{
    delete m_pUi;
}

void DialogFuelTable::showEvent(QShowEvent */*pEvent*/)
{
    if (!m_FirstShow)
    {
        move(m_PosX, m_PosY);
    }
}

void DialogFuelTable::closeEvent(QCloseEvent *pEvent)
{
    m_FirstShow = false;
    m_PosX      = x();
    m_PosY      = y();

    pEvent->accept(); // close dialog
}

void DialogFuelTable::on_buttonAddRow_clicked()
{
    m_pTableModel->insertRow(0);
}

void DialogFuelTable::on_buttonRemoveRow_clicked()
{
    m_pTableModel->removeRow(0);
}

void DialogFuelTable::on_buttonAddColumn_clicked()
{
    m_pTableModel->insertColumn(m_pTableModel->columnCount());
}

void DialogFuelTable::on_buttonRemoveColumn_clicked()
{
    m_pTableModel->removeColumn(m_pTableModel->columnCount() - 1);
}

void DialogFuelTable::on_pushButtonEditHeaders_clicked()
{
    if (m_pTableModel->getSelectableArea() == FloatTableModel::SelectableBody)
        m_pTableModel->setSelectableArea(FloatTableModel::SelectableColumnHeaders);
    else if (m_pTableModel->getSelectableArea() == FloatTableModel::SelectableColumnHeaders)
        m_pTableModel->setSelectableArea(FloatTableModel::SelectableRowHeaders);
    else if (m_pTableModel->getSelectableArea() == FloatTableModel::SelectableRowHeaders)
        m_pTableModel->setSelectableArea(FloatTableModel::SelectableBody);
}

void DialogFuelTable::on_pushButtonInterpolate_clicked()
{
    QModelIndexList indexList = m_pTableView->selectionModel()->selectedIndexes();

    if (indexList.size() < 1)
    {
        m_pTableView->setFocus();
        return;
    }

    bool isRowInterpolatable = false;
    bool isColumnInterpolatable = false;
    bool isBilinInterpolatable = false;

    int prevRow = indexList.at(0).row();
    int prevCol = indexList.at(0).column();

    for (QModelIndex index : indexList)
    {
        if (prevRow != index.row())
            isRowInterpolatable = true;
        if (prevCol != index.column())
            isColumnInterpolatable = true;
    }

    if (isRowInterpolatable && isColumnInterpolatable)
        isBilinInterpolatable = true;

    // Always call this first
    if (isBilinInterpolatable)
    {
        int firstRow = indexList.at(0).row();
        int lastRow = indexList.at(indexList.size() - 1).row();
        int firstColumn = indexList.at(0).column();
        int lastColumn = indexList.at(indexList.size() - 1).column();

        for (int currentCellRow = firstRow; currentCellRow <= lastRow; currentCellRow++)
        {
            for (int currentCellColumn = firstColumn; currentCellColumn <= lastColumn; currentCellColumn++)
            {
                double q11 = m_pTableModel->data(m_pTableModel->index(lastRow, firstColumn), Qt::EditRole).toFloat();
                double q12 = m_pTableModel->data(m_pTableModel->index(firstRow, firstColumn), Qt::EditRole).toFloat();
                double q21 = m_pTableModel->data(m_pTableModel->index(lastRow, lastColumn), Qt::EditRole).toFloat();
                double q22 = m_pTableModel->data(m_pTableModel->index(firstRow, lastColumn), Qt::EditRole).toFloat();

                double result = Helpers::BilinearInterpolation(q11, q12, q21, q22,
                                                               firstColumn, lastColumn, lastRow, firstRow,
                                                               currentCellColumn, currentCellRow);

                m_pTableModel->setData(m_pTableModel->index(currentCellRow, currentCellColumn), result);
            }
        }
        m_pTableModel->calculateColors();
        m_pTableView->setFocus();
    }
    else if (isRowInterpolatable)
    {
        int column = indexList.at(0).column();
        int firstRow = indexList.at(0).row();
        int lastRow = indexList.at(indexList.size() - 1).row();

        for (int currentCellRow = firstRow + 1; currentCellRow < lastRow; currentCellRow++)
        {
            // x.. = cells
            // y.. = values
            double x0 = firstRow;
            double y0 = indexList.at(0).data(Qt::EditRole).toFloat();

            double x1 = lastRow;
            double y1 = indexList.at(indexList.size() - 1).data(Qt::EditRole).toFloat();

            double x = currentCellRow;

            double y = Helpers::LinearInterpolation(x0, y0, x1, y1, x);

            m_pTableModel->setData(m_pTableModel->index(currentCellRow, column), y);
        }
        m_pTableModel->calculateColors();
        m_pTableView->setFocus();
    }
    else if (isColumnInterpolatable)
    {
        int row = indexList.at(0).row();
        int firstColumn = indexList.at(0).column();
        int lastColumn = indexList.at(indexList.size() - 1).column();

        for (int currentCellColumn = firstColumn + 1; currentCellColumn < lastColumn; currentCellColumn++)
        {
            // x.. = cells
            // y.. = values
            double x0 = firstColumn;
            double y0 = indexList.at(0).data(Qt::EditRole).toFloat();

            double x1 = lastColumn;
            double y1 = indexList.at(indexList.size() - 1).data(Qt::EditRole).toFloat();

            double x = currentCellColumn;

            double y = Helpers::LinearInterpolation(x0, y0, x1, y1, x);

            m_pTableModel->setData(m_pTableModel->index(row, currentCellColumn), y);
        }
        m_pTableModel->calculateColors();
        m_pTableView->setFocus();
    }
}

void DialogFuelTable::on_pushButtonIncrement_clicked()
{
    QModelIndexList indexList = m_pTableView->selectionModel()->selectedIndexes();

    for (QModelIndex index : indexList)
    {
        float newVal = m_pTableModel->data(index, Qt::EditRole).toFloat() + m_pUi->doubleSpinBoxIncrementAmount->value();
        m_pTableModel->setData(index, newVal, Qt::EditRole);
    }

    m_pTableModel->calculateColors();
    m_pTableView->setFocus();
}

void DialogFuelTable::on_pushButtonDecrement_clicked()
{
    QModelIndexList indexList = m_pTableView->selectionModel()->selectedIndexes();

    for (QModelIndex index : indexList)
    {
        float newVal = m_pTableModel->data(index, Qt::EditRole).toFloat() - m_pUi->doubleSpinBoxIncrementAmount->value();
        m_pTableModel->setData(index, newVal, Qt::EditRole);
    }

    m_pTableModel->calculateColors();
    m_pTableView->setFocus();
}
