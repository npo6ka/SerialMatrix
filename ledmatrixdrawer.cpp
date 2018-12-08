#include "ledmatrixdrawer.h"

#include <QColor>
#include <qsize.h>
#include <QLoggingCategory>

#define MAX_RES_H 800
#define MAX_RES_W 800

LedMatrixDrawer::LedMatrixDrawer()
{
    butLayout = nullptr;
}

QLayout *LedMatrixDrawer::initButtonMassive()
{
    if (butLayout == nullptr) {
        butLayout = new QGridLayout;
        butLayout->setSpacing(0);
    } else {
        /*for (int i = 0; i < buttons.size(); ++i) {
            butLayout->removeWidget(buttons.at(i));
        }*/

        QLayoutItem *child;
        while ((child = butLayout->takeAt(0)) != 0) {
            butLayout->removeItem(child);
            delete child;
        }

        qDeleteAll(butLayout->children());

        buttons.clear();
    }

    if (height != 0 && width != 0) {
        const QSize btnSize = QSize(qMin(MAX_RES_H / height, 50), qMin(MAX_RES_W / width, 50));
        const QColor *bgColor = new QColor(0x7f, 0x7f, 0x7f);

        buttons.reserve(height * width);

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                QPushButton *button = new QPushButton(QString("%1").arg(i * width + j));
                button->setFixedSize(btnSize);
                button->setStyleSheet(QString("background-color: %1").arg(bgColor->name()));

                buttons << button;
                butLayout->addWidget(button, i, j);

                //qDeleteAll(layout->children());
            }
        }
    }

    return butLayout;
}

void LedMatrixDrawer::setFpsLabel(QLabel *value)
{
    fpsLabel = value;
}

void LedMatrixDrawer::show_fps()
{
    fpsLabel->setText("led fps: " + QString::number(fps));
    fps = 0;
}

bool LedMatrixDrawer::setSize(uint8_t w, uint8_t h)
{
    if (width != w || height != h) {
        if(w > 0 && h > 0 && w < 32 && h < 32) {
            width = w;
            height = h;
            initButtonMassive();
            qDebug() << "set matrix size w:" << w << " h:" << h;
            return true;
        } else {
            qDebug() << "incorrect matrix size w:" << w << " h:" << h;
        }
    }
    return false;
}

void LedMatrixDrawer::setLedColor(int *mas, int size)
{
    fps++;
    for(int i = 0; i < size && i < buttons.size(); ++i) {
        buttons[i]->setStyleSheet(QString("background-color: %1").arg((new QColor(mas[i] >> 16 & 0xff, mas[i] >> 8 & 0xff, mas[i] & 0xff))->name()));
    }
}
