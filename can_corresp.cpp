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
    {tx[i] = 0;}
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(rx, init_array, 8);


/* *********** коннекты ***********************************************/

    // CAN-корреспонденция
    connect(pobj_can_init->pobj_can_timer, SIGNAL(timeout()), this, SLOT(can_tx()));    // отправка посылок по таймеру
//    connect(pobj_can_init->device, SIGNAL(framesReceived()), this, SLOT(can_rx()));   // не могу понять, почему этот коннект здесь не работает (сделал в widget.cpp)

    // задание режимов работы системы
    connect(pobj_ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkBox_changed()));      // режим отключено
    connect(pobj_ui->checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(checkBox_2_changed()));  // режим включено
    connect(pobj_ui->checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(checkBox_3_changed()));  // режим вентиляция
    connect(pobj_ui->checkBox_5, SIGNAL(stateChanged(int)), this, SLOT(checkBox_5_changed()));  // режим охлаждение

    // задание скорости вентилятора
    connect(pobj_ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(speed_increase()));  // прибавить
    connect(pobj_ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(speed_decrease()));  // прибавить


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

/* @brief  Метод слота на клик checkBox (отключено)
 * @param  None
 * @retval None
 */
void Can_corresp::checkBox_changed(void)
{
    uint8_t tmp_sys_mode = tx[DATA0];   // считываем текущий режим
    // снимаем галочки со всех других чекбоксов
    if(pobj_ui->checkBox_2->checkState() == Qt::Checked){pobj_ui->checkBox_2->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_3->checkState() == Qt::Checked){pobj_ui->checkBox_3->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_5->checkState() == Qt::Checked){pobj_ui->checkBox_5->setCheckState(Qt::Unchecked);}
    // устанавливаем статус системы в tx
    if(pobj_ui->checkBox->checkState() == Qt::Checked){tx[DATA0] = SYS_OFF;}
    else{tx[DATA0] = tmp_sys_mode;}
}

/* @brief  Метод слота на клик checkBox_2 (включено)
 * @param  None
 * @retval None
 */
void Can_corresp::checkBox_2_changed(void)
{
    uint8_t tmp_sys_mode = tx[DATA0];   // считываем текущий режим
    // снимаем галочки со всех других чекбоксов
    if(pobj_ui->checkBox->checkState() == Qt::Checked){pobj_ui->checkBox->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_3->checkState() == Qt::Checked){pobj_ui->checkBox_3->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_5->checkState() == Qt::Checked){pobj_ui->checkBox_5->setCheckState(Qt::Unchecked);}
    // устанавливаем статус системы в tx
    if(pobj_ui->checkBox_2->checkState() == Qt::Checked){tx[DATA0] = SYS_ON;}
    else{tx[DATA0] = tmp_sys_mode;}
}

/* @brief  Метод слота на клик checkBox_3 (вентиляция)
 * @param  None
 * @retval None
 */
void Can_corresp::checkBox_3_changed(void)
{
    uint8_t tmp_sys_mode = tx[DATA0];   // считываем текущий режим
    // снимаем галочки со всех других чекбоксов
    if(pobj_ui->checkBox->checkState() == Qt::Checked){pobj_ui->checkBox->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_2->checkState() == Qt::Checked){pobj_ui->checkBox_2->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_5->checkState() == Qt::Checked){pobj_ui->checkBox_5->setCheckState(Qt::Unchecked);}
    // устанавливаем статус системы в tx
    if(pobj_ui->checkBox_3->checkState() == Qt::Checked){tx[DATA0] = SYS_VENT;}
    else{tx[DATA0] = tmp_sys_mode;}
}

/* @brief  Метод слота на клик checkBox_5 (охлаждение)
 * @param  None
 * @retval None
 */
void Can_corresp::checkBox_5_changed(void)
{
    uint8_t tmp_sys_mode = tx[DATA0];   // считываем текущий режим
    // снимаем галочки со всех других чекбоксов
    if(pobj_ui->checkBox->checkState() == Qt::Checked){pobj_ui->checkBox->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_2->checkState() == Qt::Checked){pobj_ui->checkBox_2->setCheckState(Qt::Unchecked);}
    if(pobj_ui->checkBox_3->checkState() == Qt::Checked){pobj_ui->checkBox_3->setCheckState(Qt::Unchecked);}
    // устанавливаем статус системы в tx
    if(pobj_ui->checkBox_5->checkState() == Qt::Checked){tx[DATA0] = SYS_A_COND;}
    else{tx[DATA0] = tmp_sys_mode;}
}

/* @brief  Метод слота на увеличение скорости вентилятора
 * @param  None
 * @retval None
 */
void Can_corresp::speed_increase(void)
{
    uint8_t tmp_vent_speed = tx[DATA1];   // считываем текущую скорость приточника
    if(tmp_vent_speed < 3)
    {
        tmp_vent_speed++;
        tx[DATA1] = tmp_vent_speed;
        pobj_ui->label_9->setNum(tmp_vent_speed);
    }
}

/* @brief  Метод слота на уменьшение скорости вентилятора
 * @param  None
 * @retval None
 */
void Can_corresp::speed_decrease(void)
{
    uint8_t tmp_vent_speed = tx[DATA1];   // считываем текущую скорость приточника
    if(tmp_vent_speed > 0)
    {
        tmp_vent_speed--;
        tx[DATA1] = tmp_vent_speed;
        pobj_ui->label_9->setNum(tmp_vent_speed);
    }
}



