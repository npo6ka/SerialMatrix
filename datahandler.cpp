#include "datahandler.h"

#include <QLoggingCategory>

DataHandler::DataHandler(LedMatrixDrawer *drawer) :
    drawer(drawer),
    data_mas(nullptr)
{

}

void DataHandler::new_matrix() {
    is_new = true;
    cur_elem = 0;
}

void DataHandler::initDataMas(uint8_t width, uint8_t height)
{
    if (data_mas) {
        free(data_mas);
    } else {
        data_mas = new uint32_t[width * height];
        memset(data_mas, 0, width * height * sizeof(data_mas));
        this->width = width;
        this->height = height;
    }
}

void DataHandler::parse_string(QByteArray data) {
    for (auto elem: data) {
        data_byte_handler(elem);
    }
}

void DataHandler::data_byte_handler(uint8_t data) {
    const char start_text[] = "size";

    if (is_new) {
        if (cur_elem < 4) {
            if (start_text[cur_elem] == data) {
                cur_elem++;
            } else {
                cur_elem = 0;
            }
        } else {
            switch (cur_elem) {
            case 4:
                width = data;
                cur_elem++;
                break;
            case 5:
                height = data;
                cur_elem = 0;
                setMatrixSize(width, height);
            }
        }
    } else if (data_mas != nullptr) {
        int max_size = width * height;

        if (cur_elem / 3 >= max_size) {
            qDebug() << "out of massive range";
            return;
        }
        uint32_t val = data_mas[cur_elem / 3];
        val |= (uint32_t)data << ((cur_elem % 3) * 8);
        data_mas[cur_elem / 3] = val;

        cur_elem++;

        if (cur_elem == max_size * 3) {
            setMatrixValue(data_mas, max_size);

            memset(data_mas, 0, max_size * sizeof(data_mas[0]));
            cur_elem = 0;
        }
    }
}

void DataHandler::setMatrixSize(uint8_t width, uint8_t height) {
    bool is_set = drawer->setSize(width, height);

    if (is_set) {
        initDataMas(width, height);
    }

    is_new = false;
}

void DataHandler::setMatrixValue(uint32_t *mas, int count) {
    drawer->setLedColor(mas, count);
}


