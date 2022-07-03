

#include "dialogdatalogsettings.h"
#include "ui_dialogdatalogsettings.h"

#include "mainwindow.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QHeaderView>


DialogDatalogSettings::DialogDatalogSettings(QWidget *pParent, EcuData *pEcuData) :
    QDialog(pParent),
    m_pUi(new Ui::DialogDatalogSettings)
{
    m_pUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);

    int channelCount = static_cast<int>(EcuData::eChannelLive::CHANNEL_T_COUNT);

    m_pTableChannels = new QTableWidget(channelCount, 4, this);

    m_pTableChannels->setHorizontalHeaderItem(0, new QTableWidgetItem("Log"));
    m_pTableChannels->setColumnWidth(0, 40);
    m_pTableChannels->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
    m_pTableChannels->setColumnWidth(1, 130);
    m_pTableChannels->setHorizontalHeaderItem(2, new QTableWidgetItem("Units"));
    m_pTableChannels->setColumnWidth(2, 50);
    m_pTableChannels->setHorizontalHeaderItem(3, new QTableWidgetItem("Description"));
    m_pTableChannels->setColumnWidth(3, 200);

    m_pTableChannels->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableChannels->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableChannels->setAlternatingRowColors(true);
    m_pTableChannels->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableChannels->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableChannels->verticalHeader()->setDefaultSectionSize(20);
    m_pTableChannels->verticalHeader()->hide();

    // Add channels to table
    for (int channel = 0; channel < channelCount; channel++)
    {
        QWidget   *chBoxContainerWidget = new QWidget();
        QCheckBox *chBox = new QCheckBox(chBoxContainerWidget);

        // Centre the checkbox
        QHBoxLayout *pLayout = new QHBoxLayout(chBoxContainerWidget);
        pLayout->addWidget(chBox);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);

        m_pTableChannels->setCellWidget(channel, 0, chBoxContainerWidget);

        QTableWidgetItem *itemName = new QTableWidgetItem(pEcuData->channelsLive[channel]->getName());
        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

        m_pTableChannels->setItem(channel, 1, itemName);

        QTableWidgetItem *itemUnits = new QTableWidgetItem(pEcuData->channelsLive[channel]->getUnits());
        itemUnits->setFlags(itemUnits->flags() ^ Qt::ItemIsEditable);

        m_pTableChannels->setItem(channel, 2, itemUnits);

        QTableWidgetItem *itemDescription = new QTableWidgetItem(pEcuData->channelsLive[channel]->getDescription());
        itemDescription->setFlags(itemDescription->flags() ^ Qt::ItemIsEditable);

        m_pTableChannels->setItem(channel, 3, itemDescription);
    }

    QGridLayout *contentLayout = new QGridLayout(m_pUi->widgetTableContainer);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(m_pTableChannels);

    m_pTableChannels->setFocus();
    m_pTableChannels->selectRow(0);
}

DialogDatalogSettings::~DialogDatalogSettings()
{
    delete m_pUi;
}

/// \brief DialogDatalogSettings::on_pushButtonSelectAll_clicked
/// Select all checkboxes
void DialogDatalogSettings::on_pushButtonSelectAll_clicked()
{
    for (int row = 0; row < m_pTableChannels->rowCount(); row++)
    {
        QWidget *chBoxContainerWidget = qobject_cast<QWidget *>(m_pTableChannels->cellWidget(row, 0));

        QCheckBox *chBox = qobject_cast<QCheckBox *>(chBoxContainerWidget->children().at(0));
        chBox->setChecked(true);
    }
}

/// \brief DialogDatalogSettings::on_pushButtonDeselectAll_clicked
/// Deselect all checkboxes
void DialogDatalogSettings::on_pushButtonDeselectAll_clicked()
{
    for (int row = 0; row < m_pTableChannels->rowCount(); row++)
    {
        QWidget *chBoxContainerWidget = qobject_cast<QWidget *>(m_pTableChannels->cellWidget(row, 0));

        QCheckBox *chBox = qobject_cast<QCheckBox *>(chBoxContainerWidget->children().at(0));
        chBox->setChecked(false);
    }
}
