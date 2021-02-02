#include "can_corresp.h"

/*****************  статические атирибуты ***********************/
QTimer *Can_corresp::pobj_can_timer = new QTimer();


// конструктор
Can_corresp::Can_corresp()
{
    // инициализируем массивы can
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(tx, init_array, 8);
    memcpy(tx, init_array, 8);

    // коннекты
//    connect(device, SIGNAL(framesReceived()), this, SLOT(on_mes_received()));



}

// деструктор
Can_corresp::~Can_corresp(){}

void can_tx(void)
{

}

