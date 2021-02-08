#include "can_init.h"
#include "can_corresp.h"


// конструктор
Can_init::Can_init(Ui::Widget *_ui)
{
    k = 0;   // счётчик разов создания объекта device
    pobj_can_timer = new QTimer();
    pobj_ui = _ui;

}

// деструктор
Can_init::~Can_init(){}


/* ***************************************************************************************
 *  МЕТОДЫ
 * ***************************************************************************************
 */

/* @brief  ф-ия инициализирует канал CAN
 * @param  None
 * @retval None
 */
void Can_init::can_init(void)
{
    QString errorString;    // сюда загружается результат работы ф-ии availableDevices()

    // определили, что плагин для PCAN в наличии
    if (QCanBus::instance()->plugins().contains(QStringLiteral("peakcan")))
    {/*qDebug() << "Plagin available: " << "Yes";*/}

    // Проверяем, есть ли подключенные девайсы
    // Однако наш плагин, похоже, эту опцию не поддерживает (в любом случае пустоту возвращает)
    const QList<QCanBusDeviceInfo> devices = QCanBus::instance()->availableDevices(QStringLiteral("peakcan"), &errorString);
    if (!errorString.isEmpty()){qDebug() << "availableDevices errorString: " << errorString;}

    // считываем полезную инфо об адаптере из контейнера devices
    for (const auto &elem : devices)
    {
      port_number = elem.name();
      device_name = elem.description();
    }

    // Создаём объект device
    device = QCanBus::instance()->createDevice(QStringLiteral("peakcan"), port_number, &errorString);   //QStringLiteral("usb0") - если вместо port_number
    k=1;    // вспом. переменная для искл. повторного созд. объекта device
    device->setConfigurationParameter(QCanBusDevice::BitRateKey, 250000);
}


/* ***************************************************************************************
 *  СЛОТЫ
 * ***************************************************************************************
 */

/* @brief  ф-ия определяет, подключает и отключает канал CAN
 * @param  None
 * @retval None
 */
void Can_init::can_channel_control(void)
{
    QString errorString;    // сюда загружается результат работы ф-ии availableDevices()

    // Проверяем, есть ли подключенные девайсы
    // Однако наш плагин, похоже, эту опцию не поддерживает (в любом случае пустоту возвращает)
    const QList<QCanBusDeviceInfo> devices = QCanBus::instance()->availableDevices(QStringLiteral("peakcan"), &errorString);
    if (!errorString.isEmpty()){qDebug() << "availableDevices errorString: " << errorString;}
//    const QList<QCanBusDeviceInfo> devices;

    // считываем полезную инфо об адаптере из контейнера devices
    for (const auto &elem : devices)
    {
      port_number = elem.name();
      device_name = elem.description();
    }

    if(device_name == "PCAN-USB")   // если драйвер определяет адаптер
    {
        if(k==0)    // его нужно создавать только 1 раз, а то он перезатирается другими вызовами ф-ии
        {
            // Создаём объект device
            device = QCanBus::instance()->createDevice(QStringLiteral("peakcan"), port_number, &errorString);   //QStringLiteral("usb0") - если вместо port_number
            k=1;    // вспом. переменная сбрасывается в ветке else if(device->state() == QCanBusDevice::ConnectedState)
        }

        if(device->state() == QCanBusDevice::UnconnectedState)
        {
            if(!device)
            {
                // Error handling goes here
                qDebug() << errorString;
            }
            else
            {
                device->setConfigurationParameter(QCanBusDevice::BitRateKey, 250000);
                device->connectDevice();
            }

            // выводим инфо о подключении адаптера
            pobj_ui->label_2->setText("адаптер подключен: наименование " + device_name + "; " + "номер порта: " + port_number);
            pobj_ui->label_2->setStyleSheet("QLabel{color: rgb(0, 0, 0); }");  // делаем текст чёрным
            // переименовываем кнопку на "отключить адаптер"
            pobj_ui->pushButton->setText("отключить адаптер x");

            // запускаем таймер посылок can
            pobj_can_timer->start(1000);
            can_status = ON;
        }
        else if(device->state() == QCanBusDevice::ConnectedState)
        {
            device->disconnectDevice(); // отключаем порт
//            k=0;                        // обнуляем вспомогательную переменную

            // изменяем значение переменных на произвольные
            port_number = "0";
            device_name = "0";

            // выводим инфо об отключении адаптера
            pobj_ui->label_2->setText("адаптер отключен");
            pobj_ui->label_2->setStyleSheet("QLabel{color: rgb(0, 0, 0); }");  // делаем текст чёрным
            // переименовываем кнопку на "отключить адаптер"
            pobj_ui->pushButton->setText("подключить адаптер ->");
            // останавливаем таймер посылок can
            pobj_can_timer->stop();
            can_status = OFF;
        }
        else
        {
            pobj_ui->label_2->setText("НЕОПРЕДЕЛЁННОЕ СОСТОЯНИЕ ДРАЙВЕРА АДАПТЕРА");
            pobj_ui->label_2->setStyleSheet("QLabel{color: rgb(255, 10, 0); }");  // делаем текст красным
        }
    }
    else
    {
        // выводим инфо об ошибке подключения
        pobj_ui->label_2->setText("ОШИБКА ПОДКЛЮЧЕНИЯ: адаптер не подключен или не того типа");
        pobj_ui->label_2->setStyleSheet("QLabel{color: rgb(255, 10, 0); }");  // делаем текст красным
    }
}
