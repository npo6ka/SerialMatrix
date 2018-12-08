#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ledmatrixdrawer.h"
#include "serialreader.h"
#include <QLoggingCategory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void comboboxItemChanged(const QString &arg1);
    void serialRead();
    void refreshButtonHandler();
    void stopButtonHandler();
    void reconnectButtonHandler();

    void updateTime();
private:
    Ui::MainWindow *ui;
    uint8_t height = 1;
    uint8_t widht = 1;
    SerialReader *reader;
    LedMatrixDrawer *drawer;
    QTimer *tmr;

    void tryToConnect(QString str);
    void refresh_ports_list();
    void data_byte_handler(uint8_t data);
    void parse_string(QByteArray data);

    QLayout *init_control_panel();
    void init_layout();
    void initTimer();
    void setTimerInterval(int val);
};

#endif // MAINWINDOW_H
