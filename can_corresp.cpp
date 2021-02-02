#include "can_corresp.h"

/*****************  статические атирибуты ***********************/
QTimer *Can_corresp::pobj_timer_can = new QTimer();


// конструктор
Can_corresp::Can_corresp()
{
    // инициализируем массивы can
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(tx, init_array, 8);
    memcpy(tx, init_array, 8);



}

// деструктор
Can_corresp::~Can_corresp(){}

