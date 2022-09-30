#include "std_div.h"

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>


//只要实现圆角就行，有边框更好
//可以先不要标题

//https://blog.csdn.net/sinat_31608641/article/details/126474563
//https://blog.csdn.net/sugardu_lhm/article/details/120080793

TStdDiv::TStdDiv(QWidget *parent) : QWidget(parent)
{
    //好像也没调用这里

    //先设置背景色为透明，否则是会先画一个背景的
    //this->SetBackgroundEvent_for_paintEvent();

}//


//先设置背景色为透明，否则是会先画一个背景的
void TStdDiv::SetBackgroundEvent_for_paintEvent()
{
    TStdDiv * div = this;
    div->setAutoFillBackground(true);

    QPalette p = div->palette();

    //QColor color = QColor(44,50,61);

    p.setColor(QPalette::Window, QColor(190,190,190));

    //透明色
    p.setColor(QPalette::Window, QColor(190,190,190, 0));

    //p.setBrush(QPalette::Window,brush);

    div->setPalette(p);
}//


//自绘控件
void TStdDiv::paintEvent(QPaintEvent *event)
{

    //这句话有什么作用 //即Q_UNUSED()函数在程序中没有实质性的作用,用来避免编译器警告
    Q_UNUSED(event)

    //event->Paint;


    QPainter painter(this);

    painter.setBrush(Qt::black);
    //painter.drawRect(0, 0, this->width(), this->height());//先画成黑色


    //一定要先设置成透明 div ，否则是会再有一个背景的

    //绘制圆角矩形//没有 AA 效果，很难看
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setRenderHint(QPainter::HighQualityAntialiasing);   //抗锯齿 //奇怪，还没有 Antialiasing 效果好
    //painter.drawRoundRect(0, 0, this->width(), this->height(), 8, 8);
    painter.drawRoundRect(0, 0, this->width(), this->height(), 150, 150);

//    QPainter painter(ui->widget_5);
//    painter.setBrush(Qt::black);
//    painter.drawRect(0, 0, this->width(), this->height());//先画成黑色
//    if(m_Image.size().width() <= 0)
//        return;
//    //将图像按比例缩放成和窗口一样大小
//    QImage img = m_Image.scaled(this->size(), Qt::KeepAspectRatio);
//    int x = this->width() - img.width();
//    int y = this->height() - img.height();
//    x /=  2;
//    y /= 2;
//    painter.drawImage(QPoint(x, y), img);



}//
