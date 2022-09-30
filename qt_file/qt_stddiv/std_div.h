#ifndef TSTDDIV_H
#define TSTDDIV_H

#include <QWidget>

class TStdDiv : public QWidget
{
    Q_OBJECT
public:
    explicit TStdDiv(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event) override;//自绘控件

public:
    //先设置背景色为透明，否则是会先画一个背景的
    void SetBackgroundEvent_for_paintEvent();

};

#endif // TSTDDIV_H
