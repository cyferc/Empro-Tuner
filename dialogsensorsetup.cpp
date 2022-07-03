#include "dialogsensorsetup.h"
#include "ui_dialogsensorsetup.h"

#include "mainwindow.h"
#include <QtWidgets>


/**
 * this tab style makes the tab text horizontal when the tab is set to West
 */
class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize& size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);

        if (type == QStyle::CT_TabBarTab)
        {
            s.transpose();
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QStyleOptionTab opt(*tab);
                opt.shape = QTabBar::RoundedNorth;
                QProxyStyle::drawControl(element, &opt, painter, widget);
                return;
            }
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
};

DialogSensorSetup::DialogSensorSetup(QWidget *pParent, EcuData *pEcuData) :
    QDialog(pParent),
    ui(new Ui::DialogSensorSetup)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);

    addNumberDisplay(ui->groupBoxRawADC, pEcuData->channelsLive[static_cast<int>(pEcuData->eChannelLive::CH_ADC0)]);
}

DialogSensorSetup::~DialogSensorSetup()
{
    delete ui;
}

void DialogSensorSetup::addNumberDisplay(QWidget *pParentWidget, VocalObject *pChannel)
{
    QLCDNumber *mynum = new QLCDNumber();

    mynum->setSegmentStyle(QLCDNumber::Flat);
    mynum->setAutoFillBackground(true);
    mynum->setFrameStyle(QFrame::NoFrame);

    QPalette mynumPalette = mynum->palette();
    mynumPalette.setColor(mynumPalette.Background, Qt::black);
    mynumPalette.setColor(mynumPalette.Foreground, Qt::yellow);
    mynum->setPalette(mynumPalette);

    mynum->display(pChannel->getValueDouble());

    QObject::connect(pChannel,
                     SIGNAL(valueChangedDouble(double)),
                     mynum,
                     SLOT(display(double)));

    QGridLayout *contentLayout = new QGridLayout(pParentWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(mynum);
}
