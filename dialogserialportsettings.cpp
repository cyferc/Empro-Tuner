#include "dialogserialportsettings.h"
#include "ui_dialogserialportsettings.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QLineEdit>
#include <QStringList>

DialogSerialPortSettings::DialogSerialPortSettings(Serial *pSerial, QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::DialogSerialPortSettings)
{
    m_pUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);

    m_pSerial = pSerial;

    m_IntValidator = new QIntValidator(0, 4000000, this);
    m_pUi->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(m_pUi->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(showPortInfo(int)));
    connect(m_pUi->baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
}

DialogSerialPortSettings::~DialogSerialPortSettings()
{
    delete m_pUi;
}

void DialogSerialPortSettings::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !m_pUi->baudRateBox->itemData(idx).isValid();

    m_pUi->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
    {
        m_pUi->baudRateBox->clearEditText();
        QLineEdit *edit = m_pUi->baudRateBox->lineEdit();
        edit->setValidator(m_IntValidator);
    }
}

void DialogSerialPortSettings::showPortInfo(int idx)
{
    if (idx != -1)
    {
        QStringList list = m_pUi->serialPortInfoListBox->itemData(idx).toStringList();
        m_pUi->descriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        m_pUi->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        m_pUi->serialNumberLabel->setText(tr("Serial number: %1").arg(list.at(3)));
        m_pUi->locationLabel->setText(tr("Location: %1").arg(list.at(4)));
        m_pUi->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(5)));
        m_pUi->pidLabel->setText(tr("Product Identifier: %1").arg(list.at(6)));
    }
}

void DialogSerialPortSettings::fillPortsParameters()
{
    // fill baud rate (is not the entire list of available values,
    // desired values??, add your independently)
    m_pUi->baudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    m_pUi->baudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    m_pUi->baudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    m_pUi->baudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    m_pUi->baudRateBox->addItem(QLatin1String("Custom"));

    // fill data bits
    m_pUi->dataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    m_pUi->dataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    m_pUi->dataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    m_pUi->dataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);
    m_pUi->dataBitsBox->setCurrentIndex(3);

    // fill parity
    m_pUi->parityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    m_pUi->parityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    m_pUi->parityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    m_pUi->parityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    m_pUi->parityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

    // fill stop bits
    m_pUi->stopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_pUi->stopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_pUi->stopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);

    // fill flow control
    m_pUi->flowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    m_pUi->flowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    m_pUi->flowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
}

void DialogSerialPortSettings::fillPortsInfo()
{
    m_pUi->serialPortInfoListBox->clear();
    static const QString blankString = QObject::tr("N/A");
    QString              description;
    QString              manufacturer;
    QString              serialNumber;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QStringList list;

        description  = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        m_pUi->serialPortInfoListBox->addItem(list.first(), list);
    }
}

void DialogSerialPortSettings::updateSettings()
{
    m_pSerial->settings.name = m_pUi->serialPortInfoListBox->currentText();

    // Baud Rate
    if (m_pUi->baudRateBox->currentIndex() == 4)
    {
        // custom baud rate
        m_pSerial->settings.baudRate = m_pUi->baudRateBox->currentText().toInt();
    }
    else
    {
        // standard baud rate
        m_pSerial->settings.baudRate = static_cast<QSerialPort::BaudRate>(
            m_pUi->baudRateBox->itemData(m_pUi->baudRateBox->currentIndex()).toInt());
    }
    m_pSerial->settings.stringBaudRate = QString::number(m_pSerial->settings.baudRate);

    // Data bits
    m_pSerial->settings.dataBits = static_cast<QSerialPort::DataBits>(
        m_pUi->dataBitsBox->itemData(m_pUi->dataBitsBox->currentIndex()).toInt());
    m_pSerial->settings.stringDataBits = m_pUi->dataBitsBox->currentText();

    // Parity
    m_pSerial->settings.parity = static_cast<QSerialPort::Parity>(
        m_pUi->parityBox->itemData(m_pUi->parityBox->currentIndex()).toInt());
    m_pSerial->settings.stringParity = m_pUi->parityBox->currentText();

    // Stop bits
    m_pSerial->settings.stopBits = static_cast<QSerialPort::StopBits>(
        m_pUi->stopBitsBox->itemData(m_pUi->stopBitsBox->currentIndex()).toInt());
    m_pSerial->settings.stringStopBits = m_pUi->stopBitsBox->currentText();

    // Flow control
    m_pSerial->settings.flowControl = static_cast<QSerialPort::FlowControl>(
        m_pUi->flowControlBox->itemData(m_pUi->flowControlBox->currentIndex()).toInt());
    m_pSerial->settings.stringFlowControl = m_pUi->flowControlBox->currentText();
}

void DialogSerialPortSettings::on_buttonBox_accepted()
{
    updateSettings();
    hide();
}

void DialogSerialPortSettings::on_pushButtonRefresh_clicked()
{
    fillPortsInfo();
}
