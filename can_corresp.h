#ifndef CAN_CORRESP_H
#define CAN_CORRESP_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include "ui_widget.h"
#include "can_init.h"

// айдишники посылок can
#define ID_H_L 0x00AE0100   // id СУ высокого уровня
#define ID_UKV 0x00AС0100   // id УКВ


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Can_corresp : public QObject
{
    Q_OBJECT
public:
    Can_corresp(Can_init *pobj_ci);
    ~Can_corresp();

/******** ВСПОМОГАТЕЛЬНЫЕ АТРИБУТЫ **************************/




/******* ОСНОВНЫЕ РАБОЧИЕ АТРИБУТЫ **************************/
    uint8_t tx[8];          // отправляемая посылка
    uint8_t rx[8];          // получаемая посылка
    static QTimer *pobj_can_timer; // ук. на объект таймера
    Can_init *pobj_c_i;


/************ СЛОТЫ ****************************************/
public slots:
void can_tx(void);
//void can_rx(void);


};






#endif // CAN_CORRESP_H











