#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCanBus>
#include <QCanBusDevice>
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QTime>
#include <string>

#include "can_init.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
