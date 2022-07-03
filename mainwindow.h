
#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QCloseEvent>

#include "mdisubwindowcustom.h"
#include "serial.h"
#include "dialogserialportsettings.h"
#include "dialogfueltable.h"
#include "dialogdatalogsettings.h"
#include "ecudata.h"
#include <QLCDNumber>
#include "floattablemodel.h"
#include "Qne/qne.h"
#include "Widgets/tabwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *pParent = nullptr);
    ~MainWindow();
    void setEcuConnected(bool connected);
    QLabel *m_pLblStatusBarMsg1;
    QLCDNumber *createNumberDisplay();
    FloatTableModel *m_pFuelTableModel;

private slots:
    void closeEvent(QCloseEvent *pEvent);
    void on_actionExit_triggered();
    void on_actionFullscreen_toggled(bool arg1);
    void on_actionOpen_Tune_triggered();
    void on_actionSave_Tune_triggered();
    void on_actionSave_Tune_As_triggered();

    // Tab widget
    void tabWidgetChanged(int index);
    void tabWidgetCustomContextMenuRequested(const QPoint& pos);
    void on_actionAdd_Tab_triggered();
    void on_actionRename_Tab_triggered();

    void on_actionCascade_triggered();
    void on_actionTile_triggered();

    void on_actionChannel_Setup_Dialog_triggered();
    void on_actionClose_All_Windows_triggered();

    void on_actionSerial_Port_Settings_triggered();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();

    void on_actionAdd_Data_Log_Display_triggered();
    void on_actionAdd_Dial_Display_triggered();
    void on_actionAdd_Number_Dislpay_triggered();

    void on_actionLog_Settings_triggered();
    void on_actionSensors_Setup_triggered();
    void on_actionECU_Settings_triggered();
    void on_actionDatalog_Settings_triggered();

    void on_actionFuel_Table_triggered();

    void on_actionDebug_Increment_triggered();
    void on_actionDebug_Decrement_triggered();

    void on_actionPollingRate_2_triggered(bool checked);
    void on_actionPollingRate_4_triggered(bool checked);
    void on_actionPollingRate_10_triggered(bool checked);
    void on_actionPollingRate_15_triggered(bool checked);
    void on_actionPollingRate_20_triggered(bool checked);
    void on_actionPollingRate_25_triggered(bool checked);
    void on_actionPollingRate_30_triggered(bool checked);
    void on_actionPollingRate_35_triggered(bool checked);

    void on_actionReset_Serial_Stats_triggered();

    void on_actionRead_ECU_triggered();
    void on_actionSave_To_ECU_triggered();

    void on_actionFormat_Onboard_SD_triggered();

private:
    MdiSubWindowCustom *addInternalWindow(QString title, QWidget *pContentWidget, int sizeW, int sizeH, bool maximisable);
    void setInitialChannelValues();

    Ui::MainWindow           *m_pUi;
    DialogSerialPortSettings *m_pSerialSettingsDialog;
    Serial                 *m_pSerial;
    QMenu                 *m_pTabWidgetContextMenu;
    QLabel                *m_pLblStatusBarConnected;
    DialogFuelTable       *m_pDialogFuelTable;
    DialogDatalogSettings *m_pDialogDatalogSettings;
    EcuData               *m_pEcuData;
    Qne                   *m_pQne;
    TabWidget             *m_pTabWidget;
};
