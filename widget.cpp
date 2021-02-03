#include "widget.h"
#include "ui_widget.h"
#include "can_corresp.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("имитатор СУ верхнего уровня для УК типа АК-3");
    pobj_can_init = new Can_init(ui);
    pobj_can_corresp = new Can_corresp(ui, pobj_can_init);

    pobj_can_init->can_init();  // иницализируем канал can

    /************** КОННЕКТЫ ************************************/
    connect(ui->pushButton, SIGNAL(clicked(bool)), this->pobj_can_init, SLOT(can_channel_control()));  // управляем каналом CAN (подкл/откл)
    connect(pobj_can_init->device, SIGNAL(framesReceived()), pobj_can_corresp, SLOT(can_rx())); // пытался сделать этот коннект в Can_corresp - не получилось



}

Widget::~Widget()
{
    delete ui;
}

