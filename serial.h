
#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <stdint.h>
#include "StopAndWaitCOPS/StopAndWait.h"
#include "ecudata.h"

class Serial : public QObject
{
    Q_OBJECT

public:
    enum SerialCommands
    {
        COMMAND_LIVE_VARIABLES       = 'a',
        COMMAND_RAW_ADC              = 'q',
        COMMAND_FUEL_TABLE_ECU_TO_PC = 'f',
        COMMAND_FUEL_TABLE_PC_TO_ECU = 't',
        COMMAND_FORMAT_ONBOARD_SD = 'F'
    };

    struct Settings_t
    {
        QString                  name;
        qint32                   baudRate;
        QString                  stringBaudRate;
        QSerialPort::DataBits    dataBits;
        QString                  stringDataBits;
        QSerialPort::Parity      parity;
        QString                  stringParity;
        QSerialPort::StopBits    stopBits;
        QString                  stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString                  stringFlowControl;
    }
    settings;

    Serial(QWidget *pParent, EcuData *pEcuData);
    ~Serial();
    void sendData(QByteArray data);
    void sendCommand(SerialCommands command);
    void setPollingRate(int rate);
    void sendFormatOnboardSDCommand();

public slots:
    bool openSerialPort();
    void closeSerialPort();
    void sendReadEcuCommand();
    void sendFuelTable();
    void processFrame(const uint8_t *pFrameBuffer, uint16_t frameLength);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void sendLiveVarsCommand();
    void sendRawAdcCommand();
    void putCharArray(const uint8_t *pData, uint16_t dataCount);

private:
    void receiveLiveVariables(const uint8_t *pFrameBuffer, uint16_t frameLength);
    void receiveRawADC(const uint8_t *pFrameBuffer, uint16_t frameLength);
    void receiveFuelTable(const uint8_t *pFrameBuffer, uint16_t frameLength);
    void makeCobsFrameAndSend(const uint8_t *pAppLayerPacket, int appLayerPacketSize);

private:
    QWidget     *m_pParentWidget;
    EcuData     *m_pEcuData;
    QSerialPort *m_pSerial;
    StopAndWait *m_pStopAndWait;
    QTimer      *m_pTimerCommandLiveVars;
    QTimer      *m_pTimerCommandAdc;
    int         m_PollingRate;
    int         m_PollingTimeout;
};
