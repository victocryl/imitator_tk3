#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("имитатор СУ верхнего уровня для УК типа АК-3");
}

Widget::~Widget()
{
    delete ui;
}

