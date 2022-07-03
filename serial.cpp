#include "serial.h"
#include <QMessageBox>
#include "mainwindow.h"
#include "helpers.h"

Serial::Serial(QWidget *pParent, EcuData *pEcuData) : QObject()
{
    this->m_pParentWidget  = pParent;
    this->m_pEcuData = pEcuData;

    m_pSerial      = new QSerialPort(this);
    m_pStopAndWait = new StopAndWait();

    // StopAndWait
    connect(m_pStopAndWait, SIGNAL(processFrame(const uint8_t *, uint16_t)),
            this, SLOT(processFrame(const uint8_t *, uint16_t)));

    connect(m_pStopAndWait, SIGNAL(putCharArray(const uint8_t*,uint16_t)),
            this, SLOT(putCharArray(const uint8_t *, uint16_t)));

    // Serial
    connect(m_pSerial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(m_pSerial, SIGNAL(readyRead()),
            this, SLOT(readData()));

    m_pTimerCommandLiveVars = new QTimer(this);
    connect(m_pTimerCommandLiveVars, SIGNAL(timeout()), this, SLOT(sendLiveVarsCommand()));
    this->setPollingRate(15);
    m_pTimerCommandLiveVars->start();

    m_pTimerCommandAdc = new QTimer(this);
    connect(m_pTimerCommandAdc, SIGNAL(timeout()), this, SLOT(sendRawAdcCommand()));
    //timerCommandAdc->start(1000.0 / 20);
}

Serial::~Serial()
{
}

// Make COBS encoded packet with checksum
void Serial::makeCobsFrameAndSend(const uint8_t *pAppLayerPacket, int appLayerPacketSize)
{
    uint16_t frameSize;
    uint8_t *frame = m_pStopAndWait->makeFrameUnreliable(pAppLayerPacket, (uint16_t)appLayerPacketSize, &frameSize);

    if (frameSize == 0)
        return;

    // Packet is created, now send
    putCharArray(frame, frameSize);
}

// Send raw data
void Serial::putCharArray(const uint8_t *pData, uint16_t dataCount)
{
    if (m_pSerial->isOpen())
    {
        for (int i = 0; i < dataCount; i++)
            m_pSerial->putChar(pData[i]);

        int newValue = m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_SERIAL_FRAMES_SENT)]->getValueInt() + 1;
        m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_SERIAL_FRAMES_SENT)]->setValueInt(newValue);
    }
}

// Called from hdlc
void Serial::processFrame(const uint8_t *pFrameBuffer, uint16_t frameLength)
{
    int newValue = m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_SERIAL_FRAMES_RECEIVED)]->getValueInt() + 1;
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_SERIAL_FRAMES_RECEIVED)]->setValueInt(newValue);

    //qDebug("Frame received");

    enum SerialCommands command = static_cast<SerialCommands>(pFrameBuffer[0]);
    switch (command)
    {
    case COMMAND_LIVE_VARIABLES:
        //qDebug("COMMAND_LIVE_VARIABLES received");
        this->receiveLiveVariables(pFrameBuffer, frameLength);
        break;

    case COMMAND_RAW_ADC:
        //qDebug("COMMAND_RAW_ADC received");
        this->receiveRawADC(pFrameBuffer, frameLength);
        break;

    case COMMAND_FUEL_TABLE_ECU_TO_PC:
        //qDebug("COMMAND_FUEL_TABLE received");
        this->receiveFuelTable(pFrameBuffer, frameLength);
        break;

    default:
        break;
    }
}

void Serial::setPollingRate(int rate)
{
    m_PollingRate    = rate;
    m_PollingTimeout = 1000.0 / m_PollingRate;
    m_pTimerCommandLiveVars->setInterval(m_PollingTimeout);
}

/////////////////////////////////////////////////////////////////////////////////////
/// Send Commands
/////////////////////////////////////////////////////////////////////////////////////
void Serial::sendReadEcuCommand()
{
    //qDebug("COMMAND_FUEL_TABLE sent");
    this->sendCommand(Serial::COMMAND_FUEL_TABLE_ECU_TO_PC);
}

void Serial::sendFormatOnboardSDCommand()
{
    //qDebug("COMMAND_RAW_ADC sent");
    this->sendCommand(Serial::COMMAND_FORMAT_ONBOARD_SD);
}

void Serial::sendRawAdcCommand()
{
    this->sendCommand(Serial::COMMAND_FORMAT_ONBOARD_SD);
}

void Serial::sendLiveVarsCommand()
{
    //qDebug("COMMAND_LIVE_VARIABLES sent");
    this->sendCommand(Serial::COMMAND_LIVE_VARIABLES);
}

void Serial::sendCommand(SerialCommands command)
{
    uint8_t packetSize = 1;
    uint8_t buffer[packetSize];

    buffer[0] = (uint8_t) command;

    makeCobsFrameAndSend(buffer, packetSize);
}
/////////////////////////////////////////////////////////////////////////////////////


void Serial::sendFuelTable()
{
    MainWindow *mainWin = qobject_cast<MainWindow *>(m_pParentWidget);
    uint8_t * u8PointerToFloat;

    int packetSize = ((mainWin->m_pFuelTableModel->rowCount() * mainWin->m_pFuelTableModel->columnCount())
                      + mainWin->m_pFuelTableModel->rowCount() + mainWin->m_pFuelTableModel->columnCount())
                      * 4 + 1; // 4 = data type width, 1 = command char

    uint8_t buffer[packetSize];

    int charOffset = 0;
    buffer[charOffset] = COMMAND_FUEL_TABLE_PC_TO_ECU;      // Byte 0: command
    charOffset++;

    for (int row = 0; row < mainWin->m_pFuelTableModel->actualRowCount(); row++)
    {
        for (int col = 0; col < mainWin->m_pFuelTableModel->actualColumnCount(); col++)
        {
            float fl = mainWin->m_pFuelTableModel->actualBodyValue(row, col);
            u8PointerToFloat = (uint8_t*)&fl;

            buffer[charOffset] = u8PointerToFloat[3];
            charOffset++;
            buffer[charOffset] = u8PointerToFloat[2];
            charOffset++;
            buffer[charOffset] = u8PointerToFloat[1];
            charOffset++;
            buffer[charOffset] = u8PointerToFloat[0];
            charOffset++;
        }
    }

    for (int row = 0; row < mainWin->m_pFuelTableModel->actualRowCount(); row++)
    {
        float fl = mainWin->m_pFuelTableModel->actualRowValue(row);
        u8PointerToFloat = (uint8_t*)&fl;

        buffer[charOffset] = u8PointerToFloat[3];
        charOffset++;
        buffer[charOffset] = u8PointerToFloat[2];
        charOffset++;
        buffer[charOffset] = u8PointerToFloat[1];
        charOffset++;
        buffer[charOffset] = u8PointerToFloat[0];
        charOffset++;
    }

    for (int col = 0; col < mainWin->m_pFuelTableModel->actualColumnCount(); col++)
    {
        float fl = mainWin->m_pFuelTableModel->actualColumnValue(col);
        u8PointerToFloat = (uint8_t*)&fl;

        buffer[charOffset] = u8PointerToFloat[3];
        charOffset++;
        buffer[charOffset] = u8PointerToFloat[2];
        charOffset++;
        buffer[charOffset] = u8PointerToFloat[1];
        charOffset++;
        buffer[charOffset] = u8PointerToFloat[0];
        charOffset++;
    }

    //hdlc->sendData(buffer, packetSize);
    makeCobsFrameAndSend(buffer, packetSize);
}

void Serial::receiveFuelTable(const uint8_t *pFrameBuffer, uint16_t)
{
    MainWindow *mainWin = qobject_cast<MainWindow *>(m_pParentWidget);

    float   floatValue;
    uint8_t *u8PointerToFloat = (uint8_t *)&floatValue;

    int index = 1; // 0 is the command

    for (int row = 0; row < mainWin->m_pFuelTableModel->actualRowCount(); row++)
    {
        for (int col = 0; col < mainWin->m_pFuelTableModel->actualColumnCount(); col++)
        {
            u8PointerToFloat[3] = pFrameBuffer[index];
            index++;
            u8PointerToFloat[2] = pFrameBuffer[index];
            index++;
            u8PointerToFloat[1] = pFrameBuffer[index];
            index++;
            u8PointerToFloat[0] = pFrameBuffer[index];
            index++;

            mainWin->m_pFuelTableModel->actualSetBodyValue(row, col, floatValue);
        }
    }

    for (int row = 0; row < mainWin->m_pFuelTableModel->actualRowCount(); row++)
    {
        u8PointerToFloat[3] = pFrameBuffer[index];
        index++;
        u8PointerToFloat[2] = pFrameBuffer[index];
        index++;
        u8PointerToFloat[1] = pFrameBuffer[index];
        index++;
        u8PointerToFloat[0] = pFrameBuffer[index];
        index++;

        mainWin->m_pFuelTableModel->actualSetRowValue(row, floatValue);
    }

    for (int col = 0; col < mainWin->m_pFuelTableModel->actualColumnCount(); col++)
    {
        u8PointerToFloat[3] = pFrameBuffer[index];
        index++;
        u8PointerToFloat[2] = pFrameBuffer[index];
        index++;
        u8PointerToFloat[1] = pFrameBuffer[index];
        index++;
        u8PointerToFloat[0] = pFrameBuffer[index];
        index++;

        mainWin->m_pFuelTableModel->actualSetColumnValue(col, floatValue);
    }

    mainWin->m_pFuelTableModel->calculateColors();
}

void Serial::receiveLiveVariables(const uint8_t *pFrameBuffer, uint16_t)
{
    float   floatValue;
    uint8_t *u8PointerToFloat = (uint8_t *)&floatValue;

    //MainWindow *mainWin = qobject_cast<MainWindow *>(parent);

    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_RPM)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[5], pFrameBuffer[6]));

    // fuelPulseWidth
    u8PointerToFloat[3] = pFrameBuffer[39];
    u8PointerToFloat[2] = pFrameBuffer[40];
    u8PointerToFloat[1] = pFrameBuffer[41];
    u8PointerToFloat[0] = pFrameBuffer[42];
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_PW)]->setValueFloat(floatValue);

    // sparkAdvance
    u8PointerToFloat[3] = pFrameBuffer[55];
    u8PointerToFloat[2] = pFrameBuffer[56];
    u8PointerToFloat[1] = pFrameBuffer[57];
    u8PointerToFloat[0] = pFrameBuffer[58];
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_IGN_ADVANCE)]->setValueFloat(floatValue);

    // dwell
    u8PointerToFloat[3] = pFrameBuffer[59];
    u8PointerToFloat[2] = pFrameBuffer[60];
    u8PointerToFloat[1] = pFrameBuffer[61];
    u8PointerToFloat[0] = pFrameBuffer[62];
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_IGN_DWELL)]->setValueFloat(floatValue);

    // loopCount
    uint32_t loopCount = U32_CONCAT_U8(pFrameBuffer[63],
                                       pFrameBuffer[64],
                                       pFrameBuffer[65],
                                       pFrameBuffer[66]);
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_LOOP_COUNT)]->setValueInt((int)loopCount);

    // adcVoltsCpuTemp
    u8PointerToFloat[3] = pFrameBuffer[67];
    u8PointerToFloat[2] = pFrameBuffer[68];
    u8PointerToFloat[1] = pFrameBuffer[69];
    u8PointerToFloat[0] = pFrameBuffer[70];
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC_VOLTS_CPU_TEMP)]->setValueFloat(floatValue);

    // adcVoltsCpuVrefint
    u8PointerToFloat[3] = pFrameBuffer[71];
    u8PointerToFloat[2] = pFrameBuffer[72];
    u8PointerToFloat[1] = pFrameBuffer[73];
    u8PointerToFloat[0] = pFrameBuffer[74];
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC_VOLTS_CPU_VREFINT)]->setValueFloat(floatValue);

    // adcVoltsCpuVbat
    u8PointerToFloat[3] = pFrameBuffer[75];
    u8PointerToFloat[2] = pFrameBuffer[76];
    u8PointerToFloat[1] = pFrameBuffer[77];
    u8PointerToFloat[0] = pFrameBuffer[78];
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC_VOLTS_CPU_VBAT)]->setValueFloat(floatValue);
}

void Serial::receiveRawADC(const uint8_t *pFrameBuffer, uint16_t)
{
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC0)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[1], pFrameBuffer[2]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC1)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[3], pFrameBuffer[4]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC2)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[5], pFrameBuffer[6]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC3)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[7], pFrameBuffer[8]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC4)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[9], pFrameBuffer[10]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC5)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[11], pFrameBuffer[12]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC6)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[13], pFrameBuffer[14]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC7)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[15], pFrameBuffer[16]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC8)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[17], pFrameBuffer[18]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC9)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[19], pFrameBuffer[20]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC10)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[21], pFrameBuffer[22]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC11)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[23], pFrameBuffer[24]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC12)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[25], pFrameBuffer[26]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC13)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[27], pFrameBuffer[28]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC14)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[29], pFrameBuffer[30]));
    m_pEcuData->channelsLive[static_cast<int>(EcuData::eChannelLive::CH_ADC15)]->setValueInt(U16_CONCAT_U8(pFrameBuffer[31], pFrameBuffer[32]));
}

/**
 * @brief Serial::openSerialPort
 * @return true if connected successfully
 *         false if connect unsuccessful
 */
bool Serial::openSerialPort()
{
    MainWindow *mainWin = qobject_cast<MainWindow *>(this->m_pParentWidget);

    m_pSerial->setPortName(settings.name);
    m_pSerial->setBaudRate(settings.baudRate);
    m_pSerial->setDataBits(settings.dataBits);
    m_pSerial->setParity(settings.parity);
    m_pSerial->setStopBits(settings.stopBits);
    m_pSerial->setFlowControl(settings.flowControl);
    if (m_pSerial->open(QIODevice::ReadWrite))
    {
        mainWin->setEcuConnected(true);
        mainWin->m_pLblStatusBarMsg1->setText(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                              .arg(settings.name).arg(settings.stringBaudRate).arg(settings.stringDataBits)
                                              .arg(settings.stringParity).arg(settings.stringStopBits).arg(settings.stringFlowControl));
        //mainWin->writeStatusBar(tr("Connected to %1 : %2, %3, %4, %5, %6")
        //                            .arg(settings.name).arg(settings.stringBaudRate).arg(settings.stringDataBits)
        //                            .arg(settings.stringParity).arg(settings.stringStopBits).arg(settings.stringFlowControl));
        return true;
    }
    else
    {
        QMessageBox::critical(m_pParentWidget, tr("Error"), m_pSerial->errorString());
        //mainWin->writeStatusBar(tr("Open serial port error"));
        return false;
    }
}

void Serial::closeSerialPort()
{
    MainWindow *mainWin = qobject_cast<MainWindow *>(this->m_pParentWidget);

    m_pSerial->close();
    mainWin->setEcuConnected(false);
}

void Serial::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(m_pParentWidget, tr("Critical Error"), m_pSerial->errorString());
        closeSerialPort();
    }
}

void Serial::readData()
{
    // Relay received data to StopAndWait
    QByteArray data = m_pSerial->readAll();

    //qDebug() << data.length() << " bytes received";

    for (int i = 0; i < data.length(); i++)
    {
        //qDebug("Byte received %c", (char)data.at(i));
        m_pStopAndWait->receiveData((uint8_t)data.at(i));
    }
}
