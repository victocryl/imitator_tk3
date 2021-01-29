#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
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
    Can_init *pobj_can_init;

    typedef enum on_off
        {
            OFF,
            ON
        }TE_on_off;


};
#endif // WIDGET_H
