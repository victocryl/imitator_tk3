#ifndef CAN_INIT_H
#define CAN_INIT_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QDebug>

#include "ui_widget.h"
//#include "can_corresp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Can_init : public QObject
{
    Q_OBJECT

public:
    Can_init(Ui::Widget *_ui);
    ~Can_init();

/******* ВСПОМОГАТЕЛЬНЫЕ АТРИБУТЫ **************************/
uint8_t k;   // счётчик разов создания объекта device

/******* ОСНОВНЫЕ РАБОЧИЕ АТРИБУТЫ **************************/
    Ui::Widget *pobj_ui;        // указатель на объект ui
    QString port_number;
    QString device_name;
    QTimer *pobj_can_timer;     // указатель на объект таймера
    QCanBusDevice *device;      // объект CAN канала
    uint8_t can_status;         // статус канала CAN

    typedef enum on_off
        {
            OFF,
            ON
        }TE_on_off;


/******* МЕТОДЫ **************************/
    void can_init(void);


/******* СЛОТЫ **************************/
public slots:
    void can_channel_control(void);

};



#endif // CAN_INIT_H
