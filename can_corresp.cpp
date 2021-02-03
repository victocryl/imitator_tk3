#include "can_corresp.h"


// конструктор
Can_corresp::Can_corresp(Ui::Widget *_ui, Can_init *_pobj_can_init)
{
    pobj_ui = _ui;                  // вспомогательный указатель на объект главного виджета
    pobj_can_init = _pobj_can_init; // вспомогательный указатель на объект Can_init

    // инициализируем массивы can
    tx.resize(8);   // устанавливаем размер массива QByteArray tx
    for(uint8_t i = 0; i < DATA_NUM; i++)   // memcpy нельзя использовать из-за проблем привидения даных
    {tx[i] = 0;}
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(rx, init_array, 8);


/* *********** коннекты ***********************************************/

    // делаем отправку посылок по таймеру
    connect(pobj_can_init->pobj_can_timer, SIGNAL(timeout()), this, SLOT(can_tx()));
//    connect(pobj_can_init->device, SIGNAL(framesReceived()), this, SLOT(can_rx()));   // не могу понять, почему этот коннект здесь не работает



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
    switch(pobj_can_init->can_status)
    {
    case Can_init::OFF:
        pobj_ui->label_2->setText("СНАЧАЛА ПОДКЛЮЧИТЕ АДАПТЕР PCAN-USB");
        pobj_ui->label_2->setStyleSheet("QLabel{color: rgb(255, 10, 0); }");  // делаем текст красным
        break;

    case Can_init::ON:
    {
        // Посылаем посылку
        QCanBusFrame frame_tx;
        frame_tx.setFrameId(ID_UKV);
        frame_tx.setPayload(tx);
        pobj_can_init->device->writeFrame(frame_tx);
        break;
    }
    default: break;
    }
}

/* @brief  Метод слота на сигнал объекта device на получение посылки
 * @param  None
 * @retval None
 */
void Can_corresp::can_rx(void)
{
//    QCanBusFrame frame_rx = device->readFrame();
//        QByteArray array = frame_rx.payload();
//        char *data = array.data();  // массив-буфер

//        uint32_t tmp_id = frame_rx.frameId();   // считываем ID посылки.

//        switch(tmp_id)
//        {
//        case ID_UK_1:
//            // перегрузка данных из масива-буфера в int-овый массив can_rx
//            for(uint8_t i = 0; i < DATA_NUM; i++)
//            {can_rx_UK1[i] = (uint8_t)(data[i]);}
//            break;

//        case ID_UK_2:
//            // перегрузка данных из масива-буфера в int-овый массив can_rx
//            for(uint8_t i = 0; i < DATA_NUM; i++)
//            {can_rx_UK2[i] = (uint8_t)(data[i]);}
//            break;

    static int a = 0;
    a++;
    qDebug() << a;


}







