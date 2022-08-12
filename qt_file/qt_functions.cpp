
#include "qt_functions.h"

#include <QWidget>
#include <QMessageBox>

#include "ndk_cpp/c_view.h"



void ShowMessage(QString s)
{
    //QMessageBox::information(NULL, "title", "test");
    QMessageBox::information(NULL, "title", s);  //这个在安卓下的效果倒也还可以
}//

QString IntToStr(int i)
{
    QString r = QString::number(i);

    return r;
}//

int StrToInt(QString s)
{
    int r = s.toInt();

    return r;
}//

//设置背景色
//https://blog.csdn.net/zengcccc/article/details/7279148
//Qt 之前版本中有关背景色设置的函数如setBackgroundColor() 或是前景色设置函数如setForegroundColor()
//在Qt4中都被废止,统一由QPalette类进行管理.
void qt_setcolor(QWidget * div, QColor color)
{

    //pnlMain->setBackgroundColor(QColor(44,50,61));

    div->setAutoFillBackground(true);

    QPalette p = div->palette();

    //QColor color = QColor(44,50,61);

    p.setColor(QPalette::Window,color);

    //p.setBrush(QPalette::Window,brush);

    div->setPalette(p);


}//

//参考 https://copyfuture.com/blogs-details/202007110858124561vr0dh2u4zxp89l
//实际上有 3 种方法
/*

    Overload drawing event to set background color

 void Widget::paintEvent(QPaintEvent *)
{
QPainter p(this);
p.setPen(Qt::NoPen);
// Set red
p.setBrush(Qt::red);
p.drawRect(rect());
}

    The style sheet sets the background color , Set the style sheet background-color Attribute can . as ：

 QWidget w;

w.setStyleSheet("background-color: red;")

    There are two graph level settings for setting the background , One is the front background ( Graphic Events , style sheet ), The other is the post background ( palette ).
    Drawing events and style sheets cannot be set at the same time , And the rear ( palette ) The background color setting will not be interfered by other methods .
    pure QSS To beautify the interface, it is preferred to use the style sheet to set the background color .
    Drawing events set background color loss performance , It is preferred to use the palette to set the background color .
    Drawing events are suitable for dynamic background colors .

This article is from WeChat official account. - Qt jun （qtjuna）


*/


//----------------------------------------------------------------

crect qt_GetRect(QWidget * div)
{
    crect r = {};
    QPoint pos = div->pos();
    QSize size = div->size();

    r.left = pos.x();
    r.top = pos.y();
    r.width = size.width();
    r.height = size.height();


    return r;
}//


void qt_SetRect(QWidget * div, crect rect)
{

    div->move(rect.left, rect.top);
    div->resize(rect.width, rect.height);

}//
