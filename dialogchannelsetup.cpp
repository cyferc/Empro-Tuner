#include "dialogchannelsetup.h"
#include "ui_dialogchannelsetup.h"
#include <QMessageBox>
#include <QHeaderView>

#include "mainwindow.h"

DialogChannelSetup::DialogChannelSetup(QWidget *pParent, EcuData *pEcuData, QLCDNumber *pNumberDisplay) :
    QDialog(pParent),
    m_pUi(new Ui::DialogChannelSetup)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_pUi->setupUi(this);
    this->m_pEcuData = pEcuData;

    // Add LCD
    m_pNumberDisplay = pNumberDisplay;
    m_pNumberDisplay->setParent(this);

    QGridLayout *numContentLayout = new QGridLayout(m_pUi->widgetNumberDisplayContainer);
    numContentLayout->setSpacing(0);
    numContentLayout->setContentsMargins(0, 0, 0, 0);

    numContentLayout->addWidget(m_pNumberDisplay);

    m_pTableChannels = new QTableWidget(static_cast<int>(EcuData::eChannelLive::CHANNEL_T_COUNT), 3, this);

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
    m_pTableChannels->verticalHeader()->setDefaultSectionSize(20);
    m_pTableChannels->verticalHeader()->hide();

    // Add channels to table
    for (int channel = 0; channel < static_cast<int>(EcuData::eChannelLive::CHANNEL_T_COUNT); channel++)
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

    QGridLayout *contentLayout = new QGridLayout(m_pUi->widgetTabelContainer);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(m_pTableChannels);

    QObject::connect(m_pTableChannels,
                     SIGNAL(itemSelectionChanged()),
                     this,
                     SLOT(slotTableSelectionChanged()));

    m_pTableChannels->setFocus();
    m_pTableChannels->selectRow(0);
}

DialogChannelSetup::~DialogChannelSetup()
{
    delete m_pUi;
}

void DialogChannelSetup::slotTableSelectionChanged()
{
    // Get selected channel
    int CHANNEL_P = -1;

    for (int i = 0; i < static_cast<int>(EcuData::eChannelLive::CHANNEL_T_COUNT); i++)
    {
        if (m_pEcuData->channelsLive[i]->getName().compare(m_pTableChannels->item(m_pTableChannels->currentRow(), 0)->text()) == 0)
        {
            CHANNEL_P = i;
        }
    }

    if (CHANNEL_P == -1)
    {
        QMessageBox::warning(this, "Error", "Channel name not found");
        return;
    }

    // Update input boxes
    m_pUi->doubleSpinBoxMajorStepSize->setValue(m_pEcuData->channelsLive[CHANNEL_P]->getDialMajorStepSize());
    m_pUi->doubleSpinBoxMinorSteps->setValue(m_pEcuData->channelsLive[CHANNEL_P]->getDialMinorStepSize());
    m_pUi->doubleSpinBoxMax->setValue(m_pEcuData->channelsLive[CHANNEL_P]->getMaxDouble());
    m_pUi->doubleSpinBoxMin->setValue(m_pEcuData->channelsLive[CHANNEL_P]->getMinDouble());

    m_pUi->doubleSpinBoxTestValue->setValue(m_pEcuData->channelsLive[CHANNEL_P]->getMinDouble());
}

void DialogChannelSetup::on_doubleSpinBoxMajorStepSize_valueChanged(double /*arg1*/)
{
    //dial->setScaleStepSize(arg1);
}

void DialogChannelSetup::on_doubleSpinBoxMinorSteps_valueChanged(double /*arg1*/)
{
    //dial->setScaleMaxMinor(arg1);
}

void DialogChannelSetup::on_doubleSpinBoxMax_valueChanged(double /*arg1*/)
{
    //dial->setScale(ui->doubleSpinBoxMin->value(), arg1);
}

void DialogChannelSetup::on_doubleSpinBoxMin_valueChanged(double /*arg1*/)
{
    //dial->setScale(arg1, ui->doubleSpinBoxMax->value());
}

void DialogChannelSetup::on_spinBoxDecimalPlaces_valueChanged(int arg1)
{
    m_pNumberDisplay->setDigitCount(arg1);
    m_pNumberDisplay->display(m_pNumberDisplay->value());
}

void DialogChannelSetup::on_doubleSpinBoxTestValue_valueChanged(double arg1)
{
    //dial->setValue(arg1);
    m_pNumberDisplay->display(arg1);
}
