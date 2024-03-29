

//临时备份，不完整

//2023  在 macos 的沙盒机制下必须要有原生对话框选择具体的目录
func govcl_OpenSelectDir() string {

	r := "";
		//vcl.Application.Initialize()
	
		dlg := vcl.NewSelectDirectoryDialog(vcl.Application.MainForm())
		if dlg.Execute() {
			//vcl.ShowMessage("选择的目录为：" + dlg.Directory());
			r = dlg.FileName();
		}
		dlg.Free();
	
		//vcl.Application.Run();

		//vcl.ShowMessage(r);
	
		return r;
}//

func govcl_OpenSelectFile() string {

	r := "";

	/*
	dlgOpen := vcl.NewOpenDialog(mainForm)
	dlgOpen.SetFilter("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*")
	//    dlgOpen.SetInitialDir()
	//	dlgOpen.SetFilterIndex()

	dlgOpen.SetOptions(dlgOpen.Options().Include(types.OfShowHelp, types.OfAllowMultiSelect)) //rtl.Include(, types.OfShowHelp))
	dlgOpen.SetTitle("打开")	
	*/
	
	dlg := vcl.NewOpenDialog(vcl.Application.MainForm())
	if dlg.Execute() {
		//vcl.ShowMessage("选择的目录为：" + dlg.Directory());
		r = dlg.FileName();
	}
	dlg.Free();

	//vcl.Application.Run();

	//vcl.ShowMessage(r);

	return r;
}//

//询问对话框
func ShowMessage_Select_govcl(msg string) bool {
    // 创建一个新的消息框
	// vcl.MessageDlg()
    // dlg := vcl.NewMessageDialog(nil, "Are you sure you want to quit?", vcl.MtConfirmation, vcl.MbYesNo)

    // // 运行对话框并处理响应
    // switch dlg.ShowModal() {
    // case vcl.MrYes:
    //     println("Yes button clicked!")
    // case vcl.MrNo:
    //     println("No button clicked!")
    // }

    // // 关闭对话框
    // dlg.Free()

	// 显示询问对话框
	//if vcl.MessageDlg("Do you want to quit?", types.MtConfirmation, vcl.MbYesNo) == types.MrYes {
	//buttons := types.TMsgDlgButtons(types.MbYes).Include(types.MbNo); //types.MbYes | types.MbNo
	if vcl.MessageDlg(msg, types.MtConfirmation, types.MbYes, types.MbNo) == types.MrYes {
			// 如果用户点击了 "Yes" 按钮，则退出程序
		//vcl.Application.Terminate()
		return true;
	}	

	return false;
}//



//注意, govcl 目前只支持 29 个定时器，所以执行完要立即释放
//线程里生成的定时器是不起作用的，所以暂时只能这样用
func run_in_ui_govcl_v2(thread_run func(), ui_run func()) {

	time := vcl.NewTimer(nil);

	in_thread := false;  //理论上应该加锁

	time.SetOnTimer(func(sender vcl.IObject) {

		if (true == in_thread) { return; }  //线程还没执行完

		time.Free();

		ui_run();

	});
	
	go func ()  {
		in_thread = true;
		//buf := HttpGet_TimeOut(url, 5);
		//buf = HttpGet_TimeOut(url, 5);

		thread_run();

		in_thread = false;

	}();

}//

