package main

import (
	// 如果你使用自定义的syso文件则不要引用此包
	//_ "github.com/ying32/govcl/pkgs/winappres"

	//clq 说明。上面的文件参考 https://gitee.com/ying32/govcl/wikis/pages?doc_id=102420&sort_id=410058
	//应该只是用来加入 syso 资源文件的，参考 ..\vendor\github.com\ying32\govcl\pkgs\winappres 下的文件
	//如果对 ui 要求不高，不要其实也是可以的。
	//syso 文件是 google 的标准，其实只是原来修改 windows 下的程序的资源的。所以作者在注释中说明了只对 windows 程序生效。
	//----
	//应该也可以使用 ".manifest" 文件。
	//--------------------------------------------------------
	//高分屏的官方设置方法看两个地址
	//https://learn.microsoft.com/zh-cn/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process
	//https://learn.microsoft.com/zh-cn/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows?source=recommendations

	"fmt"

	"github.com/ying32/govcl/vcl"
	"github.com/ying32/govcl/vcl/types"
	// "github.com/ying32/govcl/vcl/api"
	// "sync"
	// "io"
	// "io/ioutil"
	// "fmt"
	// "path/filepath"
	// "os"
	// "os/exec"
	// "encoding/json"
	// "log"
	"strconv"
)

type crect struct {

    left int32;
	top int32;
    width int32; //:number = 0;
    height int32; //:number = 0;
	//t1 int  `json:"user_name" bson:"user_name"`;  //golang 中有种特殊的 "成员标签定义" (Struct tag ) 是字符串。 另外，默认下是不能访问结构体的小写成员的
	//结构体这里需要注意的是，只有公有的成员变量可以被reflect改变值，私有的变量是无法改变值得。
}//

//   // TypeOf returns the reflection Type that represents the dynamic type of variable.
//   // If variable is a nil interface value, TypeOf returns nil.
//   t := reflect.TypeOf(user)
//   // Get the type and kind of our user variable
//   fmt.Println("Type:", t.Name())
//   fmt.Println("Kind:", t.Kind())
//   // Iterate over all available fields and read the tag value
//   for i := 0; i < t.NumField(); i++ {
//     // Get the field, returns https://golang.org/pkg/reflect/#StructField
//     field := t.Field(i)
//     // Get the field tag value
//     tag := field.Tag.Get(tagName)
//     fmt.Printf("%d. %v (%v), tag: '%v'\n", i+1, field.Name, field.Type.Name(), tag)
//   }

//func CreateView(parentView *vcl.TPanel) *vcl.TPanel {
func CreateView(parentView vcl.IWinControl) *vcl.TPanel {

	//var div = vcl.NewPanel(parentView);
	var div = vcl.NewPanel(parentView.Owner()); //按道理这样更合理，因为下面的 parent 实际上一般情况下是可以更改的

	div.SetParent(parentView);
    div.SetBounds(10, 40, 200, 300);

	//div.SetAlign(types.AlLeft);

	div.SetBevelInner(types.BvNone);
	div.SetBevelOuter(types.BvNone);

	div.SetParentFont(true);

	return div;


}//


func struct_t1(){

	type crect2 struct {

		left int32;
		top int32;
	}//

	//v := new (struct Object{aa:7});
	//v := new (struct {aa int}); //ok
	v := new(struct {aa int}); 

	fmt.Println("", v);

	//return div;

}//


//func UI_CreateView(parentView *vcl.TPanel) *vcl.TPanel {
func UI_CreateView(parentView vcl.IWinControl) *vcl.TPanel {
		//parentView.Left();
	return CreateView(parentView);

}//

func View_GetRect(view vcl.IWinControl) crect { //* vcl.TPanel){

	var rect crect;

	rect.left = view.Left();
	rect.top = view.Top();
	rect.width = view.Width();
	rect.height = view.Height();

	return rect;
}//


//放大系数 //可以修改，一般高分屏设置为 2 就可以了，但是有 1.5 这样的情况
var g_ui_scale float64 = 1; //2;

//func View_SetRect(view vcl.IWinControl, rect crect)  { //* vcl.TPanel){
func View_SetRect(view vcl.IControl, rect crect)  { //* vcl.TPanel){

	//放大系数
	var scale float64 = g_ui_scale; //2;

	var left int32 = int32(float64(rect.left)   * scale);
	var top int32  = int32(float64(rect.top)    * scale);
	var w int32    = int32(float64(rect.width)  * scale);
	var h int32    = int32(float64(rect.height) * scale);

	//不行，目前直接操作 vcl.IControl 会失败 //不是，还是可以的
	//view.SetBounds(left, top, left + w, top + h);
	view.SetBounds(left, top, w, h);

	// p, err := view.(*vcl.TPanel);

	// fmt.Println("View_SetRect() error:", err);
	// if (p != nil) { 
	// 	p.SetBounds(left, top, left + w, top + h); 
	// }

}//

//set rect 有时候还是不太方便
func View_SetPos(view vcl.IControl, x,y int32)  { //* vcl.TPanel){

	//放大系数
	var scale float64 = g_ui_scale; //2;

	var x_ int32 = int32(float64(x)   * scale);
	var y_ int32  = int32(float64(y)    * scale);

	//不行，目前直接操作 vcl.IControl 会失败 //不是，还是可以的
	//view.SetBounds(left, top, left + w, top + h);

	view.SetLeft(x_);
	view.SetTop(y_);

	// p, err := view.(*vcl.TPanel);

	// fmt.Println("View_SetRect() error:", err);
	// if (p != nil) { 
	// 	p.SetBounds(left, top, left + w, top + h); 
	// }

}//

//创建一个视图 //2022 可以指定类名，如 "image"
func UI_CreateViewClass(parentView vcl.IWinControl, ui_class string) vcl.IControl {

	if (ui_class == "image") {
		var div = vcl.NewImage(parentView.Owner()); //按道理这样更合理，因为下面的 parent 实际上一般情况下是可以更改的

		div.SetParent(parentView);
		div.SetBounds(10, 10, 40, 40);
		//div.SetAutoSize(false);

		//div.SetAlign(types.AlLeft);

		// div.SetBevelInner(types.BvNone);
		// div.SetBevelOuter(types.BvNone);

		// div.SetParentFont(true);

		div.Show();

		return div;
	}//image

	if (ui_class == "edit") {
		var div = vcl.NewEdit(parentView.Owner()); //按道理这样更合理，因为下面的 parent 实际上一般情况下是可以更改的

		div.SetParent(parentView);
		div.SetBounds(10, 10, 100, 22);

		div.SetAutoSize(false);  //否则调整不了高度
		//div.SetAlign(types.AlLeft);

		// div.SetBevelInner(types.BvNone);
		// div.SetBevelOuter(types.BvNone);

		// div.SetParentFont(true);

		div.SetBorderStyle(types.BsNone);

		div.Show();

		return div;
	}//edit

	return nil;
}//

//设备视图的背景色
func View_SetBackgroundColor_hex(view * vcl.TPanel, hex_color string){

    view.SetColor(HexColor(hex_color));

}//

func View_SetBackgroundColor(view * vcl.TPanel, color types.TColor){

    view.SetColor(color);

}//



//16 进制的颜色//android 和 ios 的颜色值其实都可以转换成 int64 ，所以用它来表示颜色也没是不可以，虽然严格来说应该定义另外一种类型
//vcl 应该是不支持 argb 中的 a 的
//cint64 HexColor(const char * hex);
func HexColor(hex string) (types.TColor) {

	if (7 == len(hex)) { //"#ff0000"
		hex = SubString_byte(hex, 1, len(hex)-1);  //string(str[begin:end])
	}else if (9 == len(hex)) { //"#aaff0000" //argb 形式的话
		hex = SubString_byte(hex, 3, len(hex)-3);
	}else if (6 == len(hex)){

	}else{
		//长度不正确
		return types.TColor(0);
	}//

	sr := string(hex[0:2]);
	sg := string(hex[2:4]);
	sb := string(hex[4:6]);

	r := Hex2U8(sr);
	g := Hex2U8(sg);
	b := Hex2U8(sb);

	return types.TColor(0).RGB(r, g, b);
}//

// cint64 Functions_HexColor(const char * hex);
func Functions_HexColor(hex string) (types.TColor) {

	return HexColor(hex);
}//

//16 进制的单字节转换成整数
func Hex2U8(s string)uint8{

	// s := "CB"
 
	// n, err := strconv.ParseUint(s, 16, 8)
	// if err != nil {
	// 	panic(""Parse Error"")
	// }
	// n2 := uint8(n)
	// f := int(*(*int8)(unsafe.Pointer(&n2)))
	// fmt.Println(f)

	n, err := strconv.ParseUint(s, 16, 8)
	if err != nil {
		//panic(""Parse Error"")
		return 0;
	}
	n2 := uint8(n)
	//f := int(*(*int8)(unsafe.Pointer(&n2)))
	//fmt.Println(f)

	return n2;
}//


