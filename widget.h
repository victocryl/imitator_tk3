#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QTime>
#include <string>

#include "can_init.h"
#include "can_corresp.h"

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
/******* ОСНОВНЫЕ РАБОЧИЕ АТРИБУТЫ **************************/
    Ui::Widget *ui;
    Can_init *pobj_can_init;        // указатель на объект Can_init
    Can_corresp *pobj_can_corresp;  // указатель на объект Can_corresp

    typedef enum on_off
        {
            OFF,
            ON
        }TE_on_off;


};
#endif // WIDGET_H
