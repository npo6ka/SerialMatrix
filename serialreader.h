#ifndef SERIALREADER_H
#define SERIALREADER_H

#include "datahandler.h"
#include "ledmatrixdrawer.h"
#include <QtSerialPort/QSerialPort>
#include <QComboBox>
#include <QLabel>
#include <QSerialPortInfo>

class SerialReader
{

public:
    explicit SerialReader(DataHandler *handler);

    QSerialPort *getPort() const;
    void setComboBox(QComboBox *value);
    void refreshSerialPortsList();
    void serialRead();
    void tryToConnect();
    void disconnect();
    void setCurPort(const QString &value);
    void setCurPort();
    void setDebugWidget(QLabel *widget);

    void refreshEvent();
    void reconnectEvent();
    void comboBoxValueChanged(const QString &arg1);
private:
    QSerialPort *port;
    DataHandler *handler;
    QComboBox *comboBox;
    int block_mode = 0;
    QLabel *debug;

    void out(QString str);
    void data_byte_handler(uint8_t data);
    void parse_string(QByteArray data);
    void setComboBoxVal(QString str);
    QString getComboBoxVal();
};

#endif // SERIALREADER_H
