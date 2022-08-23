
#ifndef _ui_panel_qt_h_
#define _ui_panel_qt_h_

#include <QMainWindow>
#include <QLabel>

#include "base_c.h"
#include "lstring.h"

//----------------------------------------------------------------
//标准面板相当于 div 。在通用系统里就是 view

//好像还是得定义一个类，要不响应事件这些很困难
class QtDiv : public QWidget
{
    Q_OBJECT
public:

    //奇怪，似乎 c++ 标准并没有一定要有这个构造函数
    explicit QtDiv(QWidget *parent = nullptr);

    ~QtDiv();
private:
     void showVideo();

public:
     QLabel * txtCaption = NULL;

private slots:
    void slotDecodeError(int error);
    void slotGetOneFrame(QImage image);

protected:
//    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event) ;
    void paintWidget();

    void resizeEvent(QResizeEvent  * size);
    void mouseReleaseEvent(QMouseEvent * e);

private:
    //Ui::PageOne *ui;
    //CVideoPlayer *m_pVideoPlayer;
    //QImage m_Image;

};

//----------------------------------------------------------------



//---------------------------------------------------------------


#endif
