#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "ledmatrixdrawer.h"



class DataHandler
{
public:
    DataHandler(LedMatrixDrawer *drawer);
    void parse_string(QByteArray data);

    void new_matrix();
    void initDataMas(uint8_t masWidth, uint8_t masHeight);

private:
    LedMatrixDrawer *drawer;
    bool is_new = true;
    int cur_elem = 0;
    uint32_t *data_mas;
    uint8_t width;
    uint8_t height;

    void data_byte_handler(uint8_t data);
    void setMatrixSize(uint8_t widht, uint8_t height);
    void setMatrixValue(int *mas, int count);
};

#endif // DATAHANDLER_H
