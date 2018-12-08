#include "serialreader.h"

#define DEF_COMBOX_TEXT "..."

SerialReader::SerialReader(DataHandler *handler) :
    handler(handler)
{
    port = new QSerialPort;
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
}

QSerialPort *SerialReader::getPort() const
{
    return port;
}

void SerialReader::setDebugWidget(QLabel *widget) {
    debug = widget;
}

void SerialReader::setComboBox(QComboBox *value)
{
    comboBox = value;
}

void SerialReader::out(QString str)
{
    if (debug) {
        debug->setText(str);
    }
}

void SerialReader::setComboBoxVal(QString str)
{
    if (comboBox && comboBox->itemData(comboBox->currentIndex()) != str) {
        comboBox->setCurrentText(str);
    }
}

QString SerialReader::getComboBoxVal() {
    if (comboBox && comboBox->currentIndex() != -1) {
        return comboBox->itemText(comboBox->currentIndex());
    }

    return QString();
}

void SerialReader::refreshSerialPortsList()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    QList<QString> new_ports;
    QList<QString> old_ports;

    new_ports.append(DEF_COMBOX_TEXT);
    for (const QSerialPortInfo &info : infos) {
        new_ports.append(info.portName());
    }
    for (int i = 0; i < comboBox->count(); i++) {
        old_ports.append(comboBox->itemText(i));
    }

    for (const QString &port_name : old_ports) {
        if (new_ports.indexOf(port_name) == -1) {
            for (int i = 0; i < comboBox->count(); i++) {
                if (comboBox->itemText(i) == port_name) {
                    comboBox->removeItem(i);
                    break;
                }
            }
        }
    }

    for (const QString &port_name : new_ports) {
        if (old_ports.indexOf(port_name) == -1) {
            comboBox->addItem(port_name);
            old_ports.append(port_name);
        }
    }
}

void SerialReader::disconnect()
{
    if (port->isOpen()) {
        port->close();
        out("Disconnected");
    }
}

void SerialReader::tryToConnect()
{
    disconnect();
    QString curPortName = getComboBoxVal();

    if (!curPortName.isNull() && curPortName != DEF_COMBOX_TEXT) {
        port->setPortName(curPortName);
        port->open(QIODevice::ReadWrite);
        if (port->error() == QSerialPort::NoError) {
            out("Connected on " + curPortName);
            handler->new_matrix();
            port->write("r", 1);
            return;
        } else {
            out("Connection failed: error: " + QString::number(port->error()));
        }
    }

    block_mode++;
    setComboBoxVal(DEF_COMBOX_TEXT);
    block_mode--;
}

void SerialReader::setCurPort() {
    QString curPortName = getComboBoxVal();

    if (curPortName == DEF_COMBOX_TEXT) {
        const auto infos = QSerialPortInfo::availablePorts();

        if (!infos.isEmpty()) {
            QSerialPortInfo fport = infos.first();
            block_mode++;
            setComboBoxVal(fport.portName());
            block_mode--;
        }
    }
}

void SerialReader::setCurPort(const QString &value)
{
    QString curPortName = getComboBoxVal();

    if (curPortName != value) {
        setComboBoxVal(value);
    }
}

void SerialReader::refreshEvent()
{
    refreshSerialPortsList();

    QString val = getComboBoxVal();

    if (getComboBoxVal() == DEF_COMBOX_TEXT) {
        setCurPort();
    }
}

void SerialReader::reconnectEvent()
{
    refreshSerialPortsList();
    if (getComboBoxVal() == DEF_COMBOX_TEXT) {
        setCurPort();
    }
    tryToConnect();
}

void SerialReader::comboBoxValueChanged(const QString &arg1)
{
    setCurPort(arg1);
    tryToConnect();
}

void SerialReader::serialRead() {
    QByteArray data = port->readAll();
    handler->parse_string(data);
}
