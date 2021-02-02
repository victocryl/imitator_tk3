#include "can_corresp.h"

// конструктор
Can_corresp::Can_corresp()
{
    // инициализируем массивы can
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(tx, init_array, 8);
    memcpy(tx, init_array, 8);


//    pobj_timer_can = new QTimer();   // создаём объект таймера для can



}


// деструктор
Can_corresp::~Can_corresp(){}


void Can_corresp::timer_start(void)
{
//    pobj_timer_can->start();
}

int Can_corresp::a = 3;
