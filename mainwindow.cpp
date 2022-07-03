/**
 * To do:
 *
 *
 **/


#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QSpinBox>
#include <QObject>
#include <QLCDNumber>
#include <QMdiArea>
#include <QTableWidget>
#include <QGridLayout>

/*
 * // this is how to connect signal/slots
 * QObject::connect(ui->doubleSpinBox,
 *               SIGNAL(valueChanged(double)),
 *               channels[CH_RPM],
 *               SLOT(setValueDouble(double)));
 *
 * QObject::connect(channels[CH_RPM],
 *               SIGNAL(valueChangedInt(int)),
 *               ui->spinBox_2,
 *               SLOT(setValue(int)));
 */

#include <QWidget>
#include <QInputDialog>
#include <QStyle>
#include <QToolButton>

#include "Widgets/Datalog/plotdatalog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogchannelchoose.h"
#include "dialogchannelsetup.h"
#include "dialogsensorsetup.h"
#include "dialogecusettings.h"
#include "Widgets/RoundDial/round_dial.h"
#include "Widgets/Datalog/widgetdatalogviewcontrol.h"

MainWindow::MainWindow(QWidget *pParent) : QMainWindow(pParent), m_pUi(new Ui::MainWindow)
{
    m_pUi->setupUi(this);

    // start Maximized
    this->showMaximized();

    m_pEcuData = new EcuData();

    // Create data models
    m_pFuelTableModel = new FloatTableModel(this);

    /// Central Tab Widget ///
    m_pTabWidget = new TabWidget(this);
    setCentralWidget(m_pTabWidget);

    QObject::connect(m_pTabWidget,
                     SIGNAL(currentChanged(int)),
                     this,
                     SLOT(tabWidgetChanged(int)));

    QObject::connect(m_pTabWidget,
                     SIGNAL(customContextMenuRequested(QPoint)),
                     this,
                     SLOT(tabWidgetCustomContextMenuRequested(QPoint)));

    //////// Dock Windows ////////
    m_pUi->menuDockWindows->addAction(m_pUi->dockWidgetAlgorithmDesigner->toggleViewAction());

    //////// Serial ////////
    m_pSerial = new Serial(this, m_pEcuData);
    m_pSerialSettingsDialog  = new DialogSerialPortSettings(m_pSerial, this);
    m_pDialogFuelTable       = new DialogFuelTable(this, m_pFuelTableModel);
    m_pDialogDatalogSettings = new DialogDatalogSettings(this, m_pEcuData);

    //////// Tab Right Click Menu ////////
    m_pTabWidgetContextMenu = new QMenu(m_pTabWidget);
    m_pTabWidgetContextMenu->addAction(m_pUi->actionAdd_Bar_Display);
    m_pTabWidgetContextMenu->addAction(m_pUi->actionAdd_Data_Log_Display);
    m_pTabWidgetContextMenu->addAction(m_pUi->actionAdd_Dial_Display);
    m_pTabWidgetContextMenu->addAction(m_pUi->actionAdd_Number_Dislpay);

    m_pLblStatusBarConnected = new QLabel(m_pUi->statusBar);
    this->setEcuConnected(false);
    m_pUi->statusBar->addWidget(m_pLblStatusBarConnected);

    m_pLblStatusBarMsg1 = new QLabel(m_pUi->statusBar);
    m_pLblStatusBarMsg1->setText("");

    m_pUi->statusBar->addWidget(m_pLblStatusBarMsg1, 1);

    //////// Initial channel values ////////
    // Set initial values after the entire display has been set up, so that the display can be updated
    setInitialChannelValues();

    /// Qt Nodes Editor ///
    m_pQne = new Qne(m_pUi->dockWidgetAlgorithmDesigner);

    // Add qne widget to window
    m_pUi->dockWidgetAlgorithmDesigner->setWidget(m_pQne);
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}

void MainWindow::setInitialChannelValues()
{
    // Realtime variables
    int rpmIndex = static_cast<int>(EcuData::eChannelLive::CH_RPM);
    m_pEcuData->channelsLive[rpmIndex]->setDialMinorStepSize(100);
    m_pEcuData->channelsLive[rpmIndex]->setMinInt(0);
    m_pEcuData->channelsLive[rpmIndex]->setMaxInt(14000);
    m_pEcuData->channelsLive[rpmIndex]->setValueInt(0);

    int pwIndex = static_cast<int>(EcuData::eChannelLive::CH_PW);
    m_pEcuData->channelsLive[pwIndex]->setDialMinorStepSize(1);
    m_pEcuData->channelsLive[pwIndex]->setMinInt(0);
    m_pEcuData->channelsLive[pwIndex]->setMaxInt(100);
    m_pEcuData->channelsLive[pwIndex]->setValueInt(0);

    int ignAdvanceIndex = static_cast<int>(EcuData::eChannelLive::CH_IGN_ADVANCE);
    m_pEcuData->channelsLive[ignAdvanceIndex]->setDialMinorStepSize(1);
    m_pEcuData->channelsLive[ignAdvanceIndex]->setMinInt(-10);
    m_pEcuData->channelsLive[ignAdvanceIndex]->setMaxInt(50);
    m_pEcuData->channelsLive[ignAdvanceIndex]->setValueInt(0);

    int dwellIndex = static_cast<int>(EcuData::eChannelLive::CH_IGN_DWELL);
    m_pEcuData->channelsLive[dwellIndex]->setDialMinorStepSize(100);
    m_pEcuData->channelsLive[dwellIndex]->setMinInt(0);
    m_pEcuData->channelsLive[dwellIndex]->setMaxInt(10000);
    m_pEcuData->channelsLive[dwellIndex]->setValueInt(0);

    int loopCountIndex = static_cast<int>(EcuData::eChannelLive::CH_LOOP_COUNT);
    m_pEcuData->channelsLive[loopCountIndex]->setMinInt(0);
    m_pEcuData->channelsLive[loopCountIndex]->setMaxInt(2000000);
    m_pEcuData->channelsLive[loopCountIndex]->setValueInt(0);
    m_pEcuData->channelsLive[loopCountIndex]->setDialMinorStepSize(20000);

    // ADC Raw
    int adc0Index = static_cast<int>(EcuData::eChannelLive::CH_ADC0);
    m_pEcuData->channelsLive[adc0Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc0Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc0Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc0Index]->setValueInt(0);

    int adc1Index = static_cast<int>(EcuData::eChannelLive::CH_ADC1);
    m_pEcuData->channelsLive[adc1Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc1Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc1Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc1Index]->setValueInt(0);

    int adc2Index = static_cast<int>(EcuData::eChannelLive::CH_ADC2);
    m_pEcuData->channelsLive[adc2Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc2Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc2Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc2Index]->setValueInt(0);

    int adc3Index = static_cast<int>(EcuData::eChannelLive::CH_ADC3);
    m_pEcuData->channelsLive[adc3Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc3Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc3Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc3Index]->setValueInt(0);

    int adc4Index = static_cast<int>(EcuData::eChannelLive::CH_ADC4);
    m_pEcuData->channelsLive[adc4Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc4Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc4Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc4Index]->setValueInt(0);

    int adc5Index = static_cast<int>(EcuData::eChannelLive::CH_ADC5);
    m_pEcuData->channelsLive[adc5Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc5Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc5Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc5Index]->setValueInt(0);

    int adc6Index = static_cast<int>(EcuData::eChannelLive::CH_ADC6);
    m_pEcuData->channelsLive[adc6Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc6Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc6Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc6Index]->setValueInt(0);

    int adc7Index = static_cast<int>(EcuData::eChannelLive::CH_ADC7);
    m_pEcuData->channelsLive[adc7Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc7Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc7Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc7Index]->setValueInt(0);

    int adc8Index = static_cast<int>(EcuData::eChannelLive::CH_ADC8);
    m_pEcuData->channelsLive[adc8Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc8Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc8Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc8Index]->setValueInt(0);

    int adc9Index = static_cast<int>(EcuData::eChannelLive::CH_ADC0);
    m_pEcuData->channelsLive[adc9Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc9Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc9Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc9Index]->setValueInt(0);

    int adc10Index = static_cast<int>(EcuData::eChannelLive::CH_ADC10);
    m_pEcuData->channelsLive[adc10Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc10Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc10Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc10Index]->setValueInt(0);

    int adc11Index = static_cast<int>(EcuData::eChannelLive::CH_ADC11);
    m_pEcuData->channelsLive[adc11Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc11Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc11Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc11Index]->setValueInt(0);

    int adc12Index = static_cast<int>(EcuData::eChannelLive::CH_ADC12);
    m_pEcuData->channelsLive[adc12Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc12Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc12Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc12Index]->setValueInt(0);

    int adc13Index = static_cast<int>(EcuData::eChannelLive::CH_ADC13);
    m_pEcuData->channelsLive[adc13Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc13Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc13Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc13Index]->setValueInt(0);

    int adc14Index = static_cast<int>(EcuData::eChannelLive::CH_ADC14);
    m_pEcuData->channelsLive[adc14Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc14Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc14Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc14Index]->setValueInt(0);

    int adc15Index = static_cast<int>(EcuData::eChannelLive::CH_ADC15);
    m_pEcuData->channelsLive[adc15Index]->setDialMinorStepSize(51.2);
    m_pEcuData->channelsLive[adc15Index]->setMinInt(0);
    m_pEcuData->channelsLive[adc15Index]->setMaxInt(4096);
    m_pEcuData->channelsLive[adc15Index]->setValueInt(0);

    // ADC Volts
    int adcVoltsCpuIndex = static_cast<int>(EcuData::eChannelLive::CH_ADC_VOLTS_CPU_TEMP);
    m_pEcuData->channelsLive[adcVoltsCpuIndex]->setDialMinorStepSize(0.05);
    m_pEcuData->channelsLive[adcVoltsCpuIndex]->setMinFloat(0.0f);
    m_pEcuData->channelsLive[adcVoltsCpuIndex]->setMaxFloat(3.5f);
    m_pEcuData->channelsLive[adcVoltsCpuIndex]->setValueInt(0);

    int adcVoltsVrefIndex = static_cast<int>(EcuData::eChannelLive::CH_ADC_VOLTS_CPU_VREFINT);
    m_pEcuData->channelsLive[adcVoltsVrefIndex]->setDialMinorStepSize(0.05);
    m_pEcuData->channelsLive[adcVoltsVrefIndex]->setMinFloat(0.0f);
    m_pEcuData->channelsLive[adcVoltsVrefIndex]->setMaxFloat(3.5f);
    m_pEcuData->channelsLive[adcVoltsVrefIndex]->setValueInt(0);

    int adcVoltsVbatIndex = static_cast<int>(EcuData::eChannelLive::CH_ADC_VOLTS_CPU_VBAT);
    m_pEcuData->channelsLive[adcVoltsVbatIndex]->setDialMinorStepSize(0.05);
    m_pEcuData->channelsLive[adcVoltsVbatIndex]->setMinFloat(0.0f);
    m_pEcuData->channelsLive[adcVoltsVbatIndex]->setMaxFloat(3.5f);
    m_pEcuData->channelsLive[adcVoltsVbatIndex]->setValueInt(0);

    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_SERIAL_FRAMES_RECEIVED)]->setValueInt(0);
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_SERIAL_FRAMES_SENT)]->setValueInt(0);

    // Config variables
    m_pEcuData->channelsConfig[EcuData::CH_REQ_FUEL]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_ENGINE_STROKE]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_ENGINE_DISPLACEMENT]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_TRIGGERS_PER_CYCLE]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_INJ_SQUIRTS_PER_CYCLE]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_INJ_DEAD_TIME]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_INJ_SIZE]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_TPS_MAX_ADC]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_TPS_MIN_ADC]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_FUEL_ALGORITHM]->setValueInt(0);
    m_pEcuData->channelsConfig[EcuData::CH_REV_LIMIT]->setValueInt(0);
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    //QMessageBox::StandardButton reply;
    //reply = QMessageBox::question(this, "Exit Application", "Are you sure you want to exit?",
    //                            QMessageBox::Yes|QMessageBox::No);

    //if (reply == QMessageBox::Yes) {
    pEvent->accept();
    //} else {
    //    event->ignore();
    //}
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::setEcuConnected(bool connected)
{
    if (connected)
    {
        m_pLblStatusBarConnected->setStyleSheet("QLabel { background-color : white; color : darkblue; }");
        m_pLblStatusBarConnected->setText("CONNECTED ");
    }
    else
    {
        m_pLblStatusBarConnected->setStyleSheet("QLabel { background-color : white; color : darkred; }");
        m_pLblStatusBarConnected->setText("NOT CONNECTED ");
    }
}

void MainWindow::on_actionFullscreen_toggled(bool checked)
{
    if (checked)
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
}

void MainWindow::on_actionOpen_Tune_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    "EMPRO tune files (*.emptune);;All files (*.*)");

    if (!fileName.isEmpty())
    {
        m_pUi->actionSave_Tune->setEnabled(true);
        m_pUi->actionSave_Tune_As->setEnabled(true);
    }
}

void MainWindow::on_actionSave_Tune_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                                    "EMPRO tune files (*.emptune)");

    if (!fileName.isEmpty())
    {
    }
}

void MainWindow::on_actionSave_Tune_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"),
                                                    "",
                                                    "EMPRO tune files (*.emptune)");

    if (!fileName.isEmpty())
    {
    }
}

MdiSubWindowCustom *MainWindow::addInternalWindow(QString title, QWidget *pContentWidget, int sizeW, int sizeH, bool maximisable)
{
    QMdiArea *area = qobject_cast<QMdiArea *>(m_pTabWidget->currentWidget());

    MdiSubWindowCustom *newWin = new MdiSubWindowCustom(area, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    newWin->setAttribute(Qt::WA_DeleteOnClose);
    newWin->setWindowTitle(title);
    newWin->resize(sizeW, sizeH);
    //newWin->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    if (maximisable)
    {
        newWin->setWindowFlags(newWin->windowFlags() | Qt::WindowMaximizeButtonHint);
    }

    newWin->setWidget(pContentWidget);
    pContentWidget->setParent(newWin);

    area->addSubWindow(newWin);
    newWin->show();

    return newWin;
}

void MainWindow::on_actionAdd_Number_Dislpay_triggered()
{
    // Choose channel dialog
    DialogChannelChoose dialog(this, m_pEcuData);

    dialog.exec();

    if (dialog.result() == QDialog::Accepted)
    {
        // Get selected channel
        int CHANNEL_P = -1;
        for (int i = 0; i < static_cast<int>(m_pEcuData->eChannelLive::CHANNEL_T_COUNT); i++)
        {
            if (m_pEcuData->channelsLive[i]->getName().compare(dialog.getSelectedChannel()) == 0)
            {
                CHANNEL_P = i;
                break;
            }
        }

        if (CHANNEL_P == -1)
        {
            QMessageBox::warning(this, "Error", "Channel name not found");
            dialog.deleteLater();
            return;
        }

        QLCDNumber *mynum = this->createNumberDisplay();
        mynum->setDigitCount(m_pEcuData->channelsLive[CHANNEL_P]->getDigitCount());
        mynum->display(m_pEcuData->channelsLive[CHANNEL_P]->getValueDouble());

        QObject::connect(m_pEcuData->channelsLive[CHANNEL_P],
                         SIGNAL(valueChangedDouble(double)),
                         mynum,
                         SLOT(display(double)));

        QString windowName(m_pEcuData->channelsLive[CHANNEL_P]->getName());

        // Append Units
        if (m_pEcuData->channelsLive[CHANNEL_P]->getUnits().compare("") != 0)
        {
            windowName.append(" [");
            windowName.append(m_pEcuData->channelsLive[CHANNEL_P]->getUnits());
            windowName.append("]");
        }

        this->addInternalWindow(windowName, mynum, 180, 80, false);
    }

    dialog.deleteLater();
}

QLCDNumber *MainWindow::createNumberDisplay()
{
    QLCDNumber *mynum = new QLCDNumber();

    mynum->setSegmentStyle(QLCDNumber::Flat);
    mynum->setAutoFillBackground(true);
    mynum->setFrameStyle(QFrame::NoFrame);

    QPalette mynumPalette = mynum->palette();
    mynumPalette.setColor(mynumPalette.Background, Qt::black);
    mynumPalette.setColor(mynumPalette.Foreground, Qt::yellow);
    mynum->setPalette(mynumPalette);

    return mynum;
}

void MainWindow::on_actionAdd_Tab_triggered()
{
    m_pTabWidget->newTab();
}

void MainWindow::on_actionCascade_triggered()
{
    QMdiArea *area = qobject_cast<QMdiArea *>(m_pTabWidget->currentWidget());

    area->cascadeSubWindows();
}

void MainWindow::on_actionTile_triggered()
{
    QMdiArea *area = qobject_cast<QMdiArea *>(m_pTabWidget->currentWidget());

    area->tileSubWindows();
}

void MainWindow::tabWidgetCustomContextMenuRequested(const QPoint& pos)
{
    if (m_pTabWidget->count() > 0)
    {
        m_pTabWidgetContextMenu->exec(m_pTabWidget->mapToGlobal(pos));
    }
}

void MainWindow::on_actionRename_Tab_triggered()
{
    bool    ok;
    QString text = QInputDialog::getText(this, tr("Rename Tab"),
                                         tr("New name:"), QLineEdit::Normal, m_pTabWidget->tabText(m_pTabWidget->currentIndex()), &ok);

    if (ok && !text.isEmpty())
    {
        m_pTabWidget->setTabText(m_pTabWidget->currentIndex(), text);
    }
}

void MainWindow::on_actionChannel_Setup_Dialog_triggered()
{
    // Choose channel dialog
    QLCDNumber         *mynum = this->createNumberDisplay();
    DialogChannelSetup dialog(this, m_pEcuData, mynum);

    dialog.exec();

    if (dialog.result() == QDialog::Accepted)
    {
    }

    dialog.deleteLater();
}

void MainWindow::on_actionClose_All_Windows_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Close All Windows", "Are you sure you want to close all the windows on this tab?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QMdiArea *area = qobject_cast<QMdiArea *>(m_pTabWidget->currentWidget());

        foreach(QMdiSubWindow * window, area->subWindowList())
        {
            window->deleteLater();
        }
    }
}

void MainWindow::on_actionAdd_Data_Log_Display_triggered()
{
    WidgetDatalogViewControl *contentWidget = new WidgetDatalogViewControl();
    this->addInternalWindow("Data Log", contentWidget, 700, 300, true);
}

void MainWindow::on_actionSerial_Port_Settings_triggered()
{
    m_pSerialSettingsDialog->show();
    m_pSerialSettingsDialog->raise();
    m_pSerialSettingsDialog->activateWindow();
}

void MainWindow::on_actionConnect_triggered()
{
    if (m_pSerial->openSerialPort())
    {
        m_pUi->actionConnect->setEnabled(false);
        m_pUi->actionSerial_Port_Settings->setEnabled(false);
        m_pUi->actionDisconnect->setEnabled(true);
        m_pUi->actionECU_Settings->setEnabled(true);
        m_pUi->actionSensors_Setup->setEnabled(true);
        m_pUi->actionFormat_Onboard_SD->setEnabled(true);
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    m_pSerial->closeSerialPort();
    m_pUi->actionConnect->setEnabled(true);
    m_pUi->actionSerial_Port_Settings->setEnabled(true);
    m_pUi->actionDisconnect->setEnabled(false);
    m_pUi->actionECU_Settings->setEnabled(false);
    m_pUi->actionSensors_Setup->setEnabled(false);
    m_pUi->actionFormat_Onboard_SD->setEnabled(false);
}

void MainWindow::on_actionLog_Settings_triggered()
{
    m_pDialogDatalogSettings->show();
    m_pDialogDatalogSettings->raise();
    m_pDialogDatalogSettings->activateWindow();
}

void MainWindow::on_actionAdd_Dial_Display_triggered()
{
    // Choose channel dialog
    DialogChannelChoose dialog(this, m_pEcuData);

    dialog.exec();

    if (dialog.result() == QDialog::Accepted)
    {
        // Get selected channel
        int CHANNEL_P = -1;
        for (int i = 0; i < static_cast<int>(m_pEcuData->eChannelLive::CHANNEL_T_COUNT); i++)
        {
            if (m_pEcuData->channelsLive[i]->getName().compare(dialog.getSelectedChannel()) == 0)
            {
                CHANNEL_P = i;
            }
        }

        if (CHANNEL_P == -1)
        {
            QMessageBox::warning(this, "Error", "Channel name not found");
            dialog.deleteLater();
            return;
        }

        /*QwtDial *dial = this->createDial();

        dial->setScaleStepSize(ecuData->channelsLive[CHANNEL_P]->getDialMajorStepSize());
        dial->setScaleMaxMinor(ecuData->channelsLive[CHANNEL_P]->getDialMinorStepSize());
        dial->setScale(ecuData->channelsLive[CHANNEL_P]->getMinDouble(), ecuData->channelsLive[CHANNEL_P]->getMaxDouble());
        dial->setValue(ecuData->channelsLive[CHANNEL_P]->getValueDouble());
        */

        RoundDial *dial = new RoundDial(this);
        dial->SetRange(m_pEcuData->channelsLive[CHANNEL_P]->getMinDouble(), m_pEcuData->channelsLive[CHANNEL_P]->getMaxDouble());
        dial->SetStep(m_pEcuData->channelsLive[CHANNEL_P]->getDialMinorStepSize());
        dial->SetUnits(m_pEcuData->channelsLive[CHANNEL_P]->getUnits());
        dial->update_value(m_pEcuData->channelsLive[CHANNEL_P]->getValueDouble());

        RoundDial::RangeList yellowRange;
        yellowRange.append(QPointF(13000, 13500));
        dial->SetYellowRanges(yellowRange);

        RoundDial::RangeList redRange;
        redRange.append(QPointF(13500, 14000));
        dial->SetRedRanges(redRange);

        // Temporarily dissabled, fix it
        //dial->EnableSaffetyRanges();

        QObject::connect(m_pEcuData->channelsLive[CHANNEL_P],
                         SIGNAL(valueChangedDouble(double)),
                         dial,
                         SLOT(update_value(double)));

        QString windowName(m_pEcuData->channelsLive[CHANNEL_P]->getName());

        // Append Units
        if (m_pEcuData->channelsLive[CHANNEL_P]->getUnits().compare("") != 0)
        {
            windowName.append(" [");
            windowName.append(m_pEcuData->channelsLive[CHANNEL_P]->getUnits());
            windowName.append("]");
        }

        this->addInternalWindow(windowName, dial, 200, 220, false);
    }

    dialog.deleteLater();
}

void MainWindow::on_actionSensors_Setup_triggered()
{
    // Show sensor setup dialog
    DialogSensorSetup dialog(this, m_pEcuData);

    dialog.exec();

    if (dialog.result() == QDialog::Accepted)
    {
    }

    dialog.deleteLater();
}

void MainWindow::tabWidgetChanged(int index)
{
    Q_UNUSED(index);

    if (m_pTabWidget->count() < 1)
    {
        // Disable all actions using a tab
        m_pUi->actionAdd_Number_Dislpay->setDisabled(true);
        m_pUi->actionCascade->setDisabled(true);
        m_pUi->actionTile->setDisabled(true);
        m_pUi->actionRename_Tab->setDisabled(true);
        m_pUi->actionClose_All_Windows->setDisabled(true);
        m_pUi->actionAdd_Data_Log_Display->setDisabled(true);
        m_pUi->actionAdd_Dial_Display->setDisabled(true);
    }
    else
    {
        // Enable all action using a tab
        m_pUi->actionAdd_Number_Dislpay->setEnabled(true);
        m_pUi->actionCascade->setEnabled(true);
        m_pUi->actionTile->setEnabled(true);
        m_pUi->actionRename_Tab->setEnabled(true);
        m_pUi->actionClose_All_Windows->setEnabled(true);
        m_pUi->actionAdd_Data_Log_Display->setEnabled(true);
        m_pUi->actionAdd_Dial_Display->setEnabled(true);
    }
}

void MainWindow::on_actionECU_Settings_triggered()
{
    DialogEcuSettings dialog(this);

    dialog.exec();

    if (dialog.result() == QDialog::Accepted)
    {
    }

    dialog.deleteLater();
}

void MainWindow::on_actionFuel_Table_triggered()
{
    m_pDialogFuelTable->show();
    m_pDialogFuelTable->raise();
    m_pDialogFuelTable->activateWindow();

    /*
     * QWidget *contentWidget = new QWidget();
     *
     * QGridLayout *contentLayout = new QGridLayout(contentWidget);
     * contentLayout->setSpacing(0);
     * contentLayout->setContentsMargins(0,0,0,0);
     *
     * QTableWidget *table = new QTableWidget(tableSizeVErows, tableSizeVEcolumns, this);
     * table->verticalHeader()->setDefaultSectionSize(20);
     * table->horizontalHeader()->setDefaultSectionSize(40);
     *
     * QToolBar *toolbar = new QToolBar("toolbar", contentWidget);
     * toolbar->setIconSize(QSize(20,20));
     *
     * QToolButton *btnOpenTable = new QToolButton;
     * //btnOpenTable->setIconSize(QSize(20,20));
     * //btnOpenTable->setFixedSize(20,20);
     * btnOpenTable->setIcon(QIcon(":/icons/images/Folder-document-open-icon.png"));
     *
     * btnOpenTable->setText("Open Table");
     * btnOpenTable->setToolTip("Open table");
     *
     * QToolButton *btnSaveTable = new QToolButton;
     * btnSaveTable->setIcon(QIcon(":/icons/images/Actions-document-save-icon.png"));
     * btnSaveTable->setText("Save Table");
     * btnSaveTable->setToolTip("Save table");
     *
     * toolbar->addWidget(btnOpenTable);
     * toolbar->addWidget(btnSaveTable);
     * toolbar->addSeparator();
     *
     * contentLayout->addWidget(toolbar);
     * contentLayout->addWidget(table);
     *
     * this->addInternalWindow("Volumetric Efficiency Table", contentWidget, 840, 420, true);*/
}

void MainWindow::on_actionDatalog_Settings_triggered()
{
    on_actionLog_Settings_triggered();
}

void MainWindow::on_actionDebug_Increment_triggered()
{
    //_ecuData->channelsLive[_ecuData->CH_RPM]->setValueFloat(_ecuData->channelsLive[_ecuData->CH_RPM]->getValueFloat() + 482);
    //fuelTableModel->setData(fuelTableModel->index(3,3), fuelTableModel->data(fuelTableModel->index(3,3)).toFloat() + 12.1);
    //serial->sendCommand(Serial::COMMAND_TOGGLE_LED);
}

void MainWindow::on_actionDebug_Decrement_triggered()
{
    //_ecuData->channelsLive[_ecuData->CH_RPM]->setValueFloat(_ecuData->channelsLive[_ecuData->CH_RPM]->getValueFloat() - 346);
    m_pFuelTableModel->setData(m_pFuelTableModel->index(3,3), m_pFuelTableModel->data(m_pFuelTableModel->index(3,3)).toFloat() - 15.13152);
}

void MainWindow::on_actionPollingRate_2_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(2);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_4_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(4);
        m_pUi->actionPollingRate_2->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_10_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(10);
        m_pUi->actionPollingRate_2->setChecked(false);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_15_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(15);
        m_pUi->actionPollingRate_2->setChecked(false);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_20_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(20);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_2->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_25_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(25);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_2->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_30_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(30);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_2->setChecked(false);
        m_pUi->actionPollingRate_35->setChecked(false);
    }
}

void MainWindow::on_actionPollingRate_35_triggered(bool checked)
{
    if (checked)
    {
        m_pSerial->setPollingRate(35);
        m_pUi->actionPollingRate_4->setChecked(false);
        m_pUi->actionPollingRate_10->setChecked(false);
        m_pUi->actionPollingRate_15->setChecked(false);
        m_pUi->actionPollingRate_20->setChecked(false);
        m_pUi->actionPollingRate_25->setChecked(false);
        m_pUi->actionPollingRate_30->setChecked(false);
        m_pUi->actionPollingRate_2->setChecked(false);
    }
}

void MainWindow::on_actionReset_Serial_Stats_triggered()
{
    m_pEcuData->channelsLive[static_cast<int>(m_pEcuData->eChannelLive::CH_SERIAL_FRAMES_RECEIVED)]->setValueInt(0);
    m_pEcuData->channelsLive[static_cast<int>(m_pEcuData->eChannelLive::CH_SERIAL_FRAMES_SENT)]->setValueInt(0);
}

void MainWindow::on_actionRead_ECU_triggered()
{
    m_pSerial->sendReadEcuCommand();
}

void MainWindow::on_actionSave_To_ECU_triggered()
{
    m_pSerial->sendFuelTable();
}

void MainWindow::on_actionFormat_Onboard_SD_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Format Onboard SD Card", "Are you sure you want to format the onboard SD card? This will erase all settings and data logs.",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        m_pSerial->sendFormatOnboardSDCommand();
    }
}
