
#ifndef _QT_FUNCTIONS_H_
#define _QT_FUNCTIONS_H_

#include <QWidget>

#include "ndk_cpp/c_view.h"

void ShowMessage(QString s);
QString IntToStr(int i);
int StrToInt(QString s);

//设置背景色
void qt_setcolor(QWidget * div, QColor color);

crect qt_GetRect(QWidget * div);

void qt_SetRect(QWidget * div, crect rect);
#endif
