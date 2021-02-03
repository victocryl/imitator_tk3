#ifndef CAN_CORRESP_H
#define CAN_CORRESP_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QCanBusDevice>
#include <QCheckBox>
#include "ui_widget.h"
#include "can_init.h"

// айдишники посылок can
#define ID_H_L 0x00AE0100   // id СУ высокого уровня (входящая)
#define ID_UKV 0x00AC0100   // id УКВ (исходящая)


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Can_corresp : public QObject
{
    Q_OBJECT
public:
    Can_corresp(Ui::Widget *_ui, Can_init *_pobj_can_init);
    ~Can_corresp();

/******** ВСПОМОГАТЕЛЬНЫЕ АТРИБУТЫ **************************/




/******* ОСНОВНЫЕ РАБОЧИЕ АТРИБУТЫ **************************/
    QByteArray tx;              // отправляемая посылка
    uint8_t rx[8];              // получаемая посылка
    Ui::Widget *pobj_ui;        // вспомогательный указатель на объект главного виджета
    Can_init *pobj_can_init;    // вспомогательный указатель на объект Can_init

/*******  enum байтов TX посылок (от Серв. ПО) *************/
    typedef enum canbytes
    {
        DATA0,
        DATA1,
        DATA2,
        DATA3,
        DATA4,
        DATA5,
        DATA6,
        DATA7,
        DATA_NUM
    }TE_canbytes;

/*******  enum статусов системы *************/
    typedef enum s_st
    {
        SYS_OFF,
        SYS_ON,
        SYS_VENT,
        SYS_A_COND,
        SYS_ALARM,
    }TE_sys_stat;




/************ СЛОТЫ ****************************************/
public slots:
void can_tx(void);
void can_rx(void);
void checkBox_changed(void);
void checkBox_2_changed(void);
void checkBox_3_changed(void);
void checkBox_5_changed(void);

void speed_increase(void);
void speed_decrease(void);

void temp_increase(void);
void temp_decrease(void);


};






#endif // CAN_CORRESP_H











