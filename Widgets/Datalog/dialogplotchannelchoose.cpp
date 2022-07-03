#include "dialogplotchannelchoose.h"
#include "ui_dialogplotchannelchoose.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QDebug>
#include <QHeaderView>

DialogPlotChannelChoose::DialogPlotChannelChoose(QList<PointList *> listOfPointLists, QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::DialogPlotChannelChoose)
{
    m_pUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_pTable = new QTableWidget(listOfPointLists.count(), 2, this);
    m_pTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_pTable->setAlternatingRowColors(true);
    m_pTable->verticalHeader()->hide();
    m_pTable->verticalHeader()->setDefaultSectionSize(17);
    m_pTable->setColumnCount(2);

    m_pTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Plot"));
    m_pTable->setColumnWidth(0, 40);

    m_pTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Channel"));
    m_pTable->setColumnWidth(1, 228);

    // Add channels to table
    for (int channel = 0; channel < listOfPointLists.count(); channel++)
    {
        PointList *pList = listOfPointLists.at(channel);

        QWidget   *chBoxContainerWidget = new QWidget();
        QCheckBox *chBox = new QCheckBox(chBoxContainerWidget);

        // Centre the checkbox
        QHBoxLayout *pLayout = new QHBoxLayout(chBoxContainerWidget);
        pLayout->addWidget(chBox);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);

        m_pTable->setCellWidget(channel, 0, chBoxContainerWidget);

        QTableWidgetItem *itemName = new QTableWidgetItem(pList->getName());
        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

        m_pTable->setItem(channel, 1, itemName);
    }

    QGridLayout *contentLayout = new QGridLayout(m_pUi->widgetTableContainer);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(m_pTable);

    m_pTable->setFocus();
    m_pTable->selectRow(0);
}

DialogPlotChannelChoose::~DialogPlotChannelChoose()
{
    delete m_pUi;
}

QList<int> DialogPlotChannelChoose::getSelectedIndices()
{
    QList<int> selectedIndices;

    for (int row = 0; row < m_pTable->rowCount(); row++)
    {
        QWidget *widg = m_pTable->cellWidget(row, 0);
        QCheckBox *chBox = qobject_cast<QCheckBox *>(widg->children().at(0));

        if (chBox->isChecked())
        {
            //qDebug() << row;
            selectedIndices.append(row);
        }
    }

    return selectedIndices;
}

void DialogPlotChannelChoose::on_btnSelectAll_clicked()
{
    for (int row = 0; row < m_pTable->rowCount(); row++)
    {
        QWidget *widg = m_pTable->cellWidget(row, 0);
        QCheckBox *chBox = qobject_cast<QCheckBox *>(widg->children().at(0));

        chBox->setChecked(true);
    }
}

void DialogPlotChannelChoose::on_btnDeselectAll_clicked()
{
    for (int row = 0; row < m_pTable->rowCount(); row++)
    {
        QWidget *widg = m_pTable->cellWidget(row, 0);
        QCheckBox *chBox = qobject_cast<QCheckBox *>(widg->children().at(0));

        chBox->setChecked(false);
    }
}
