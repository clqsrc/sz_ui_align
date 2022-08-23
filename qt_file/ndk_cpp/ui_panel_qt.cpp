

#ifdef __cplusplus
extern "C" {
#endif


//
//
//#include "c_functions_windows.h"

#ifdef __cplusplus
}
#endif

#include "c_view.h"
#include "ui_panel.h"
#include "ui_panel_qt.h"
#include "windows_handle_map.h"  //句柄向 windows info 结构体的映射
#include "c_functions_windows.h"

#include <QMainWindow>
#include <qlabel.h>

//----------------------------------------------------------------
//标准面板相当于 div 。在通用系统里就是 view


//----------------------------------------------------------------

//按钮点击事件
void div_DoOnclick(QWidget * view);

//面板的 onsize 事件
void div_DoOnSize(QWidget * view);

//----------------------------------------------------------------


//LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//// 	monitor_message(message);
//	switch (message)
//	{
//	// case WM_MYPAINT:
//	// 	HDC hdc;
//	// 	hdc = GetDC(hWnd);
//	// 	TextOut(hdc, 0, 0, L"012abcABC", 9);
//	// 	ReleaseDC(hWnd, hdc);
//	// 	break;

//    case WM_CTLCOLOREDIT:  //test 奇怪，要在父亲窗口中才行
//        {


//            cint64 color = RGB(255, 0, 0);

//            hb=CreateSolidBrush(color);
//            // SetBkColor(HDC)wParam,InputInfo[CurrentInput].bgcolor)
//            SetBkMode((HDC)wParam,TRANSPARENT);
//            //SetTextColor((HDC)wParam, InputInfo[CurrentInput].fgcolor);
//            SetTextColor((HDC)wParam, RGB(0, 0, 255));

//            return (LRESULT)hb;
//        }


//	case WM_PAINT:
//		{
//            //MessageBox(hWnd, "WM_PAINT", "info", 0);  return 0;
            
//            window_info * win = handle_to_window(hWnd);
            
//            printf("create_window_for_handle() WM_PAINT hWnd:%d\r\n", hWnd);
            
//            if (NULL == win) break; //有这个就是能取到句柄对应的结构体了，一切都好办
            
//            printf("ok! create_window_for_handle() WM_PAINT win->background_color 32:%d\r\n", win->background_color);
//            printf("ok! create_window_for_handle() WM_PAINT win->background_color 64:%lld\r\n", win->background_color);
            
//            //int color = ndkColor_ARGB(win->background_color);
//            cint64 color = ndkColor_ARGB(win->background_color);  //最好还是用 64 位减少麻烦
            
//            //http://blog.sina.com.cn/s/blog_6e65e8cc0100p7qn.html
//            //BeginPaint只能在WM_PAINT处理函数中使用
//            PAINTSTRUCT ps;
//            //LPPAINTSTRUCT pps;
//            HDC hdc = BeginPaint(hWnd, &ps);
            
//            HBRUSH hbrush = CreateSolidBrush(color); //据说要加 -lgdi32 -lwinmm
            
//            RECT rect;
//            GetClientRect(hWnd, &rect);
            
//            //FillRect(hdc, NULL, hbrush); //这个是不行的，会异常
//            FillRect(hdc, &rect, hbrush);

//            //--------
//            //2021.11.06 输出 caption
                        



//            //int caption_color2 = ndkColor_ARGB(win->caption_color);
//            cint64 caption_color2 = ndkColor_ARGB(win->caption_color);
//            printf("caption_color2:%d \r\n", caption_color2);
//            cint64 caption_color = ndkColor_ARGB(win->caption_color);
//            //cint64 caption_color = ndkColor_ARGB(win->caption_color);  //奇怪，有问题
//            //int caption_color = ndkColor_ARGB(Functions_hexStringToColor("#ff00ff00")); //ok
//            //int caption_color = ndkColor_ARGB(Functions_hexStringToColor("#00ff00")); //ok
//            //cint64 caption_color = ndkColor_ARGB(Functions_hexStringToColor("#ffffff")); //ok

//            printf("caption_color:%lld \r\n", caption_color);

//            SetTextColor(hdc, caption_color);  //字体颜色
            
//            //TextOutW(hdc, 0, 0, L"123aaa", 6);  //有些 gcc 支持
//            //TextOutW(hdc, 0, 0, L"aaa", 3);

//            //用 DrawText 简单点，就可以不用计算字符串宽度了
//            //RECT rect = {};
//            //rect.bottom = win->; rect.right = ;
//            //GetWindowRect(h, &rect);

//            //我们的 caption 是 utf8 的
//            //int count_unicode = utf8_to_unicode_winapi(win->caption, sizeof(win->caption)-2);  //空两个比较安全，按道理是一个就行

//            wchar_t caption[256]; //标题
//            int count_unicode = utf8_to_unicode_winapi(win->caption, &caption[0], sizeof(win->caption)-2);

//            DrawTextW(hdc, //要输出中文，用 DrawTextW 代替 DrawText 比较好
//                //"123", 3,
//                //(const wchar_t *)win->caption,
//                (const wchar_t *)caption,
//                //strlen(win->caption),
//                count_unicode,
//                &rect,
//                DT_CENTER
//                | DT_VCENTER //没起作用? 据说要和 DT_SINGLELINE 一起用
//                | DT_SINGLELINE
//                //DT_LEFT //左对齐？
//                //|DT_SINGLELINE //单行?
//                |DT_END_ELLIPSIS //应该是画省略号
//                );

//            //--------

//			break;
//		}


//    case WM_LBUTTONUP:
//    //算了，要左键得了//case WM_RBUTTONUP:
//        {
//            //OnCreate(hwnd);
//            //MessageBox(hWnd, "WM_MBUTTONUP", "info", 0);
//            window_info * win = handle_to_window(hWnd); //通过句柄取得一个 windows 结构体

//            if (NULL == win) break;

//            c_func_on_event func_c = win->func_onclick;
//            //func_c(hWnd);  //不是传递句柄，是传递设置时的参数

//            if (NULL != func_c){
//                func_c(win->func_onclick_param);
//            }else{
//                //MessageBox(hWnd, "WM_MBUTTONUP 无绑定事件", "info", 0);
//            }

//        }
//        break;

//	case WM_DESTROY:
//		{
//			// PostQuitMessage(NULL);
//			// break;
//		}
//	}

//	return DefWindowProc(hWnd, message, wParam, lParam);
//}



////HANDLE create_windows_panel(HANDLE parentView)
//HWND create_windows_panel(HWND parentView)
//{
//    TCHAR szClassName[] = TEXT("TPanel_c");



//    // The class is registered, let's create the program
//    hwnd = CreateWindowEx (
//           0,                   // Extended possibilites for variation
//           szClassName,         // Classname
//           TEXT("cf5t1"), 		// Title Text

//           //这个参数其实最重要
//           //WS_CHILD | WS_VISIBLE | WS_BORDER | WS_GROUP | WS_TABSTOP | ES_WANTRETURN,  //WS_OVERLAPPEDWINDOW, // default window
//           //WS_CHILD | ES_WANTRETURN,  //WS_OVERLAPPEDWINDOW, //default window
//           WS_CHILD | ES_WANTRETURN | WS_CLIPSIBLINGS,  //WS_OVERLAPPEDWINDOW, //default window
//           //WS_CLIPSIBLINGS 风格非常重要，如果没有的话会在 wm_paint 的时候画到别人上面


//           0,    //x
//           0,    //y
//           90,   //width
//           50, 	 //height

//           parentView, //HWND_DESKTOP,        // The window is a child-window to desktop //父窗口
//           NULL,                // No menu
//           hThisInstance,       // Program Instance handler
//           NULL                 // No Window Creation data
//           );

//    //显示 //因为前面没有设置 WS_VISIBLE
//    ShowWindow(hwnd, SW_SHOW);                    //奇怪。后创建的不会提前
//    //ShowWindow(hwnd, SW_HIDE);

//    BringWindowToTop(hwnd);        //提前无效

    
//    //--------
//    //新建一个结构体
//    window_info * win = create_window_for_handle(hwnd);
    
//    printf("create_window_for_handle() \r\n");
//    printf("create_window_for_handle() hWnd:%d\r\n", hwnd);
//    printf("create_window_for_handle() win->background_color:%d\r\n", win->background_color);
    
    
//    //--------

//    return hwnd;

//}//

//----------------------------------------------------------------
#include <QPainter>


//QtDiv::QtDiv(QWidget *parent) : QWidget(parent)
//     // , ui(new Ui::PageOne)
//{
//    //ui->setupUi(this);
//    //ui->widget_5->installEventFilter(this);
//    //showVideo();
//}

QtDiv::QtDiv(QWidget *parent) : QWidget(parent)
     // , ui(new Ui::PageOne)
{
    //ui->setupUi(this);
    //ui->widget_5->installEventFilter(this);
    //showVideo();
}

QtDiv::~QtDiv()
{
    //delete ui;
}

void QtDiv::showVideo()
{
    //m_pVideoPlayer = new CVideoPlayer();
    //connect(m_pVideoPlayer, SIGNAL(signalDecodeError(int)), this, SLOT(slotDecodeError(int)));
    //connect(m_pVideoPlayer, SIGNAL(signalGetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
    //m_pVideoPlayer->start();
}

void QtDiv::slotDecodeError(int error)
{
    //qDebug() << "slotDecodeError======error====" << error;
}

void QtDiv::slotGetOneFrame(QImage image)
{
    //m_Image = image;
    update();//调用update将执行 paintEvent函数
}

bool QtDiv::eventFilter(QObject *watched, QEvent *event) {
//    if (watched == ui->widget_5 && event->type() == QEvent::Paint) {
//        paintWidget();
//    }

    return QWidget::eventFilter(watched, event);
}//

void QtDiv::paintWidget() {
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

//大小变化事件
void QtDiv::resizeEvent(QResizeEvent  * size)
{
    //ShowMessage("QtDiv::resizeEvent");

    QtDiv * div = this;
    QWidget * view = div->txtCaption;

    if (NULL == view) return;

    crect rect = View_GetRect(this);
    rect.left = 0;
    rect.top = 0;

    View_SetRect(view, rect);

    //----------------------------------------------------------------
    //事件
    div_DoOnSize(this);

}//

void QtDiv::mouseReleaseEvent(QMouseEvent * e)
{
    div_DoOnclick(this);
}//

//----------------------------------------------------------------

//QWidget * div_setCaption(QtDiv * parentView, lstring * caption)
QWidget * div_setCaption(QWidget * parentView, lstring * caption)
{
    window_info * win = handle_to_window(parentView);
    if (NULL != win)
    {
        strncpy(&win->caption[0], caption->str, sizeof(win->caption));
    }//

    QtDiv * div = (QtDiv *)parentView;
    QLabel * view = div->txtCaption;

    if (NULL == div->txtCaption)
    {
        view = new QLabel();

        //初始的字符串是居中显示的
        view->setAlignment(Qt::AlignCenter);
    }


    //parentView->addWidget(view);
    view->setText(caption->str);

    view->setParent(parentView);

    view->show();

    //--------
    //QtDiv * div = (QtDiv *)parentView;
    div->txtCaption = view;

    crect rect = View_GetRect(parentView);
    rect.left = 0;
    rect.top = 0;

    //View_SetRect(view, rect); //按道理应该先对齐一次，不过这里其实比较复杂

    return view;
}//

//按钮点击事件
void div_DoOnclick(QWidget * view)
{
    //通过句柄取得一个 windows 结构体
    window_info * win = handle_to_window((QWidget *)view);

    if (NULL == win) return;

    //win->func_onclick = func_c;
    //win->func_onclick_param = param;

    if (NULL == win->func_onclick)
    {
        //ShowMessage("无事件。");
        return;
    }

    win->func_onclick(win->func_onclick_param);
}//

//面板的 onsize 事件
void div_DoOnSize(QWidget * view)
{
    //通过句柄取得一个 windows 结构体
    window_info * win = handle_to_window((QWidget *)view);

    if (NULL == win) return;

    //win->func_onclick = func_c;
    //win->func_onclick_param = param;

    if (NULL == win->func_onsize)
    {
        //ShowMessage("无事件。");
        return;
    }

    win->func_onsize(win->func_onsize_param);

}//


