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
    tx[DATA2] = 22;                             // задаём начальную желаемую температуру
    pobj_ui->label_13->setNum(tx[DATA2]);       // и отображаем её в окне
    uint8_t init_array[8] = {0,0,0,0,0,0,0,0};  // массив для инициализации
    memcpy(rx, init_array, 8);


/* *********** коннекты ***********************************************/

    // CAN-корреспонденция
    connect(pobj_can_init->pobj_can_timer, SIGNAL(timeout()), this, SLOT(on_timer()));    // по достижениии таймера макс. значения
//    connect(pobj_can_init->device, SIGNAL(framesReceived()), this, SLOT(can_rx()));   // не могу понять, почему этот коннект здесь не работает (сделал в widget.cpp)

    // задание режимов работы системы
    connect(pobj_ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkBox_changed()));      // режим отключено
    connect(pobj_ui->checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(checkBox_2_changed()));  // режим включено
    connect(pobj_ui->checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(checkBox_3_changed()));  // режим вентиляция
    connect(pobj_ui->checkBox_5, SIGNAL(stateChanged(int)), this, SLOT(checkBox_5_changed()));  // режим охлаждение

    // задание скорости вентилятора
    connect(pobj_ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(speed_increase()));  // прибавить
    connect(pobj_ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(speed_decrease()));  // прибавить

    // установка желаемой температуры
    connect(pobj_ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(temp_increase()));  // прибавить
    connect(pobj_ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(temp_decrease()));  // прибавить

    // сброс ошибок
    connect(pobj_ui->pushButton_6, SIGNAL(clicked(bool)), this, SLOT(btn_reset()));


}

// деструктор
Can_corresp::~Can_corresp(){}

/* ***************************************************************************************
 *  МЕТОДЫ
 * **************************************************************************************/

/* @brief  Метод отправки посылки tx
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
        frame_tx.setFrameId(ID_H_L);
        frame_tx.setPayload(tx);
        pobj_can_init->device->writeFrame(frame_tx);

        pobj_can_init->device->clear(QCanBusDevice::Input);    // очищаем приёмный буфер контроллера can
        // без этого буфер rx может заполняться длинной очередью LIFO и всё будет тормозить
        break;
    }
    default: break;
    }
}


/* @brief  Метод разбора посылки от УКВ
 * @detail Данные уже загружены в массив rx. Здесь реализуется
 * оборажение данных в rx на интерфейсе.
 * @param  None
 * @retval None
 */
void Can_corresp::rx_parsing_ID_UKV(void)
{
    int tmp_evap = rx[DATA0] - 40;  // температура испарителя
    int tmp_supp = rx[DATA1] - 40;  // температура приточника

    if(tmp_evap < -40){pobj_ui->label_27->setNum(0);}  // выводим температуру испарителя
    else{pobj_ui->label_27->setNum(tmp_evap);}
    if(tmp_supp < -40){pobj_ui->label_25->setNum(0);}  // выводим температуру приточника
    else{pobj_ui->label_25->setNum(tmp_supp);}

    // отображние битов ошибок
    if(rx[DATA2] & BIT0_NO_CAN){pobj_ui->checkBox_11->setCheckState(Qt::Checked);}
    else{pobj_ui->checkBox_11->setCheckState(Qt::Unchecked);;}
    if(rx[DATA2] & BIT1_PRESS_FAIL){pobj_ui->checkBox_13->setCheckState(Qt::Checked);}
    else{pobj_ui->checkBox_13->setCheckState(Qt::Unchecked);;}
    if(rx[DATA2] & BIT2_SUPP_TEMP_TOO_HI){pobj_ui->checkBox_12->setCheckState(Qt::Checked);}
    else{pobj_ui->checkBox_12->setCheckState(Qt::Unchecked);;}
    if(rx[DATA2] & BIT3_SUPP_TEMP_SENS_BREAK){pobj_ui->checkBox_10->setCheckState(Qt::Checked);}
    else{pobj_ui->checkBox_10->setCheckState(Qt::Unchecked);;}
    if(rx[DATA2] & BIT4_EVAP_TEMP_TOO_LOW){pobj_ui->checkBox_14->setCheckState(Qt::Checked);}
    else{pobj_ui->checkBox_14->setCheckState(Qt::Unchecked);;}
    if(rx[DATA2] & BIT5_EVAP_TEMP_SENS_BREAK){pobj_ui->checkBox_15->setCheckState(Qt::Checked);}
    else{pobj_ui->checkBox_15->setCheckState(Qt::Unchecked);;}


}


/* ***************************************************************************************
 *  СЛОТЫ
 * **************************************************************************************/

/* @brief  Метод слота на сигнал таймера по истечениии 1 сек.
 * @param  None
 * @retval None
 */
void Can_corresp::on_timer(void)
{
    can_tx();
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
        case ID_UKV:
            // перегрузка данных из массива-буфера char *data в int-овый массив rx
            for(uint8_t i = 0; i < DATA_NUM; i++)
            {rx[i] = (uint8_t)(data[i]);}
            rx_parsing_ID_UKV();   // вызываем ф-ию разбора посылки
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

/* @brief  Метод слота на увеличение желаемой температуры
 * @param  None
 * @retval None
 */
void Can_corresp::temp_increase(void)
{
    uint8_t tmp_temp = tx[DATA2];   // считываем текущую желаемую температуру
    if(tmp_temp < 27)
    {
        tmp_temp++;
        tx[DATA2] = tmp_temp;
        pobj_ui->label_13->setNum(tmp_temp);
    }
}

/* @brief  Метод слота на уменьшение желаемой температуры
 * @param  None
 * @retval None
 */
void Can_corresp::temp_decrease(void)
{
    uint8_t tmp_temp = tx[DATA2];   // считываем текущую желаемую температуру
    if(tmp_temp > 16)
    {
        tmp_temp--;
        tx[DATA2] = tmp_temp;
        pobj_ui->label_13->setNum(tmp_temp);
    }
}

/* @brief  Метод слота на нажатие кнопки сброс
 * @param  None
 * @retval None
 */
void Can_corresp::btn_reset(void)
{
    uint8_t tmp_sys_stat = tx[DATA0];
    if(tmp_sys_stat == SYS_ALARM)
    {
        tx[DATA0] = SYS_RESET;
        pobj_ui->label_15->setText("ошибок нет");
        pobj_ui->label_15->setStyleSheet("QLabel{color: rgb(0, 0, 0); }");  // делаем текст чёрным
    }
}
