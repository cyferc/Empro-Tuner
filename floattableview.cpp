#include "floattableview.h"
#include <QHeaderView>
#include <QDebug>
#include <QMouseEvent>

FloatTableView::FloatTableView(QWidget *pParent):
    QTableView(pParent)
{
    setSelectionMode(FloatTableView::ContiguousSelection);

    verticalHeader()->setDefaultSectionSize(20);
    verticalHeader()->hide();

    horizontalHeader()->setDefaultSectionSize(50);
    horizontalHeader()->hide();

    setHorizontalScrollMode(QTableView::ScrollPerPixel);
    setVerticalScrollMode(QTableView::ScrollPerPixel);
}
