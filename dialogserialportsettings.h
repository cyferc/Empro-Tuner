
#pragma once

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QIntValidator>
#include "serial.h"

namespace Ui {
class DialogSerialPortSettings;
}

class DialogSerialPortSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSerialPortSettings(Serial *pSerial, QWidget *pParent);
    ~DialogSerialPortSettings();

private slots:
    void showPortInfo(int idx);
    void on_buttonBox_accepted();
    void checkCustomBaudRatePolicy(int idx);

    void on_pushButtonRefresh_clicked();

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::DialogSerialPortSettings *m_pUi;
    QIntValidator                *m_IntValidator;
    Serial *m_pSerial;
};
