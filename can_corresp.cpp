#include "can_corresp.h"
#include <QWidget>



// конструктор
Can_corresp::Can_corresp(Ui::Widget *_ui, Can_init *_pobj_can_init)
{
    pobj_ui = _ui;                  // вспомогательный указатель на объект главного виджета
    pobj_can_init = _pobj_can_init; // вспомогательный указатель на объект Can_init

    // инициализируем массивы can
    tx.resize(8);   // устанавливаем размер массива QByteArray tx
    for(uint8_t i = 0; i < DATA_NUM; i++)   // memcpy нельзя использовать из-за проблем приведения даных
    {tx[i] = 2;}
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(rx, init_array, 8);


/* *********** коннекты ***********************************************/

    // CAN-корреспонденция
    connect(pobj_can_init->pobj_can_timer, SIGNAL(timeout()), this, SLOT(can_tx()));    // отправка посылок по таймеру
//    connect(pobj_can_init->device, SIGNAL(framesReceived()), this, SLOT(can_rx()));   // не могу понять, почему этот коннект здесь не работает (сделал в widget.cpp)

    // задание параметров
    connect(pobj_ui->checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(checkBox_2_changed()));


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

        pobj_can_init->device->clear(QCanBusDevice::Input);    // очищаем приёмный буфер контроллера can
        // без этого буфер rx может заполняться длинной очередью LIFO и всё будет тормозить
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
    if(pobj_can_init->can_status)
    {
        QCanBusFrame frame_rx = pobj_can_init->device->readFrame();
        QByteArray array = frame_rx.payload();
        char *data = array.data();  // массив-буфер

        uint32_t tmp_id = frame_rx.frameId();   // считываем ID посылки.

        // здесь идёт разбор посылок. в данном случае мы ждём только одну
        switch(tmp_id)
        {
        case ID_H_L:
            // перегрузка данных из массива-буфера char *data в int-овый массив rx
            for(uint8_t i = 0; i < DATA_NUM; i++)
            {rx[i] = (uint8_t)(data[i]);}
            break;
        default: break;
        }
    }
}

/* @brief  Метод слота на клик checkBox_2
 * @param  None
 * @retval None
 */
void Can_corresp::checkBox_2_changed(void)
{
    qDebug() << "checkBox_2_changed";
}






