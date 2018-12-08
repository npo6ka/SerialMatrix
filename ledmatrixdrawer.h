#ifndef LEDMATRIXDRAWER_H
#define LEDMATRIXDRAWER_H

#include "stdint.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

class LedMatrixDrawer
{
public:
    LedMatrixDrawer();
    QLayout *initButtonMassive();
    QLabel *fpsLabel;

    bool setSize(uint8_t width, uint8_t height);
    void setLedColor(uint32_t *mas, int size);
    void show_fps();
    void setFpsLabel(QLabel *value);

private:
    uint8_t height = 0;
    uint8_t width = 0;
    uint8_t fps = 0;
    QList<QPushButton*> buttons;
    QGridLayout *butLayout;
};

#endif // LEDMATRIXDRAWER_H
