#include "dialogchannelchoose.h"
#include "ui_dialogchannelchoose.h"
#include <QGridLayout>
#include <QHeaderView>
#include "mainwindow.h"

DialogChannelChoose::DialogChannelChoose(QWidget *pParent, EcuData *pEcuData) :
    QDialog(pParent),
    m_pUi(new Ui::DialogChannelChoose)
{
    m_pUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    int channelCount = static_cast<int>(EcuData::eChannelLive::CHANNEL_T_COUNT);

    m_pTableChannels = new QTableWidget(channelCount, 3, this);

    m_pTableChannels->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    m_pTableChannels->setColumnWidth(0, 130);
    m_pTableChannels->setHorizontalHeaderItem(1, new QTableWidgetItem("Units"));
    m_pTableChannels->setColumnWidth(1, 50);
    m_pTableChannels->setHorizontalHeaderItem(2, new QTableWidgetItem("Description"));
    m_pTableChannels->setColumnWidth(2, 200);

    m_pTableChannels->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableChannels->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableChannels->setAlternatingRowColors(true);
    m_pTableChannels->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableChannels->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableChannels->verticalHeader()->setDefaultSectionSize(17);
    m_pTableChannels->verticalHeader()->hide();

    // Add channels to table
    for (int channel = 0; channel < channelCount; channel++)
    {
        QTableWidgetItem *itemName = new QTableWidgetItem(pEcuData->channelsLive[channel]->getName());
        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

        m_pTableChannels->setItem(channel, 0, itemName);

        QTableWidgetItem *itemUnits = new QTableWidgetItem(pEcuData->channelsLive[channel]->getUnits());
        itemUnits->setFlags(itemUnits->flags() ^ Qt::ItemIsEditable);

        m_pTableChannels->setItem(channel, 1, itemUnits);

        QTableWidgetItem *itemDescription = new QTableWidgetItem(pEcuData->channelsLive[channel]->getDescription());
        itemDescription->setFlags(itemDescription->flags() ^ Qt::ItemIsEditable);

        m_pTableChannels->setItem(channel, 2, itemDescription);
    }

    QGridLayout *contentLayout = new QGridLayout(m_pUi->widgetTableContainer);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(m_pTableChannels);

    m_pTableChannels->setFocus();
    m_pTableChannels->selectRow(0);
}

DialogChannelChoose::~DialogChannelChoose()
{
    delete m_pUi;
}

QString DialogChannelChoose::getSelectedChannel()
{
    return m_pTableChannels->item(m_pTableChannels->currentRow(), 0)->text();
}
