#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("имитатор СУ верхнего уровня для УК типа АК-3");
    pobj_can_init = new Can_init(ui);

    pobj_can_init->can_init();

//    connect(ui->pushButton, SIGNAL(clicked(bool)), pobj_can_init, SLOT(can_channel_control()));  // управляем каналом CAN (подкл/откл)



}

Widget::~Widget()
{
    delete ui;
}

