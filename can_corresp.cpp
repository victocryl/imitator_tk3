#include "can_corresp.h"

/*****************  статические атирибуты ***********************/
QTimer *Can_corresp::pobj_can_timer = new QTimer();


// конструктор
Can_corresp::Can_corresp(Can_init *pobj_ci)
{
    pobj_c_i = pobj_ci;


    // инициализируем массивы can
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(tx, init_array, 8);
    memcpy(tx, init_array, 8);

    // коннекты

    // делаем отправку посылок по таймеру


    connect(Can_corresp::pobj_can_timer, SIGNAL(timeout()), this, SLOT(can_tx()));
//    connect(pobj_c_i->device, SIGNAL(framesReceived()), this, SLOT(can_rx()));



}

// деструктор
Can_corresp::~Can_corresp(){}


/* ***************************************************************************************
 *  СЛОТЫ
 * **************************************************************************************/

/* @brief  Метод слота на сигнал таймера по истечениии 1 сек.
 * @param  None
 * @retval None
 */
void Can_corresp::can_tx(void)
{
    static int a = 0;
    a++;
    qDebug() << a;
}


//void Can_corresp::can_rx(void)
//{

//}

