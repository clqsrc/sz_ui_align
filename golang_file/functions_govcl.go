

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


