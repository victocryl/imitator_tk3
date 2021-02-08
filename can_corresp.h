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
#define ID_H_L 0x1FFFFAAA   // id СУ высокого уровня (исходящая)
#define ID_UKV 0x1FFFFBBB   // id УКВ (входящая)

// биты аварий
#define BIT0_NO_CAN               (1 << 0)
#define BIT1_PRESS_FAIL           (1 << 1)
#define BIT2_SUPP_TEMP_TOO_HI     (1 << 2)
#define BIT3_SUPP_TEMP_SENS_BREAK (1 << 3)
#define BIT4_EVAP_TEMP_TOO_LOW    (1 << 4)
#define BIT5_EVAP_TEMP_SENS_BREAK (1 << 5)


#define SYS_RESET 0x55  // команда сброса аварий


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
    QTimer *reset_timer;        // таймер установки SYS_OFF после сброса
    uint8_t flg_reset;          // флаг нажатия кнопки Reset


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


/************ МЕТОДЫ ****************************************/
void can_tx(void);
void rx_parsing_ID_UKV(void);




/************ СЛОТЫ ****************************************/
public slots:
void on_timer(void);
void can_rx(void);
void checkBox_changed(void);
void checkBox_2_changed(void);
void checkBox_3_changed(void);
void checkBox_5_changed(void);

void speed_increase(void);
void speed_decrease(void);

void temp_increase(void);
void temp_decrease(void);
void btn_reset(void);

void on_reset_timer(void);



};






#endif // CAN_CORRESP_H











