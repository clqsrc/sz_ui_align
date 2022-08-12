
#include "qt_functions.h"

#include <QWidget>
#include <QMessageBox>
#include <QPaintDevice>
#include <QApplication>

#include "ndk_cpp/c_view.h"
#include "ndk_cpp/c_ui_json.h"
#include "ndk_cpp/c_functions.h"

//#include "GWindowWidget.h"
#include "app_functions.h"

void FormMain_OnClick_btnSave(chandle param);
void FormMain_OnClick_btnReturn(chandle param);
void FormMain_OnClick_btnExit(chandle param);

//clq 临时方案
struct T_App{

    chandle mainForm;
    chandle btnSave;
    chandle btnExit;

};

T_App _gApp = T_App{};
T_App * gApp = &_gApp;


//创建一个带图片的按钮
chandle App_createImageButton(chandle parentView, crect rect_btn, const char * fn_image);


//本程序的根面板，其实是本程序只有一个非全屏的窗口 //暂时定为占 1/3 高度
void App_createRootView(QWidget * this_)
{


    crect rect = qt_GetRect(this_);

    QWidget * t = new QWidget(this_);
    gApp->mainForm = t;
    t->setParent(this_);

    t->show();

    int w = 280; //rect.width / 3;
    int left = (rect.width - w) /2;
    int top = rect.height / 3;
    int h = 240;  //300

    //t->move(100, 100);
    t->move(left, top);
    //t->resize(400, 300);
    //t->resize(w, 300);
    t->resize(w, h);

    //--------------------------------------------------------
    //顺便把两个按钮位置计算出来好了
    crect rect_btn1;
    int half_w = w / 2;
    int btn_w = 100;
    int btn_left = (half_w - btn_w)/2;
    rect_btn1.left = btn_left;//4; //0;
    rect_btn1.top = 120;
    rect_btn1.width = 100;
    rect_btn1.height = 100;

    crect rect_btn2;
    rect_btn2.left = half_w +btn_left;//4; //0;
    rect_btn2.top = 120;
    rect_btn2.width = 100;
    rect_btn2.height = 100;

    //----
    crect rect_btnReturn;
    rect_btnReturn.left = 4; //0;
    rect_btnReturn.top = 4; //0;
    rect_btnReturn.width = w - 8;
    rect_btnReturn.height = 100;


    //--------------------------------------------------------
    //qt_setcolor(t, QColor(200,0,0)); //
    //qt_setcolor(t, QColor(190,190,190)); //弹出窗口背景色
    qt_setcolor(t, QColor(190,190,190)); //弹出窗口背景色

    /*

    chandle btnSave = CreateView(t);  //android 下有问题, ok 是要加返回值
    //QWidget * view = new QWidget();
    crect rect2;
    rect2.left = 4; //0;
    rect2.top = 4; //0;
    //rect2.width = 100;
    rect2.width = w - 8;
    rect2.height = 80;

    //qt 有缩放比例，不过没有开启的项目中还是要自己算一下

    View_SetCaption(btnSave, "Save Image");

    View_SetRect(btnSave, rect2);
    View_SetBackgroundColor(btnSave, HexColor("#ffff00"));
    View_SetOnClick(btnSave, FormMain_OnClick_btnSave, NULL);
    //View_SetCaption(btnSave, "Save Image");

    //----
    //图片
    //chandle img_save = UI_CreateViewClass(t, "image");
    chandle img_save = UI_CreateViewClass(btnSave, "image");  //点击事件确实可以穿透图片

    rect2 = crect{};
    rect2.left = 4; //0;
    rect2.top = 4; //0;
    //rect2.width = 100;
    rect2.width = w - 8;
    rect2.height = 80;

    //qt 有缩放比例，不过没有开启的项目中还是要自己算一下

    //View_SetCaption(img_save, "Save Image");
    //注意，这是资源文件中的图片
    //Div_SetImage(img_save, ":/images/Left_arrow_color.png");
    //Div_SetImage(img_save, ":/images/save.png");
    //Div_SetImage(img_save, "btn_save.png");

    View_SetRect(img_save, rect2);
    //View_SetBackgroundColor(img_save, HexColor("#ff0000"));
    Div_SetImage(img_save, ":/images/Left_arrow_color.png");    //qt 的实现目前大小变化后都要重新设置图片
    //Div_SetImage(img_save, ":/images/save.png");
    */

    chandle btnReturn = App_createImageButton(t, rect_btnReturn, ":/images/Left_arrow_color.png");
    chandle btnSave = App_createImageButton(t, rect_btn1, ":/images/save.png");
    chandle btnExit = App_createImageButton(t, rect_btn2, ":/images/pix_X.png");

    View_SetBackgroundColor(btnReturn, HexColor("#555555"));
    View_SetBackgroundColor(btnSave, HexColor("#cccccc"));
    View_SetBackgroundColor(btnExit, HexColor("#cccccc"));

    View_SetOnClick(btnReturn, FormMain_OnClick_btnReturn, NULL);
    View_SetOnClick(btnSave, FormMain_OnClick_btnSave, NULL);
    View_SetOnClick(btnExit, FormMain_OnClick_btnExit, NULL);

    return;

    //-------- //ok
    /*
    //clq test 增加一个面板 //ll //确实可以直接加原始 qt 组件
    QWidget * view = new QWidget(this_);
    QWidget * div = view;
    view->setParent(this_);

    view->show();

    view->move(100, 100);
    view->resize(400, 300);

    div->setAutoFillBackground(true);
    QPalette p = div->palette();
    //QColor color = QColor(44,50,61);
    QColor color = QColor(200,0,0);
    p.setColor(QPalette::Window,color);
    //p.setBrush(QPalette::Window,brush);

    div->setPalette(p);

    //showmess
    //QMessageBox::information(NULL, "title", "test");

    //return;
    */
    //--------

}//


//创建一个带图片的按钮
chandle App_createImageButton(chandle parentView, crect rect_btn, const char * fn_image)
{
    //crect rect = qt_GetRect(parentView);

    //--------------------------------------------------------

    chandle btn_view = CreateView(parentView);  //android 下有问题, ok 是要加返回值


    //qt 有缩放比例，不过没有开启的项目中还是要自己算一下

    ////View_SetCaption(btn_view, "Save Image");

    View_SetRect(btn_view, rect_btn);
    View_SetBackgroundColor(btn_view, HexColor("#ffff00"));
    //View_SetOnClick(btnSave, FormMain_OnClick_btnSave, NULL);
    //View_SetCaption(btnSave, "Save Image");

    //----
    //图片
    //chandle img_save = UI_CreateViewClass(t, "image");
    chandle img_view = UI_CreateViewClass(btn_view, "image");  //点击事件确实可以穿透图片

    //qt 有缩放比例，不过没有开启的项目中还是要自己算一下

    //View_SetCaption(img_save, "Save Image");
    //注意，这是资源文件中的图片
    //Div_SetImage(img_save, ":/images/Left_arrow_color.png");
    //Div_SetImage(img_save, ":/images/save.png");
    //Div_SetImage(img_save, "btn_save.png");

    crect rect_img = rect_btn;
    rect_img.left = 0;
    rect_img.top = 0;


    View_SetRect(img_view, rect_img);//rect_btn);
    //View_SetBackgroundColor(img_save, HexColor("#ff0000"));
    ////Div_SetImage(img_view, ":/images/Left_arrow_color.png");    //qt 的实现目前大小变化后都要重新设置图片
    //Div_SetImage(img_save, ":/images/save.png");


    Div_SetImage(img_view, fn_image);    //qt 的实现目前大小变化后都要重新设置图片


    return btn_view;

}//


void FormMain_OnClick_btnSave(chandle param)
{
    //ShowMessage("ok");

    View_Hide(gApp->mainForm);

    menuSaveImage();

}//

void FormMain_OnClick_btnReturn(chandle param)
{
    //ShowMessage("ok");

    View_Hide(gApp->mainForm);

}//


void FormMain_OnClick_btnExit(chandle param)
{
    //ShowMessage("ok");

    //View_Hide(gAPP->mainForm);
    //exit(0);
    qApp->quit();
    //qApp->exit(0);

}//


//本 app 自己的缩放比例 //直接用 dpi 密度计算好了，因为所说浮点值缩放会有问题，所以用整数值就可以了
//手机一般是 2-4 之间
////暂时//逻辑宽度转换为 物理宽度
//int dp2px(int px)
//{
//    return px * gWindowsDpiScale;
//}//

int gWindowsDpiScale = 1;

float GetAppDpiScale()
{
//#include <QPaintDevice>
////逻辑DPI
//	int horizontalDPI = logicalDpiX();
//	int verticalDPI  = logicalDpiY();
////物理DPI （和逻辑DPI不一定相同）
//	int horizontalDPI = physicalDpiX();
//	int verticalDPI  = physicalDpiY();

    //似乎默认是 72


    return 1;
}

int APP_dp2px(int px)
{
    return px * gWindowsDpiScale;
}//

